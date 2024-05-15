#include "pch.h"
#include "yolov5_builder.hpp"
#include "yolov5_detector.hpp"
#include "yolov5_detection.hpp"
#include "yoloTask.h"
using namespace yolov5;
extern "C"
{
	CLASS_DECLSPEC yoloTask* createYoloTask()
	{
		return new yoloTask();
	}
	CLASS_DECLSPEC void destroyYoloTask(yoloTask* ptr)
	{
		if (!ptr)
		{
			std::cout << "YoloTask对象指针为空,无法删除." << std::endl;
			return;
		}
		delete ptr;
	}

	CLASS_DECLSPEC Builder* createBuilder()
	{
		return new Builder();
	}
	CLASS_DECLSPEC void destroyBuilder(Builder* ptr)
	{
		if (!ptr)
		{
			std::cout << "Builder对象指针为空,无法删除." << std::endl;
			return;
		}
		delete ptr;
	}

	CLASS_DECLSPEC Detector* createDetector()
	{
		return new Detector();
	}
	CLASS_DECLSPEC void destroyDetector(Detector* ptr)
	{
		if (!ptr)
		{
			std::cout << "Detector对象指针为空,无法删除." << std::endl;
			return;
		}
		delete ptr;
	}
	CLASS_DECLSPEC Detection* createDetection()
	{
		return new Detection();
	}
	CLASS_DECLSPEC void destroyDetection(Detection* ptr)
	{
		if (!ptr)
		{
			std::cout << "Detection对象指针为空,无法删除." << std::endl;
			return;
		}
		delete ptr;
	}
	CLASS_DECLSPEC Classes* createClasses()
	{
		return new Classes();
	}
	CLASS_DECLSPEC void destroyClasses(Classes* ptr)
	{
		if (!ptr)
		{
			std::cout << "Classes对象指针为空,无法删除." << std::endl;
			return;
		}
		delete ptr;
	}
}