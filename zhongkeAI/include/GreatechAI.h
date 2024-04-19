#pragma once
#include "yolov5_builder.hpp"
#include "yolov5_detector.hpp"
using namespace yolov5;

#define CLASS_DECLSPEC __declspec(dllexport)//��ʾ���ർ��

class CLASS_DECLSPEC GreatechAI
{
public:
	GreatechAI();
	~GreatechAI();

	void builder_init();
	void builder_buildEngine(const std::string& inputFilePath,const std::string& outputFilePath);
public:
	Builder builder;
	Detector detector;
};


