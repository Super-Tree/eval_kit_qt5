#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <stdio.h>
#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int32_t bbox_eval(std::string gt_path,std::string det_path,float *recall_tol,float *precise_tol,bool DEBUG,float THRESHOLD_IOU);
private slots:

    void on_gt_file_button_clicked();

    void on_det_file_button_clicked();

    void on_start_button_clicked();

    void on_gt_path_editingFinished();

    void on_det_path_editingFinished();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
