#include "diff_frame.h"
#include <opencv2/opencv.hpp>  

using namespace cv;
using namespace std;

int main_3_video()
{
	VideoCapture capture("./C++常用操作/三帧帧差法/Laboratory_raw.avi");  
	//VideoCapture capture(0);
 
	Mat tempframe, currentframe, previousframe,difframe;
	Mat s3,difframe2,difframe3,tempframe2;
	Mat frame;
	int framenum = 0;
	while (true)
	{
		if (!capture.isOpened())
		{
			cout << "read video failure" << endl;
			return -1;
		}
		//tempframe = capture.read(frame);  
		capture >> frame;
		imshow("frame", frame);
		previousframe = frame.clone();

		capture >> frame;
		currentframe = frame.clone();

		cvtColor(previousframe, previousframe, COLOR_BGR2GRAY);
		cvtColor(currentframe, currentframe, COLOR_BGR2GRAY);
		absdiff(currentframe, previousframe, difframe);
		threshold(difframe, tempframe, 20, 255.0, THRESH_BINARY);
		dilate(tempframe, tempframe, Mat());
		erode(tempframe, tempframe, Mat());

		capture >> frame;
		s3 = frame.clone();
		cvtColor(s3, s3, COLOR_BGR2GRAY);
		absdiff(previousframe, s3, difframe2);
		bitwise_and(difframe, difframe2, difframe3);
		threshold(difframe3, tempframe2, 20, 255.0,THRESH_BINARY);
		dilate(tempframe2, tempframe2, Mat()); 
		erode(tempframe2, tempframe2, Mat());

		imshow("1-2帧差", tempframe);

		imshow("1-2-3帧差", tempframe2);
 
		waitKey(50);
	}//end while    
}
