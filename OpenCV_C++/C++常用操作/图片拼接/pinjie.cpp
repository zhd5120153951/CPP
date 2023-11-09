#include <iostream>
#include<opencv2\opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/stitching.hpp>
#include "pinjie.h"

using namespace std;
using namespace cv;
bool try_use_gpu = false;//是否使用GPU，否
vector<Mat> imgs;
vector<Mat> imgss;
int jiequ_shang = 30;//截取高度上
int jiequ_xia = 60;//截取高度下
int main_pinjie()
{
    Mat img1 = imread("./C++常用操作/图片拼接/pinjie1.jpg");
    Mat img2 = imread("./C++常用操作/图片拼接/pinjie2.jpg");
	Mat img3 = imread("./C++常用操作/图片拼接/pinjie3.jpg");

	
    imshow("p1", img1); 
    imshow("p2", img2);
	imshow("p3", img3);

    imgs.push_back(img1);
    imgs.push_back(img2);

    //Stitcher stitcher = Stitcher::createDefault(try_use_gpu);//旧版opencv
	Ptr<Stitcher> stitcher = Stitcher::create();


    Mat tmp; 
	//stitcher.stitch();//旧版opencv
    stitcher->stitch(imgs, tmp);

	imgss.push_back(tmp);
    imgss.push_back(img3);
	Mat dst;
	//stitcher.stitch();//旧版opencv
	stitcher->stitch(imgss, dst);
	imshow("结果", dst);
	Rect rect(0, jiequ_shang, dst.cols, dst.rows-jiequ_xia);
	Mat image_roi = dst(rect);
	imshow("截取后", image_roi);
		
	waitKey(0);
    return 0;
}
