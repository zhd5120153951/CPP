#pragma once
#include "yolov5_builder.hpp"
#include "yolov5_detector.hpp"
#include "yolov5_detection.hpp"
#include <cuda_runtime.h>
#include <thread>
#include <functional>
#define CLASS_DECLSPEC __declspec(dllexport)
using namespace yolov5;
class CLASS_DECLSPEC yoloTask
{
public:
	yoloTask();
	~yoloTask();

	Result buildEngine(const std::string inputFilePath, const std::string outputFilePath);
	Result detect(const int& gpuIndex,const std::string enginePath);
	//测试部分
	double add(double a, double b);
	double subtract(double a, double b);
	double multiply(double a, double b);
	double divide(double a, double b);
public:
	Builder builder;
	Detector detector;
	Result buildEngieResult;
	std::thread calculationThread;//线程对象
	std::atomic<bool> shouldExit;
private:
	void compute(std::function<void()>task);//导出类中执行运算的线程函数
	double ret;//存储运算结果
	
};


