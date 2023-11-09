#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "mask.h"

//�ڸǲ�����Ȥ������
int main_mask()
{

	cv::Mat src = cv::imread("./C++���ò���/mask/1.jpg");
	if (src.empty())
	{
		std::cout << "src is empty..." << std::endl;
		return -1;
	}
	cv::Mat src_copy = src.clone();//�����Ƽ�
	//����
	/*cv::Mat src_copy;
	src.copyTo(src_copy);*/

	cv::Rect targetRect(50, 50, 150, 150);//����
	//����һ��������һ����Mat,ͨ��,λ��һ����ȫ0ͼ
	cv::Mat allZeroZone(targetRect.height, targetRect.width, CV_8UC3, cv::Scalar(0));
	
	cv::Mat dst = src_copy(targetRect);//��ȡ����--����û��clone()--ǳ����
	allZeroZone.copyTo(dst);//�Ѹ����򸲸�(��ȫ0ͼ)

	cv::imshow("src", src);
	cv::imshow("dst", src_copy);
	cv::imshow("region", dst);
	cv::waitKey(0);
	return 0;
}
//�ڸǲ�����Ȥ������--�����
int main_mask_polyfill()
{
	cv::Mat srcImg = cv::imread("./C++���ò���/mask/2.jpg");
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

	const cv::Point* ppt[1]{ root_points[0] };//����ָ��--��ά
	int npt[] = { 6 };

	cv::Mat dstImg = srcImg.clone();

	polylines(dstImg, ppt, npt, 1, 1, cv::Scalar(0, 0, 0), 1, 8,0);
	fillPoly(dstImg, ppt, npt, 1, cv::Scalar(0, 0, 0));

	imshow("srcImg", srcImg);
	imshow("dstImg", dstImg);
	cv::waitKey(0);

	return 0;
}

//��������Ȥ������
int main_mask2()
{
	// ��ȡͼ��
	cv::Mat image = cv::imread("./C++���ò���/mask/2.jpg");

	if (image.empty()) 
	{
		std::cerr << "Error loading image!" << std::endl;
		return -1;
	}

	// �������룬���̶�������Ϊ255����ɫ��������������Ϊ0����ɫ��
	cv::Mat mask = cv::Mat::zeros(image.size(), CV_8U);
	cv::Rect roi(100, 100, 300, 400);  // ����̶�����ľ���
	mask(roi) = 255;

	// ��ͼ����������а�λ�����
	cv::Mat result;
	image.copyTo(result, mask);

	// ��ʾ���
	cv::imshow("Original Image", image);
	cv::imshow("Masked Image", result);
	cv::waitKey(0);

	return 0;
}
//��������Ȥ������--�����
int main_mask2_polyfill()
{
	// ��ȡͼ��
	cv::Mat image = cv::imread("./C++���ò���/mask/2.jpg");

	if (image.empty())
	{
		std::cerr << "Error loading image!" << std::endl;
		return -1;
	}

	// �������룬���̶�������Ϊ255����ɫ��������������Ϊ0����ɫ��
	//��ȡԭͼ��С��ȫ0����
	cv::Mat mask = cv::Mat::zeros(image.size(), CV_8U);
	//���������
	cv::Point root_points[1][6];
	root_points[0][0] = cv::Point(100, 130);
	root_points[0][1] = cv::Point(200, 150);
	root_points[0][2] = cv::Point(300, 200);
	root_points[0][3] = cv::Point(250, 300);
	root_points[0][4] = cv::Point(100, 300);
	root_points[0][5] = cv::Point(0, 200);
	
	const cv::Point* ppt[1]{ root_points[0] };//����ָ��
	int pts[] = { 6 };

	cv::polylines(mask, ppt, pts, 1, 1, cv::Scalar(255, 255, 255), 1, 8, 0);//����
	cv::fillPoly(mask, ppt, pts, 1, cv::Scalar(255, 255, 255));//���

	// ��ͼ����������а�λ�����
	cv::Mat result;
	image.copyTo(result, mask);

	// ��ʾ���
	cv::imshow("Original Image", image);
	cv::imshow("Masked Image", result);
	cv::waitKey(0);

	return 0;
}
