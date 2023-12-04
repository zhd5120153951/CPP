#include "segement.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp> 
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;
int main_manshui()
{
	Mat src = imread("./C++常用操作/图像分割/Fruits.jpg");
	imshow("img", src);
	Rect ccomp;
	floodFill(src, Point(50, 300), Scalar(155, 255, 55), &ccomp, Scalar(20, 20, 20), Scalar(20, 20, 20));
	imshow("ret", src);
	waitKey(0);
	return 0;
}
