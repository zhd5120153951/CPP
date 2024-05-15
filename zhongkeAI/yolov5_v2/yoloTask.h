#pragma once
#include "yolov5_builder.hpp"
#include "yolov5_detector.hpp"
#include "yolov5_detection.hpp"
#define CLASS_DECLSPEC __declspec(dllexport)
namespace yolov5
{
	class Builder;
	class Detector;
	class Detection;
	class Classes;
	class CLASS_DECLSPEC yoloTask
	{
	public:
		yoloTask();
		~yoloTask();

		Result detectTask(Detector& detector);
		Result detector_task();
	public:
		//Builder builder;
		Detector detector;
	};

}