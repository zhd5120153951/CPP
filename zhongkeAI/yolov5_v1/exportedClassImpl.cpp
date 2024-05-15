#include "pch.h"
#include "GreatechBuilder.h"
#include "GreatechDetector.h"
#include "GreatechDetectionClasses.h"
using namespace yolov5;
extern "C"
{
	CLASS_DECLSPEC GreatechBuilder* createBuilder()
	{
		return new GreatechBuilder();
	}
	CLASS_DECLSPEC void destroyBuilder(GreatechBuilder* ptr)
	{
		if (!ptr)
		{
			std::cout << "Builder对象指针为空,无法删除." << std::endl;
			return;
		}
		delete ptr;
	}

	CLASS_DECLSPEC GreatechDetector* createDetector()
	{
		return new GreatechDetector();
	}
	CLASS_DECLSPEC void destroyDetector(GreatechDetector* ptr)
	{
		if (!ptr)
		{
			std::cout << "Detector对象指针为空,无法删除." << std::endl;
			return;
		}
		delete ptr;
	}
	CLASS_DECLSPEC GreatechDetection* createDetection()
	{
		return new GreatechDetection();
	}
	CLASS_DECLSPEC void destroyDetection(GreatechDetection* ptr)
	{
		if (!ptr)
		{
			std::cout << "Detection对象指针为空,无法删除." << std::endl;
			return;
		}
		delete ptr;
	}
	CLASS_DECLSPEC GreatechClasses* createClasses()
	{
		return new GreatechClasses();
	}
	CLASS_DECLSPEC void destroyClasses(GreatechClasses* ptr)
	{
		if (!ptr)
		{
			std::cout << "Classes对象指针为空,无法删除." << std::endl;
			return;
		}
		delete ptr;
	}
}