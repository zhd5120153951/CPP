#include "pch.h"
#include "GreatechAI.h"

GreatechAI::GreatechAI()
{

}

GreatechAI::~GreatechAI()
{

}

Result GreatechAI::builder_init()
{
	return builder.init();
}

Result GreatechAI::builder_buildEngine(const std::string& inputFilePath, const std::string& outputFilePath)
{
	return builder.buildEngine(inputFilePath, outputFilePath);
}

Result GreatechAI::builder_buildEngine(const std::string& inputFilePath, std::vector<char>* output)
{
	return builder.buildEngine(inputFilePath, output);
}

Result GreatechAI::builder_setLogger(std::shared_ptr<Logger> logger)
{
	return builder.setLogger(logger);
}

std::shared_ptr<yolov5::Logger> GreatechAI::logger()
{
	return builder.logger();
}

Result GreatechAI::detector_init()
{
	return this->detector.init();
}

bool GreatechAI::detector_isInitialized()
{
	return this->detector.isInitialized();
}

Result GreatechAI::detector_loadEngine(const std::string& inputFilePath)
{
	return this->detector.loadEngine(inputFilePath);
}

Result GreatechAI::detector_loadEngine(const std::vector<char>& input)
{
	return this->detector.loadEngine(input);
}

bool GreatechAI::detector_isEngineLoaded()
{
	return this->detector.isEngineLoaded();
}

int GreatechAI::detector_numClasses()
{
	return this->detector.numClasses();
}

Result GreatechAI::detector_setClasses(const Classes& classes)
{
	return this->detector.setClasses(classes);
}

Result GreatechAI::detector_detect(const cv::Mat& img, std::vector<Detection>* out, int flags)
{
	return this->detector.detect(img, out, flags);
}

Result GreatechAI::detector_detect(const cv::cuda::GpuMat& img, std::vector<Detection>* out, int flags)
{
	return this->detector.detect(img, out, flags);
}

Result GreatechAI::detector_detectBatch(const std::vector<cv::Mat>& images, std::vector<std::vector<Detection>>* out, int flags)
{
	return this->detector.detectBatch(images, out, flags);
}

Result GreatechAI::detector_detectBatch(const std::vector<cv::cuda::GpuMat>& images, std::vector<std::vector<Detection>>* out, int flags)
{
	return this->detector.detectBatch(images, out, flags);
}

double GreatechAI::detector_scoreThreshold()
{
	return this->detector.scoreThreshold();
}

Result GreatechAI::detector_setScoreThreshold(const double& value)
{
	return this->detector.setScoreThreshold(value);
}

double GreatechAI::detector_nmsThreshold()
{
	return this->detector.nmsThreshold();
}

Result GreatechAI::detector_setNmsThreshold(const double& value)
{
	return this->detector.setNmsThreshold(value);
}

int GreatechAI::detector_batchSize()
{
	return this->detector.batchSize();
}

cv::Size GreatechAI::detector_inferenceSize()
{
	return this->detector.inferenceSize();
}


