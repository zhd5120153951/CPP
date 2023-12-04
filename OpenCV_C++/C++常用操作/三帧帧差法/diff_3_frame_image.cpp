#include "diff_frame.h"
#include <opencv2/opencv.hpp>  

using namespace cv;
using namespace std;

int main_3_image()
{
	Mat src0,src1,src2,dst0,dst1,dst2,dst3,tempframe;
	src0=imread("00.bmp");
	src1=imread("01.bmp");
	src2=imread("02.bmp");

	cvtColor(src0,dst0,COLOR_BGR2GRAY);
	cvtColor(src1,dst1,COLOR_BGR2GRAY);
	cvtColor(src2,dst2,COLOR_BGR2GRAY);

	absdiff(dst0,dst1,dst1);
	absdiff(dst1,dst2,dst2);
	bitwise_and(dst1, dst2, dst3);

	threshold( dst3, tempframe, 20, 255, THRESH_BINARY );
	dilate(tempframe, tempframe, Mat());
	erode(tempframe, tempframe, Mat());

	imshow("帧差结果", tempframe);


	waitKey(0);   
    return 0;  
}
