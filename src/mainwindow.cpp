#include "../ui/mainwindow.h"
#include "../ui/ui_mainwindow.h"
#include <QFileDialog>
#include <string>
#include <iostream>
#include <fstream>
#include <time.h>
#include "../include/evaluate_object_bv.h"

using namespace std;
QString getTime();
string getUserName();
bool check_path(string FILENAME);

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::on_start_button_clicked()
{//Start eval
  QString metric_method = ui->metric_method->currentText();
  ui->debug_info->setText("START "+metric_method+" Evalation....");
  QString det_path=  ui->det_path->text();
  QString gt_path=  ui->gt_path->text();
  bool debug_flag = ui->checkBox->isChecked();
  float thres = ui->value_iou->value();
  QString metric = ui->metric_method->currentText();
  if(metric == "bv_eval")
  {
    if( check_path(det_path.toStdString())&& check_path(gt_path.toStdString())  )
    {
      float recall_tol = 0.0;
      float precise_tol = 0.0;
      int res = MainWindow::bbox_eval(gt_path.toStdString(),det_path.toStdString(),&recall_tol,&precise_tol,debug_flag,thres);
      if(res!=0)
      {
        QString time = getTime();
        ui->debug_info->append(time+QString("Total Recall: %1").arg(recall_tol,0,'f',3)+QString("  Total Precise: %1").arg(precise_tol,0,'f',3));
      }
    }
    else if(!check_path(gt_path.toStdString()))
    {  ui->debug_info->setTextColor(Qt::red);
      ui->debug_info->append("  Gt File not found! Please reedit");
      ui->debug_info->setTextColor(Qt::black);
    }
    else if(!check_path(det_path.toStdString()))
    {
      ui->debug_info->setTextColor(Qt::red);
      ui->debug_info->append("  Det path not found! Please reedit");
      ui->debug_info->setTextColor(Qt::black);
    }
  }
  else if(metric == "pixel_eval")
  {
    ui->debug_info->setTextColor(Qt::green);
    ui->debug_info->append("\nWarning: Pixel-wise evalation has not been done yet!\n");
    ui->debug_info->setTextColor(Qt::black);
  }


}

void MainWindow::on_det_file_button_clicked()
{//gt_path_choose
  std::string host_name = getUserName();
  QString file_name = QFileDialog::getExistingDirectory(this,"Choose a det path","/home/"+QString::fromStdString(host_name));
  if(file_name!="")
    ui->det_path->setText(file_name);
}

void MainWindow::on_gt_file_button_clicked()
{//det_path_choose
  std::string host_name = getUserName();
  QString file_name = QFileDialog::getOpenFileName(this,"Choose a det .txt file","/home/"+QString::fromStdString(host_name),"*.txt");
  if(file_name!="")
    ui->gt_path->setText(file_name);

}


void MainWindow::on_gt_path_editingFinished()
{
//    ui->debug_info->append("Checking gt file existence....");
}

void MainWindow::on_det_path_editingFinished()
{
//    ui->debug_info->append("Checking det file existence....");
}

int32_t MainWindow::bbox_eval(std::string gt_path,std::string det_path,float *recall_tol,float *precise_tol,bool DEBUG,float THRESHOLD_IOU)
{
  vector< vector<tGroundtruth> > groundtruth = load_labels(gt_path);
  vector< vector<tDetection> > detections = load_detections(det_path);
  ui->debug_info->append(QString::fromStdString("   setting IOU: %1,  metric: bird view ").arg(THRESHOLD_IOU));
  if(detections.size()==0)
  {
    ui->debug_info->setTextColor(Qt::red);
    ui->debug_info->append("\nError: Detection path or file has no available data!\n");
    ui->debug_info->setTextColor(Qt::black);
    return 0;
  }
  else if(groundtruth.size()==0)
  {
    ui->debug_info->setTextColor(Qt::red);
    ui->debug_info->append("\nError: GroundTruth file has no available data!\n");
    ui->debug_info->setTextColor(Qt::black);
    return 0;
  }
  else{
    for(int i=0;i<detections.size();i++)
    {
      float recall =0.0;
      float precise = 0.0;
      eval_frame(groundtruth[i], detections[i], recall, precise, I_DIV_DET,THRESHOLD_IOU);
      if(DEBUG)
        ui->debug_info->append(QString("   frame_id: %1").arg(i)+QString(" recall: %1").arg(recall,0,'f',3)+QString("precise: %1").arg(precise,0,'f',3));
      *recall_tol +=recall;
      *precise_tol+=precise;
    }
    *recall_tol/=detections.size();
    *precise_tol/=detections.size();
    return 1;
  }

}





#include <iostream>
#include <string>

#ifdef linux
#include <unistd.h>
#include <pwd.h>
#endif

#ifdef _WIN32
#include<Windows.h>
#endif

std::string getUserName()
{
#if defined linux   //linux system
  uid_t userid;
  struct passwd* pwd;
  userid=getuid();
  pwd=getpwuid(userid);
  return pwd->pw_name;

#elif defined _WIN32  //windows system
  const int MAX_LEN = 100;
    char szBuffer[MAX_LEN];
    DWORD len = MAX_LEN;
    if( GetUserName(szBuffer, &len) )     //用户名保存在szBuffer中,len是用户名的长度
        return szBuffer;

#else  //outher system
    return "";
#endif
}

bool check_path(string FILENAME)
{
  fstream _file;
  _file.open(FILENAME.c_str(),ios::in);
  if(!_file)
  {
    return false;
  }
  else
  {
    return true;
  }

}

QString getTime()
{
  time_t timep;
  time (&timep);
  char tmp[64];
  strftime(tmp, sizeof(tmp), "[%H:%M:%S] ",localtime(&timep) );

  return QString::fromStdString(tmp);
}

