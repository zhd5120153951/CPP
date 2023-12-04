#include "segement.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp> 
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;
int main_Meanshfit(int argc, char** argv)  
{  
    Mat img = imread("./C++常用操作/图像分割/lena.jpg");   
    imshow("img",img);  
    Mat res; 
    int spatialRad = 50;  
    int colorRad = 50;    
    int maxPyrLevel = 2;   
    pyrMeanShiftFiltering( img, res, spatialRad, colorRad, maxPyrLevel); 
    imshow("res",res);        
    RNG rng = theRNG();  
    Mat mask( res.rows+2, res.cols+2, CV_8UC1, Scalar::all(0) ); 
    for( int y = 0; y < res.rows; y++ )  
    {  
        for( int x = 0; x < res.cols; x++ )  
        {  
            if( mask.at<uchar>(y+1, x+1) == 0 ) 
            {  
                Scalar newVal( rng(256), rng(256), rng(256) );  
                floodFill( res, mask, Point(x,y), newVal, 0, Scalar::all(5), Scalar::all(5) );
            }  
        }  
    }  
    imshow("meanShift", res );  
    waitKey();  
    return 0;  
}
