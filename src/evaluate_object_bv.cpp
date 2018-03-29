#include <iostream>
#include <algorithm>
#include <stdio.h>

#include <vector>
#include <numeric>
#include <dirent.h>
#include <regex.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/adapted/c_array.hpp>

#include "../include/parse.h"
#include "../ui/mainwindow.h"
#include "../include/evaluate_object_bv.h"
BOOST_GEOMETRY_REGISTER_C_ARRAY_CS(cs::cartesian)
#define THRESHOLD_IOU 0.7
#define DEBUG true

//Refer https://en.wikipedia.org/w/index.php?title=Information_retrieval&oldid=793358396#Average_precision


/* Notice 2018-3-16 hxd
 * detection file saving format xxxxxx.txt
 * data format: label x y z l w h phi
 * */
using namespace std;
typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double> > Polygon;

// holding bounding boxes for ground truth and detections

// compute polygon of an oriented bounding box
template <typename T>
Polygon toPolygon(const T& g) {
  using namespace boost::numeric::ublas;
  using namespace boost::geometry;
  matrix<double> mref(2, 2);
  mref(0, 0) = cos(g.ry); mref(0, 1) = sin(g.ry);
  mref(1, 0) = -sin(g.ry); mref(1, 1) = cos(g.ry);

  static int count = 0;
  matrix<double> corners(2, 4);
  double data[] = {g.l / 2, g.l / 2, -g.l / 2, -g.l / 2,
                   g.w / 2, -g.w / 2, -g.w / 2, g.w / 2};
  std::copy(data, data + 8, corners.data().begin());
  matrix<double> gc = prod(mref, corners);
  for (int i = 0; i < 4; ++i) {
    gc(0, i) += g.t1;
    gc(1, i) += g.t3;
  }

  double points[][2] = {{gc(0, 0), gc(1, 0)},{gc(0, 1), gc(1, 1)},{gc(0, 2), gc(1, 2)},{gc(0, 3), gc(1, 3)},{gc(0, 0), gc(1, 0)}};
  Polygon poly;
  append(poly, points);
  return poly;
}
double BoxOverlap_bv(tDetection d, tGroundtruth g, IOU_METHOD criterion=I_DIV_U)
{
  using namespace boost::geometry;
  Polygon gp = toPolygon(g);
  Polygon dp = toPolygon(d);

  std::vector<Polygon> in, un;
  intersection(gp, dp, in);
  union_(gp, dp, un);
  //TODO(hxd-BUG-2018-3-15):The value of IOU turns to zero when there is no intersection between the cubes of detection and groundtruth(in 3D) even though they truly have intersection from birdview(2D)
  double inter_area = in.empty() ? 0 : area(in.front());
  double union_area = area(un.front());
  double o;

  if(criterion==I_DIV_U)     // union
    o = inter_area / union_area;
  else if(criterion==I_DIV_DET) // bbox_a
    o = inter_area / area(dp);
  else if(criterion==I_DIV_GT) // bbox_b
    o = inter_area / area(gp);

  return o;
}

vector<string> get_det_Files( string path)
{
  vector<string> FFiles;
  struct dirent *direntp;

  DIR *dirp = opendir(path.c_str());

  regmatch_t atMatch[5];
  regex_t tReg;
  const char* pattern = "^[0-9]*.txt$";
  regcomp(&tReg, pattern, REG_EXTENDED);

  if (dirp != NULL)
  {
    while ((direntp = readdir(dirp)) != NULL)
      if(regexec(&tReg, direntp->d_name, 1, atMatch, 0)==REG_NOERROR)
      {
        FFiles.push_back(path+"/"+string(direntp->d_name));
      }
    if(FFiles.size()==0) printf("ERROR: There is no available detected file!!\n");
  }
  else{
    printf("ERROR: An error happened in reading detection files!!\n");
  }

  closedir(dirp);
  sort(FFiles.begin(),FFiles.end());
  return FFiles;
}
tGroundtruth robolabel2tgroundtruth(RoboLabel label_sti)
{
  tGroundtruth res;
  res.box.type = label_sti.type_;
  res.l=label_sti.size_[0];
  res.w=label_sti.size_[1];
  res.h=label_sti.size_[2];
  res.ry =label_sti.phi_;
  res.t1=label_sti.x_;
  res.t2=label_sti.y_;
  res.t3=label_sti.z_;

  return res;
}

vector< vector<tGroundtruth> > load_labels(string path)
{
  vector< vector<tGroundtruth> > gt;

  SerilizeLabelFile label;
  try{
        label.serilizeData(path);
     }
  catch(int i)
      {
          printf("%d",i);
      }
  catch(...)
      {
          printf(" meet unknown errror");
      }
  for(int i=0;i<label.data_.size();i++)
  {
    vector<tGroundtruth> gt_tmp;
    for(int j=0;j<label.data_[i].label_v_.size();j++)
    {
      tGroundtruth label_ = robolabel2tgroundtruth(label.data_[i].label_v_[j]);
      if(label_.box.type!="unknown") //Filter label "unknown"
        gt_tmp.push_back(label_);
    }
    gt.push_back(gt_tmp);
  }

  return gt;
}
vector< vector<tDetection> > load_detections(string det_path)
{
  vector< vector<tDetection> > det;
  vector<string> files_name = get_det_Files(det_path);

  for(int i=0;i<files_name.size();i++)
  {
    vector<tDetection> detections;
    FILE *fp = fopen(files_name[i].c_str(),"r");
    if (!fp)
    {
      printf("Load detections fails!!\n");
      return det;
    }
    while (!feof(fp))
    {
      tDetection d;
      char str[255];
      if (fscanf(fp, "%s %lf %lf %lf %lf %lf %lf %lf",  //hxd:read one line
                 str,&d.t1, &d.t2, &d.t3,&d.l, &d.w, &d.h,&d.ry)==8)
      {
        // d.thresh = 1;
        d.box.type = str;//TODO:add type
        d.h=10.0;//TODO: tmp use
        if(d.box.type!="unknown")
          detections.push_back(d);
      }
    }
    fclose(fp);
    det.push_back(detections);
  }

  return det;
}

int eval_frame(vector<tGroundtruth> &gt, vector<tDetection> &det, float &recall, float &precise,IOU_METHOD method=I_DIV_U, double thresh = 0.7)
{
  //TODO: deal with the case that a frame has no object or detections
  if(gt.size()==0)
  {
    if(det.size() == 0)
    {
      printf("  warning get one frame without objects or detections,turn recall&precise to 1.0\n");
      recall = 1.0;
      precise = 1.0;
      return 0;
    }
    else
    {
      printf("  warning get one frame without objects but detections,turn recall&precise to 0.0\n");
      recall = 0.0;
      precise = 0.0;
      return 0;
    }
  }
  else if(det.size()==0)
  {
    printf("  warning get one frame without detections but objects,turn recall&precise to 0.0\n");
    recall=0.0;
    precise=0.0;
    return 0;
  }

  // evaluate all ground truth boxes
  double res_overlaps[det.size()][gt.size()];
  for (int32_t i=0; i<gt.size(); i++)
  {
    // search for a possible detection
    for(int32_t j = 0; j < det.size(); j++)
    {
      // find the maximum score for the candidates and get idx of respective detection
      double overlap = BoxOverlap_bv(det[j], gt[i], method);
      res_overlaps[j][i]=overlap;
    }
  }

  int detected_gt_cnt=0;
  for (int m=0;m<gt.size();m++)
  {
    for (int n=0;n<det.size();n++)
    {
      if(res_overlaps[n][m]>thresh)
      {
        detected_gt_cnt++;
        break;
      }
    }
  }
  recall = (float)detected_gt_cnt/gt.size();

  int right_det_cnt=0;
  for (int n=0;n<det.size();n++)
  {
    for (int m=0;m<gt.size();m++)
    {
      if(res_overlaps[n][m]>thresh)
      {
        right_det_cnt++;
        break;
      }
    }
  }
  precise = (float)right_det_cnt/det.size();

  return 0;
}

//int32_t mian2 (string gt_path,string det_path,float *recall_tol,float *precise_tol)
//{
//  char buf[256];
//  getcwd(buf,sizeof(buf));
//  string path = buf;
////  string gt_path = path+"/data/label";
////  string det_path = path+"/data/detections";

//  vector< vector<tGroundtruth> > groundtruth = load_labels(gt_path);
//  vector< vector<tDetection> > detections = load_detections(det_path);

////  float recall_tol =0.0;
////  float precise_tol = 0.0;
//  printf("Start the evaluation(IOU:%.2f) on bv...\n",THRESHOLD_IOU);
//  for(int i=0;i<detections.size();i++)
//  {
//    float recall =0.0;
//    float precise = 0.0;
//    eval_frame(groundtruth[i], detections[i], recall, precise, I_DIV_DET,THRESHOLD_IOU);
//    if(DEBUG)
//      printf("  frame %d,recall: %f, precision: %f\n",i,recall,precise);
//    *recall_tol +=recall;
//    *precise_tol+=precise;
//  }
//  *recall_tol/=detections.size();
//  *precise_tol/=detections.size();

//  printf("The average result of %lu frames: recall: %f, precision: %f",detections.size(),recall_tol,precise_tol);
//  return 0;
//}

