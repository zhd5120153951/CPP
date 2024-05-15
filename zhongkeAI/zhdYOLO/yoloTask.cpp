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
	//���������б�֤��ȫ�Ĺر��߳�
	if (calculationThread.joinable())
	{
		calculationThread.join();
	}
}

Result yoloTask::buildEngine(const std::string inputFilePath,const std::string outputFilePath)
{
	//�����̶߳���һ�����ں�ִ̨��Ĭ������,ʵ����������Ҫ���ٶ��߳̾ͷ�������
	calculationThread = std::thread([this,inputFilePath,outputFilePath]() {	
		const std::string _in = inputFilePath;
		const std::string _out = outputFilePath;

		buildEngieResult = this->builder.init();//��ʼ��
		buildEngieResult = this->builder.buildEngine(_in, _out);
		//�������������������
		this->shouldExit = true;
		});
	calculationThread.detach();
	return buildEngieResult;
}
//���������Ҫ���ν��Խӣ���������ôʹ�ã���Ҫʲô���ܵĽӿ�
yolov5::Result yoloTask::detect(const int& gpuIndex,const std::string enginePath)
{
	std::thread([this](const int& gpuIndex,const std::string enginePath) {
		buildEngieResult = this->detector.init();
		cudaSetDevice(gpuIndex);

		//// C++17��׼���е�std::filesystem�����ļ�ϵͳ����--C++17֮ǰû��
		std::filesystem::path path(enginePath);
		if (!std::filesystem::exists(path)) 
		{
			std::cout << "ģ���ļ��в����ڣ�" << std::endl;
			return RESULT_FAILURE_OTHER;
		}

		// engine��׺��������ʽ
		std::regex engineRegex(".+\\.engine$");

		// ɸѡ����engine�ļ�·��
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
		
		//��ʼ����
		std::vector<Detection> detections;
		cv::Mat frame;
		cv::VideoCapture cap = cv::VideoCapture(0);
		if (!cap.isOpened())
		{
			std::cout << "������ͷʧ��...." << std::endl;
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
	//���߳̽��мӷ�����
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
	//ʵ�ʼ�����ù��̣�����������ʵ�ֶ��̼߳���
	task();
}

