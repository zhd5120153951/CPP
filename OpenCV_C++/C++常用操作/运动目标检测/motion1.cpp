#include <opencv2/opencv.hpp>
#include "motion.h"
using namespace std;
using namespace cv;

int main_motion1()
{
	//������Ƶ  
	VideoCapture capture("./C++���ò���/�˶�Ŀ����/Laboratory_raw.avi");  
	//VideoCapture capture(0);
 
	Mat tempframe, currentframe, previousframe,difframe;
	Mat frame;
	int framenum = 0;
	//��ȡһ֡����  
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
		
		if (framenum >= 2)//��һ֡Ҫô�������,Ҫô��ȫ0ͼ�����
		{
			cvtColor(tempframe, currentframe, COLOR_BGR2GRAY);//ת��Ϊ��ͨ���Ҷ�ͼ����ʱcurrentFrame�Ѿ�����tempFrame������
			//���ǰ����ȥ��
			absdiff(currentframe, previousframe, currentframe);//���������ֵ    
			threshold(currentframe, currentframe, 20, 255.0, THRESH_BINARY);
			//��ʴ���Ϳ��Դ�һ��
			dilate(currentframe, currentframe, Mat());//����  
			erode(currentframe, currentframe, Mat());//��ʴ  

			//��ʾͼ��    
			imshow("camera", tempframe);
			imshow("moving area", currentframe);

		}
		//�ѵ�ǰ֡������Ϊ��һ�δ����ǰһ֡    
		cvtColor(tempframe, previousframe, COLOR_BGR2GRAY);  
		waitKey(33);
	}  
}
