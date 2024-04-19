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

	Result builder_init();
	Result builder_buildEngine(const std::string& inputFilePath,const std::string& outputFilePath);
	Result builder_buildEngine(const std::string& inputFilePath, std::vector<char>* output);
	Result builder_setLogger(std::shared_ptr<Logger> logger);
	std::shared_ptr<Logger> logger();

	Result detector_init();
	bool detector_isInitialized();
	Result detector_loadEngine(const std::string& inputFilePath);
	Result detector_loadEngine(const std::vector<char>& input);
	bool detector_isEngineLoaded();
	int detector_numClasses();
	Result detector_setClasses(const Classes& classes);
	Result detector_detect(const cv::Mat& img,
		std::vector<Detection>* out,
		int flags);
	Result detector_detect(const cv::cuda::GpuMat& img, std::vector<Detection>* out, int flags);
	Result detector_detectBatch(const std::vector<cv::Mat>& images,
		std::vector<std::vector<Detection>>* out,
		int flags);
	Result detector_detectBatch(const std::vector<cv::cuda::GpuMat>& images,
		std::vector<std::vector<Detection>>* out,
		int flags);
	double detector_scoreThreshold();
	Result detector_setScoreThreshold(const double& value);
	double detector_nmsThreshold();
	Result detector_setNmsThreshold(const double& value);
	int detector_batchSize();
	cv::Size detector_inferenceSize();
	Result setLogger(std::shared_ptr<Logger> logger);
	std::shared_ptr<Logger> logger();
public:
	Builder builder;
	Detector detector;
	//Result result;//暂时无用
	//Classes classes;//暂时无用
};


