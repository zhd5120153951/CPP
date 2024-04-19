#pragma once
#include "yolov5_builder.hpp"
#include "yolov5_detector.hpp"
using namespace yolov5;

#define CLASS_DECLSPEC __declspec(dllexport)//表示把类导出

class CLASS_DECLSPEC GreatechAI
{
public:
	GreatechAI();
	~GreatechAI();

	void builder_init();
	void builder_buildEngine(const std::string& inputFilePath,const std::string& outputFilePath);
	void builder_buildEngine(const std::string& inputFilePath, std::vector<char>* output);
	void builder_setLogger(std::shared_ptr<Logger> logger);
	std::shared_ptr<Logger> logger();
public:
	Builder builder;
	Detector detector;
};


