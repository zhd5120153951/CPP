#include "koutu.h"
#include<opencv2\opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
Rect rect_koutu;
Mat src_koutu, roiImg_koutu, result_koutu;

void onMouse_koutu(int event, int x, int y, int flags, void* userdata);
void showImg_koutu();

void getBinMask_koutu(const Mat& comMask, Mat& binMask)
{
	binMask.create(comMask.size(), CV_8UC1);
	binMask = comMask & 1;
}
int main_koutu(int arc, char** argv)
{ 
	src_koutu = imread("./C++常用操作/抠图案例/test2.jpg");
	namedWindow("input", WINDOW_AUTOSIZE);
	imshow("input", src_koutu); 
	setMouseCallback("input", onMouse_koutu);		
	Mat result = Mat::zeros(src_koutu.size(), CV_8UC1);
	Mat bgModel, fgModel;
	char c = waitKey(0);
	if (c == 'g') 
	{
		Mat image = imread("./C++常用操作/抠图案例/test2.jpg", 1);
		Mat bg; Mat fg;
		Mat mask, res;
		mask.create(image.size(), CV_8UC1);
		grabCut(image, mask, rect_koutu, bg, fg,2, 0);
		Mat binMask;
		getBinMask_koutu(mask, binMask);
		image.copyTo(res, binMask);
		Mat res_clone = res.clone();
		Mat background;
		absdiff(image,res_clone, background);
		imshow("dst", background);
		Mat imageGray;
		//转换为灰度图
		cvtColor(background, imageGray, COLOR_RGB2GRAY, 0);
		Mat imageMask = Mat(background.size(), CV_8UC1, Scalar::all(0));
		//通过阈值处理生成Mask
		threshold(imageGray, imageMask, 1, 255, THRESH_BINARY_INV);//240-1 CV_THRESH_BINARY-THRESH_BINARY_INV
		Mat Kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
		//对Mask膨胀处理，增加Mask面积
		dilate(imageMask, imageMask, Kernel);
		//图像修复
		inpaint(background, imageMask, background, 50, INPAINT_TELEA);//5-50
		imshow("Mask", imageMask);
		imshow("修复后", background);
		Mat new_dst;
		bilateralFilter(background,new_dst,15,150,3);

		//namedWindow("bilateralFilter", WINDOW_AUTOSIZE);
		//imshow("bilateralFilter", dst);

		Mat  result;
		Mat kernal = (Mat_<int>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
		filter2D(new_dst, result, -1, kernal,Point(-1,-1),0);
		imshow("图像增强后", result);
	}
	waitKey(0); 
	return 0;
}
void showImg_koutu()
{
	src_koutu.copyTo(roiImg_koutu);
	rectangle(roiImg_koutu, rect_koutu, Scalar(0, 0, 255), 2);
	imshow("input", roiImg_koutu);
}
//鼠标选择矩形框
void onMouse_koutu(int event, int x, int y, int flags, void* userdata)
{
	switch (event)
	{
	case EVENT_LBUTTONDOWN://鼠标左键按下事件
		rect_koutu.x = x;
		rect_koutu.y = y;
		rect_koutu.width = 1;
		rect_koutu.height = 1;
		break;
	case EVENT_MOUSEMOVE://鼠标移动事件
		if (flags && EVENT_FLAG_LBUTTON)
		{
			rect_koutu = Rect(Point(rect_koutu.x, rect_koutu.y), Point(x, y));
			showImg_koutu();
		}
		break;
	case EVENT_LBUTTONUP://鼠标弹起事件
		if (rect_koutu.width > 1 && rect_koutu.height > 1)
		{
			showImg_koutu();
		}
		break;
	default:
		break;
	}
}