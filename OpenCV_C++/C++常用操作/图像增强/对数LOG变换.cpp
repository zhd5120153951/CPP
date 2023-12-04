#include "Argument.h"
#include <opencv2/highgui/highgui.hpp>      
#include <opencv2/imgproc/imgproc.hpp>  
#include <iostream>
using namespace cv;
using namespace std;
int main_log(int argc, char *argv[])  
{  
    Mat image = imread("./C++常用操作/图像增强/Test.jpg");  
    Mat imageLog(image.size(), CV_32FC3);  
    for (int i = 0; i < image.rows; i++)  
    {  
        for (int j = 0; j < image.cols; j++)  
        {  
            imageLog.at<Vec3f>(i, j)[0] = log(float(1 + image.at<Vec3b>(i, j)[0]));  
            imageLog.at<Vec3f>(i, j)[1] = log(float(1 + image.at<Vec3b>(i, j)[1]));  
            imageLog.at<Vec3f>(i, j)[2] = log(float(1 + image.at<Vec3b>(i, j)[2]));  
        }  
    }  
    
    normalize(imageLog, imageLog, 0, 255, NORM_MINMAX);
    
    convertScaleAbs(imageLog, imageLog);  
    imshow("Soure", image);  
    imshow("log_ret", imageLog);  
    waitKey();    
    return 0;  
}