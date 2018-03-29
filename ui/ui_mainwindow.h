/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <cstddef>
QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *gt_file_button;
    QPushButton *det_file_button;
    QPushButton *start_button;
    QComboBox *metric_method;
    QTextBrowser *debug_info;
    QLineEdit *gt_path;
    QLineEdit *det_path;
    QDoubleSpinBox *value_iou;
    QLabel *label;
    QCheckBox *checkBox;
    QLabel *label_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(497, 349);
        MainWindow->setFocusPolicy(Qt::ClickFocus);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gt_file_button = new QPushButton(centralWidget);
        gt_file_button->setObjectName(QStringLiteral("gt_file_button"));
        gt_file_button->setGeometry(QRect(10, 10, 89, 31));
        det_file_button = new QPushButton(centralWidget);
        det_file_button->setObjectName(QStringLiteral("det_file_button"));
        det_file_button->setGeometry(QRect(10, 50, 89, 31));
        start_button = new QPushButton(centralWidget);
        start_button->setObjectName(QStringLiteral("start_button"));
        start_button->setGeometry(QRect(390, 90, 89, 31));
        metric_method = new QComboBox(centralWidget);
        metric_method->addItem(QString());
        metric_method->addItem(QString());
        metric_method->setObjectName(QStringLiteral("metric_method"));
        metric_method->setGeometry(QRect(120, 90, 101, 31));
        debug_info = new QTextBrowser(centralWidget);
        debug_info->setObjectName(QStringLiteral("debug_info"));
        debug_info->setGeometry(QRect(10, 130, 471, 151));
        gt_path = new QLineEdit(centralWidget);
        gt_path->setObjectName(QStringLiteral("gt_path"));
        gt_path->setGeometry(QRect(120, 10, 361, 31));
        gt_path->setFocusPolicy(Qt::ClickFocus);
        gt_path->setInputMethodHints(Qt::ImhHiddenText);
        gt_path->setCursorPosition(88);
        gt_path->setClearButtonEnabled(false);
        det_path = new QLineEdit(centralWidget);
        det_path->setObjectName(QStringLiteral("det_path"));
        det_path->setGeometry(QRect(120, 50, 361, 31));
        det_path->setFocusPolicy(Qt::StrongFocus);
        det_path->setCursorPosition(82);
        det_path->setClearButtonEnabled(false);
        value_iou = new QDoubleSpinBox(centralWidget);
        value_iou->setObjectName(QStringLiteral("value_iou"));
        value_iou->setGeometry(QRect(260, 90, 51, 31));
        value_iou->setMaximum(1);
        value_iou->setSingleStep(0.05);
        value_iou->setValue(0.7);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(230, 90, 31, 31));
        checkBox = new QCheckBox(centralWidget);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(320, 90, 91, 31));
        checkBox->setChecked(true);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 90, 61, 31));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 497, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        gt_file_button->setText(QApplication::translate("MainWindow", "gt_file", 0));
        det_file_button->setText(QApplication::translate("MainWindow", "det_file", 0));
        start_button->setText(QApplication::translate("MainWindow", "Start!", 0));
        metric_method->setItemText(0, QApplication::translate("MainWindow", "bv_eval", 0));
        metric_method->setItemText(1, QApplication::translate("MainWindow", "pixel_eval", 0));

#ifndef QT_NO_TOOLTIP
        gt_path->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-style:italic; color:#babdb6;\">gt_file</span></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        gt_path->setText(QApplication::translate("MainWindow", "/home/hexindong/Desktop/KITTI_Evaluation/detection_evaluation_tool/data/label/result.txt", 0));
#ifndef QT_NO_TOOLTIP
        det_path->setToolTip(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-style:italic; color:#babdb6;\">det_file</span></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_WHATSTHIS
        det_path->setWhatsThis(QApplication::translate("MainWindow", "<html><head/><body><p>det</p></body></html>", 0));
#endif // QT_NO_WHATSTHIS
        det_path->setText(QApplication::translate("MainWindow", "/home/hexindong/Desktop/KITTI_Evaluation/detection_evaluation_tool/data/detections", 0));
        label->setText(QApplication::translate("MainWindow", "IOU:", 0));
        checkBox->setText(QApplication::translate("MainWindow", "DEBUG", 0));
        label_2->setText(QApplication::translate("MainWindow", "METRIC:", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
