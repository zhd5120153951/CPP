#include "pch.h"
#include "yoloTask.h"
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <atomic>
#include <algorithm>
#include <functional>
#include <future>
#include <filesystem>
#include <regex>

yoloTask::yoloTask()
{
	ret = 0;
	shouldExit = false;
	buildEngieResult = RESULT_SUCCESS;
}

yoloTask::~yoloTask()
{
	//析构函数中保证安全的关闭线程
	if (calculationThread.joinable())
	{
		calculationThread.join();
	}
}

Result yoloTask::buildEngine(const std::string inputFilePath,const std::string outputFilePath)
{
	//创建线程对象，一般用于后台执行默认任务,实际是哪里需要开辟多线程就放在哪里
	calculationThread = std::thread([this,inputFilePath,outputFilePath]() {	
		const std::string _in = inputFilePath;
		const std::string _out = outputFilePath;

		buildEngieResult = this->builder.init();//初始化
		buildEngieResult = this->builder.buildEngine(_in, _out);
		//可以添加其他运算任务
		this->shouldExit = true;
		});
	calculationThread.detach();
	return buildEngieResult;
}
//这里的需求要和廖健对接，他那里怎么使用，需要什么功能的接口
yolov5::Result yoloTask::detect(const int& gpuIndex,const std::string enginePath)
{
	std::thread([this](const int& gpuIndex,const std::string enginePath) {
		buildEngieResult = this->detector.init();
		cudaSetDevice(gpuIndex);

		//// C++17标准库中的std::filesystem进行文件系统操作--C++17之前没有
		std::filesystem::path path(enginePath);
		if (!std::filesystem::exists(path)) 
		{
			std::cout << "模型文件夹不存在！" << std::endl;
			return RESULT_FAILURE_OTHER;
		}

		// engine后缀的正则表达式
		std::regex engineRegex(".+\\.engine$");

		// 筛选出的engine文件路径
		//std::vector<std::filesystem::path> engineFiles;
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			std::string filename = entry.path().filename().string();
			if (std::regex_search(filename, engineRegex))
			{
				buildEngieResult = this->detector.loadEngine(filename);
			}
		}
		//buildEngieResult = this->detector.loadEngine(enginePath);
		
		//开始推理
		std::vector<Detection> detections;
		cv::Mat frame;
		cv::VideoCapture cap = cv::VideoCapture(0);
		if (!cap.isOpened())
		{
			std::cout << "打开摄像头失败...." << std::endl;
			return RESULT_FAILURE_OTHER;
		}
		while (true)
		{
			cap.read(frame);
			if (frame.empty())continue;
			this->detector.detect(frame, &detections);
			for (int i=0;i<detections.size();i++)
			{
				if (detections[i].classId()==0&&detections[i].score()>=0.5)
				{
					std::cout << "Person..." << std::endl;
					//cv::imwrite("/save"+)
				}
			}
		}
		shouldExit = true;
		}, gpuIndex, enginePath).detach();
		return buildEngieResult;
}

double yoloTask::add(double a, double b)
{
	//多线程进行加法处理
	//std::thread::id addID = std::this_thread::get_id();
	//std::cout << "add thread id:" << addID << std::endl;
	compute([this, a, b]() {
		ret = a + b;
		std::cout << "a+b=" << ret << std::endl;
		});
	return ret;
}

double yoloTask::subtract(double a, double b)
{
	compute([this, a, b]() {
		ret = a - b;
		std::cout << "a-b=" << ret << std::endl;
		});
	return ret;
}

double yoloTask::multiply(double a, double b)
{
	compute([this, a, b]() {
		ret = a * b;
		std::cout << "a*b=" << ret << std::endl;
		});
	return ret;
}

double yoloTask::divide(double a, double b)
{
	compute([this, a, b]() {
		if (b == 0) {
			std::cerr << "Error! Division by Zero." << std::endl;
		}
		ret = a / b;
		std::cout << "a/b=" << ret << std::endl;
		});
	return ret;
}

void yoloTask::compute(std::function<void()>task)
{
	//实际计算调用过程，可以在这里实现多线程计算
	task();
}

