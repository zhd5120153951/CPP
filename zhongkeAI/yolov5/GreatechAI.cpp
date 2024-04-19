#include "pch.h"
#include "GreatechAI.h"

GreatechAI::GreatechAI()
{

}

GreatechAI::~GreatechAI()
{

}

void GreatechAI::builder_init()
{
	builder.init();
}

void GreatechAI::builder_buildEngine(const std::string& inputFilePath, const std::string& outputFilePath)
{
	builder.buildEngine(inputFilePath, outputFilePath);
}

void GreatechAI::builder_buildEngine(const std::string& inputFilePath, std::vector<char>* output)
{
	builder.buildEngine(inputFilePath, output);
}

void GreatechAI::builder_setLogger(std::shared_ptr<Logger> logger)
{
	builder.setLogger(logger);
}

std::shared_ptr<yolov5::Logger> GreatechAI::logger()
{
	builder.logger();
}


