#pragma once
#include "yolov5_builder.hpp"
#include "yolov5_detector.hpp"
#include "GreatechDetectionClasses.h"
#define CLASS_DECLSPEC __declspec(dllexport)
namespace yolov5
{
	class CLASS_DECLSPEC GreatechDetector : public Detector
	{
	public:
		GreatechDetector()noexcept;
		~GreatechDetector()noexcept;

		virtual Result init(int flags = 0) noexcept;
		virtual bool isInitialized() const noexcept;
		virtual Result loadEngine(const std::string& filepath) noexcept;
		virtual Result loadEngine(const std::vector<char>& data) noexcept;
		virtual bool isEngineLoaded() const noexcept;
		virtual int numClasses() const noexcept;
		virtual Result setClasses(const GreatechClasses& classes) noexcept;
		virtual Result detect(const cv::Mat& img, std::vector<GreatechDetection>* out, int flags = 0) noexcept;
		virtual Result detect(const cv::cuda::GpuMat& img, std::vector<GreatechDetection>* out, int flags = 0) noexcept;
		virtual Result detectBatch(const std::vector<cv::Mat>& images, std::vector<std::vector<GreatechDetection>>* out, int flags = 0) noexcept;
		virtual Result detectBatch(const std::vector<cv::cuda::GpuMat>& images,std::vector<std::vector<GreatechDetection>>* out,int flags = 0) noexcept;
		virtual double scoreThreshold() const noexcept;
		virtual Result setScoreThreshold(const double& v) noexcept;
		virtual double nmsThreshold() const noexcept;
		virtual Result setNmsThreshold(const double& v) noexcept;
		virtual int batchSize() const noexcept;
		virtual cv::Size inferenceSize() const noexcept;
		virtual Result setLogger(std::shared_ptr<Logger> logger) noexcept;
		virtual std::shared_ptr<Logger> logger() const noexcept;
		//virtual Detector& operator=(const Detector& rhs);
		virtual Result _loadEngine(const std::vector<char>& data) noexcept;
		virtual void _printBindings(const std::unique_ptr<nvinfer1::ICudaEngine>& engine)const noexcept;

		virtual int _batchSize() const noexcept;

		virtual int _numClasses() const noexcept;

		virtual Result _detect(std::vector<GreatechDetection>* out);

		virtual Result _detectBatch(const int& nrImages,std::vector<std::vector<GreatechDetection>>* out);
		virtual Result _inference(const char* logid);
		virtual Result _decodeOutput(const char* logid, const int& index,std::vector<GreatechDetection>* out);
	private:
		bool                            _initialized;
		std::shared_ptr<Logger>         _logger;
		GreatechClasses                 _classes;
		double                          _scoreThreshold;
		double                          _nmsThreshold;


		/*  TensorRT    */
		std::unique_ptr<TensorRT_Logger>                _trtLogger;
		std::unique_ptr<nvinfer1::IRuntime>             _trtRuntime;

		/*  note: execution context depends on the engine, and should be destroyed
				_before_ the engine is destroyed */
		std::unique_ptr<nvinfer1::ICudaEngine>          _trtEngine;
		std::unique_ptr<nvinfer1::IExecutionContext>    _trtExecutionContext;

		/*  I/O  */
		internal::EngineBinding         _inputBinding;
		internal::EngineBinding         _outputBinding;

		std::unique_ptr<internal::Preprocessor>     _preprocessor;

		internal::DeviceMemory          _deviceMemory;

		std::vector<float>              _outputHostMemory;
	};
}