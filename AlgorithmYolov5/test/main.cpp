#include "opencv2/opencv.hpp"
#include "opencv2/dnn.hpp"
#include "opencv2/dnn/shape_utils.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "ffmpeg_demuxer.hpp"
#include "cuvid_decoder.hpp"
#include "nalu.hpp"

#include "yolov5_multask.h"

#include "Public_Function.h"
#include "ilogger.hpp"

#include <cstdlib>
#include <corecrt_io.h>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <fstream>
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;
using namespace cv;

queue<Mat> video_frames;
mutex mtx;
condition_variable c_v;

bool ready = false;           // 全局标志位
uint64_t m_createfd2, m_createfd3;


//存视频帧
void video_read(int64_t& param)
{
	std::string video_path = "fire3.mp4";
	cv::VideoCapture cap(video_path,CAP_FFMPEG);
	if (!cap.isOpened())
	{
		std::cerr << "Error opening video file" << std::endl;
		return;
	}

	cv::Mat frame;
	
	int seq = 1;
	while (true) 
	{
		cap >> frame;
		if (frame.empty()) 
		{
			break;
		}
		ready = false;
		Mat ret;
		vector<RecTrackResult> vecobjRec;
		if (yolov5_onnxtrt_multask_process(param, frame, ret, vecobjRec))
			imwrite("E:\\Github\\CPP\\AlgorithmYolov5\\release\\save\\" + to_string(seq) + ".jpg", ret);
		//std::unique_lock<std::mutex> lck(mtx);
		//video_frames.push(frame);
		//需要限制队列长度的
		//c_v.notify_one();
		seq++;
	}
	ready = true;
	std::cout << "结束了read" << endl;
}
//取视频帧
void display_video()
{
	//cv::namedWindow("Video", cv::WINDOW_AUTOSIZE);
	Mat ret;
	vector<RecTrackResult> vecobjRec;
	int seq = 1;
	while (true)
	{	
		if (ready)break;
		std::unique_lock<std::mutex> lck(mtx);
		c_v.wait(lck, [] {
			return !video_frames.empty();
			});

		cv::Mat frame = video_frames.front();
		video_frames.pop();
		
		if (yolov5_onnxtrt_multask_process(m_createfd2, frame, ret, vecobjRec))
			imwrite("E:\\Github\\CPP\\AlgorithmYolov5\\release\\save" + to_string(seq) + ".jpg", ret);
		//cv::imshow("Video", frame);
		//cv::waitKey(40); // 25ms delay between frames	
	}
	//cv::destroyAllWindows();
	std::cout << "结束了display" << endl;
}
int main()
{
	uint64_t m_createfd1, mcreatefd;

	//string fire_trtmodel_path = ExecuteDirAppend("model\\fire.engine");
	//string fire_name_file = ExecuteDirAppend("model\\fire.name");
	const char* fire_trtmodel_path = "E:\\Github\\CPP\\AlgorithmYolov5\\release\\model\\fire.engine";
	const char* fire_name_file = "E:\\Github\\CPP\\AlgorithmYolov5\\release\\model\\fire.names";

	/*const char* smoke_trtmodel_path = "E:\\Github\\CPP\\AlgorithmYolov5\\release\\model\\fire1.engine";
	const char* smoke_name_file = "E:\\Github\\CPP\\AlgorithmYolov5\\release\\model\\fire.names";*/

	const char* yolo_trtmodel_path = "E:\\Github\\CPP\\AlgorithmYolov5\\release\\model\\yolov5s.engine";
	const char* yolo_name_file = "E:\\Github\\CPP\\AlgorithmYolov5\\release\\model\\yolov5s.names";
	

	//加载模型
	m_createfd1 = yolov5_onnxtrt_multask_create(fire_trtmodel_path,fire_name_file, 0.5, 0.5, 0);
	cout << "m_createfd1:" << m_createfd1 << endl;
	mcreatefd = yolov5_onnxtrt_multask_create(yolo_trtmodel_path, yolo_name_file, 0.5, 0.5, 0);
	cout << "mcreatefd:" << mcreatefd << endl;

	/*m_createfd2 = yolov5_onnxtrt_multask_create(smoke_trtmodel_path,smoke_name_file,0.5, 0.5, 0);
	cout << "m_createfd2:" << m_createfd2 << endl;

	m_createfd3 = yolov5_onnxtrt_multask_create(yolo_trtmodel_path, yolo_name_file, 0.5, 0.5, 0);
	cout << "m_createfd3:" << m_createfd3 << endl;*/

	if (m_createfd1 > 0&& mcreatefd>0)
	{
		cout << "模型加载成功..." << endl;
	}
	//开启视频解码线程
	
	//thread read_thread(&video_read,m_createfd1);
	//thread display_thread(display_video);

	//read_thread.join();
	//display_thread.join();

	
	//std::unique_lock<std::mutex> lck(mtx);
	//video_frames.push(frame);
	//需要限制队列长度的
	//c_v.notify_one();
		
	std::string video_path = "fire3.mp4";
	cv::VideoCapture cap(video_path, CAP_FFMPEG);
	if (!cap.isOpened())
	{
		std::cerr << "Error opening video file" << std::endl;
		return 0;
	}

	cv::Mat frame;

	int seq = 1;
	while (true)
	{
		cap >> frame;
		if (frame.empty())
		{
			break;
		}
		ready = false;
		Mat ret1, ret2;
		vector<RecTrackResult> vecobjRec1, vecobjRec2;
		if (yolov5_onnxtrt_multask_process(m_createfd1, frame, ret1, vecobjRec1))
			imwrite("E:\\Github\\CPP\\AlgorithmYolov5\\release\\save\\" + to_string(seq) + "_ret1.jpg", ret1);
		if (yolov5_onnxtrt_multask_process(mcreatefd, frame, ret2, vecobjRec2))
			imwrite("E:\\Github\\CPP\\AlgorithmYolov5\\release\\save\\" + to_string(seq) + "_ret2.jpg", ret2);
		
		seq++;
	}

	bool desret = yolov5_onnxtrt_multask_destroy(m_createfd1);
	bool desret2 = yolov5_onnxtrt_multask_destroy(mcreatefd);
	cout << "desret:" << desret <<"desret2" <<desret2<< endl;
	return 0;
}