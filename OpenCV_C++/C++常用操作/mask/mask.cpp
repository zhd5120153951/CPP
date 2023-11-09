#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "mask.h"

//掩盖不感兴趣的区域
int main_mask()
{

	cv::Mat src = cv::imread("./C++常用操作/mask/1.jpg");
	if (src.empty())
	{
		std::cout << "src is empty..." << std::endl;
		return -1;
	}
	cv::Mat src_copy = src.clone();//法①推荐
	//法②
	/*cv::Mat src_copy;
	src.copyTo(src_copy);*/

	cv::Rect targetRect(50, 50, 150, 150);//区域
	//构造一个和区域一样的Mat,通道,位深一样的全0图
	cv::Mat allZeroZone(targetRect.height, targetRect.width, CV_8UC3, cv::Scalar(0));
	
	cv::Mat dst = src_copy(targetRect);//获取区域--这里没有clone()--浅拷贝
	allZeroZone.copyTo(dst);//把该区域覆盖(用全0图)

	cv::imshow("src", src);
	cv::imshow("dst", src_copy);
	cv::imshow("region", dst);
	cv::waitKey(0);
	return 0;
}
//掩盖不感兴趣的区域--多边形
int main_mask_polyfill()
{
	cv::Mat srcImg = cv::imread("./C++常用操作/mask/2.jpg");
	if (srcImg.empty())
	{
		std::cout << "Read Image Failed!" << std::endl;
		return -1;
	}
	
	cv::Point root_points[1][6];
	root_points[0][0] = cv::Point(100, 100);
	root_points[0][1] = cv::Point(200, 100);
	root_points[0][2] = cv::Point(300, 200);
	root_points[0][3] = cv::Point(200, 300);
	root_points[0][4] = cv::Point(100, 300);
	root_points[0][5] = cv::Point(0, 200);

	const cv::Point* ppt[1]{ root_points[0] };//数组指针--二维
	int npt[] = { 6 };

	cv::Mat dstImg = srcImg.clone();

	polylines(dstImg, ppt, npt, 1, 1, cv::Scalar(0, 0, 0), 1, 8,0);
	fillPoly(dstImg, ppt, npt, 1, cv::Scalar(0, 0, 0));

	imshow("srcImg", srcImg);
	imshow("dstImg", dstImg);
	cv::waitKey(0);

	return 0;
}

//保留感兴趣的区域
int main_mask2()
{
	// 读取图像
	cv::Mat image = cv::imread("./C++常用操作/mask/2.jpg");

	if (image.empty()) 
	{
		std::cerr << "Error loading image!" << std::endl;
		return -1;
	}

	// 创建掩码，将固定区域置为255（白色），其他区域置为0（黑色）
	cv::Mat mask = cv::Mat::zeros(image.size(), CV_8U);
	cv::Rect roi(100, 100, 300, 400);  // 定义固定区域的矩形
	mask(roi) = 255;

	// 将图像与掩码进行按位与操作
	cv::Mat result;
	image.copyTo(result, mask);

	// 显示结果
	cv::imshow("Original Image", image);
	cv::imshow("Masked Image", result);
	cv::waitKey(0);

	return 0;
}
//保留感兴趣的区域--多边形
int main_mask2_polyfill()
{
	// 读取图像
	cv::Mat image = cv::imread("./C++常用操作/mask/2.jpg");

	if (image.empty())
	{
		std::cerr << "Error loading image!" << std::endl;
		return -1;
	}

	// 创建掩码，将固定区域置为255（白色），其他区域置为0（黑色）
	//获取原图大小的全0掩码
	cv::Mat mask = cv::Mat::zeros(image.size(), CV_8U);
	//多边形区域
	cv::Point root_points[1][6];
	root_points[0][0] = cv::Point(100, 130);
	root_points[0][1] = cv::Point(200, 150);
	root_points[0][2] = cv::Point(300, 200);
	root_points[0][3] = cv::Point(250, 300);
	root_points[0][4] = cv::Point(100, 300);
	root_points[0][5] = cv::Point(0, 200);
	
	const cv::Point* ppt[1]{ root_points[0] };//数组指针
	int pts[] = { 6 };

	cv::polylines(mask, ppt, pts, 1, 1, cv::Scalar(255, 255, 255), 1, 8, 0);//画线
	cv::fillPoly(mask, ppt, pts, 1, cv::Scalar(255, 255, 255));//填充

	// 将图像与掩码进行按位与操作
	cv::Mat result;
	image.copyTo(result, mask);

	// 显示结果
	cv::imshow("Original Image", image);
	cv::imshow("Masked Image", result);
	cv::waitKey(0);

	return 0;
}
