#include <stdio.h>
enum IOU_METHOD
{
  I_DIV_U,
  I_DIV_DET,
  I_DIV_GT
};
using namespace std;struct tBox {
    string  type;     // object type as car, pedestrian or cyclist,...
    double   x1;      // left corner
    double   y1;      // top corner
    double   x2;      // right corner
    double   y2;      // bottom corner
    double   alpha;   // image orientation
    tBox (string type, double x1,double y1,double x2,double y2,double alpha) :
            type(type),x1(x1),y1(y1),x2(x2),y2(y2),alpha(alpha) {}
  };
  // holding ground truth data
  struct tGroundtruth {
    tBox    box;        // object type, box, orientation
    double  truncation; // truncation 0..1
    int32_t occlusion;  // occlusion 0,1,2 (non, partly, fully)
    double ry;
    double  t1, t2, t3;
    double h, w, l;
    tGroundtruth () :
            box(tBox("invalild",-1,-1,-1,-1,-10)),truncation(-1),occlusion(-1) {}
    tGroundtruth (tBox box,double truncation,int32_t occlusion) :
            box(box),truncation(truncation),occlusion(occlusion) {}
    tGroundtruth (string type,double x1,double y1,double x2,double y2,double alpha,double truncation,int32_t occlusion) :
            box(tBox(type,x1,y1,x2,y2,alpha)),truncation(truncation),occlusion(occlusion) {}
  };
  // holding detection data
  struct tDetection {
    tBox    box;    // object type, box, orientation
    double  thresh; // detection score
    double  ry;
    double  t1, t2, t3;
    double  h, w, l;
    tDetection ():
            box(tBox("invalid",-1,-1,-1,-1,-10)),thresh(-1000) {}
    tDetection (tBox box,double thresh) :
            box(box),thresh(thresh) {}
    tDetection (string type,double x1,double y1,double x2,double y2,double alpha,double thresh) :
            box(tBox(type,x1,y1,x2,y2,alpha)),thresh(thresh) {}
  };

//int32_t bbox_eval_api (std::string gt_path,std::string det_path,float *recall_tol,float *precise_tol);

vector< vector<tGroundtruth> > load_labels(string path);
vector< vector<tDetection> > load_detections(string det_path);
int32_t eval_frame(vector<tGroundtruth> &gt, vector<tDetection> &det, float &recall, float &precise,IOU_METHOD method, double thresh);
