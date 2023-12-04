#include "segement.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp> 
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "segement.h"
using namespace std;
using namespace cv;

void MyShowImage(Mat Image, const string winName);
void getBinMask_Grabcut(const Mat& comMask, Mat& binMask);

void MyShowImage(Mat Image, const string winName)
{
	imshow(winName, Image);
}
void getBinMask_Grabcut(const Mat& comMask, Mat& binMask)
{
	binMask.create(comMask.size(), CV_8UC1);
	binMask = comMask & 1;
}

int main_Grabcut(int argc, char** argv)
{
	Mat image = imread("./C++常用操作/图像分割/lena.jpg", 1);

	const string winName = "image";
	MyShowImage(image, winName);

	Mat bg; Mat fg;
	Rect rect = Rect(47, 48, 408, 464);
	Mat mask, res;
	mask.create(image.size(), CV_8UC1);
	grabCut(image, mask, rect, bg, fg, 1, 0);

	Mat binMask;
	getBinMask_Grabcut(mask, binMask);
	image.copyTo(res, binMask);
	MyShowImage(res, winName);
	waitKey(0);
	return 0;
}
