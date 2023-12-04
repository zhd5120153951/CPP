#include "Argument.h" 
#include <opencv2/highgui/highgui.hpp>      
#include <opencv2/imgproc/imgproc.hpp>  
#include <iostream>
using namespace cv;
using namespace std;
int main_hist(int argc, char *argv[])  
{  
    Mat image = imread("./C++常用操作/图像增强/Test.jpg", 1);  
    if (image.empty())  
    {  
        std::cout << "读图失败" << std::endl;  
        return -1;  
    }  
    imshow("img", image);  
    Mat imageRGB[3];  
    split(image, imageRGB);  
    for (int i = 0; i < 3; i++)  
    {  
        equalizeHist(imageRGB[i], imageRGB[i]);  
    }
    merge(imageRGB, 3, image);  
    imshow("ֱret", image);  
    waitKey(0);  
    return 0;  
} 