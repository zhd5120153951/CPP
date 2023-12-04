#include <iostream>
#include "C++常用操作/运动目标检测/motion.h"
#include "C++常用操作/图片拼接/pinjie.h"
#include "C++常用操作/mask/mask.h"
#include "C++常用操作/布匹瑕疵检测小思路/xiaci.h"
#include "C++常用操作/抠图案例/koutu.h"
#include "C++常用操作/图像分割/segement.h"
#include "C++常用操作/图像增强/Argument.h"
#include "C++常用操作/人脸识别案例/face.h"
#include "C++常用操作/图像边缘-角点-轮廓检测/edge_corner_outline.h"
#include "C++常用操作/颜色直方图比较/Hist.h"
#include "C++常用操作/三帧帧差法/diff_frame.h"


int main(int argc, char* argv[])
{
	//main_motion1();
	//main_pinjie();
	//main_mask();
	//main_mask_polyfill();
	//main_mask2();
	//main_mask2_polyfill();
	//main_xiaci(argc, argv);//指针数组--二维数组
	main_koutu(argc, argv);


	return 0;
}