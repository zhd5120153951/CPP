#include <opencv2/opencv.hpp>
#include "motion.h"
using namespace std;
using namespace cv;

int main_motion1()
{
	//读入视频  
	VideoCapture capture("./C++常用操作/运动目标检测/Laboratory_raw.avi");  
	//VideoCapture capture(0);
 
	Mat tempframe, currentframe, previousframe,difframe;
	Mat frame;
	int framenum = 0;
	//读取一帧处理  
	while (true)
	{
		if (!capture.isOpened())
		{
			cout << "read video failure" << endl;
			return -1;
		}
		//tempframe = capture.read(frame);  


		capture >> frame;
		tempframe = frame;
		framenum++;
		
		if (framenum >= 2)//第一帧要么不做差分,要么和全0图做差分
		{
			cvtColor(tempframe, currentframe, COLOR_BGR2GRAY);//转化为单通道灰度图，此时currentFrame已经存了tempFrame的内容
			//差分前可以去噪
			absdiff(currentframe, previousframe, currentframe);//做差求绝对值    
			threshold(currentframe, currentframe, 20, 255.0, THRESH_BINARY);
			//腐蚀膨胀可以大一点
			dilate(currentframe, currentframe, Mat());//膨胀  
			erode(currentframe, currentframe, Mat());//腐蚀  

			//显示图像    
			imshow("camera", tempframe);
			imshow("moving area", currentframe);

		}
		//把当前帧保存作为下一次处理的前一帧    
		cvtColor(tempframe, previousframe, COLOR_BGR2GRAY);  
		waitKey(33);
	}  
}
