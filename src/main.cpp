#include <QApplication>
#include <ros/ros.h>
#include "../ui/mainwindow.h"
int main(int argc, char *argv[])
{
    ros::init(argc, argv, "control");
    ros::NodeHandle node;
    ros::NodeHandle private_nh("~");

    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Evaluation of BBox v0.1.0");
    QIcon icon("../data/eval_icon.ico");
    w.setWindowIcon(icon);
    w.show();

    return a.exec();
}