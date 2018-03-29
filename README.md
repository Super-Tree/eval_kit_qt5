README
======
2018-3-29 hxd

将评估算法封装在了基于qt5的UI程序中,靠catkin_make组织的工程,依赖问题见CmakeLists.txt

使用方法:

点击gt_file 按钮选择 gt文件,点击det_file按钮选择存储检测结果的文件夹,
设置好IOU,metric,debug后点击Start即可

![run_pic](https://github.com/Super-Tree/eval_kit_qt5/blob/master/data/qt5_ui.png)

2018-3-16 hxd

该程序用来评估检测结果,产生recall和precision
****

## 存储方式
将整个路段的检测文件放置于同一文件夹下,文件名以此格式"xxxxxx.txt"递增,例如:000000.txt,000001.txt
[注意]:检测文件要和pcd文件保持一一对应,可以不检测完所有pcd,但必须从第一帧开始检测且中间不能有间隔漏帧

## 数据格式
label x y z l w h phi
例如
smallMot 30.042467 -7.288148 -0.019265 2.669403 1.572729 1.081375 -0.060933
bigMot -13.637826 3.968581 -0.884042 4.200720 1.609454 1.381249 0.000000
phi为Z轴逆时针旋转弧度,正前方(x-axis)为0.00  范围(-π ~ +π)

## 使用方法
设置main函数中gt_path(打标文件夹路径)和det_path(检测文件夹)
详见evaluate_object_bv.cpp main函数

运行如下图:

Start the evaluation(iou:0.70) on bv...
    frame 0,recall: 0.142857, precision: 1.000000
    frame 1,recall: 0.250000, precision: 1.000000
    frame 2,recall: 0.625000, precision: 1.000000
The result of 3 frames: recall: 0.339286, precision: 1.000000
