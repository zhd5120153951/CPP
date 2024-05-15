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
		ptr->~yoloTask();
		/*if (!ptr)
		{
			std::cout << "YoloTask对象指针为空,无法删除." << std::endl;
			return;
		}*/
		delete ptr;
	}
}