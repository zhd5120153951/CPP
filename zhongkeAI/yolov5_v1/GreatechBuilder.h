/*!
 * \file GreatechBuilder.h
 * \date 2024/04/21 10:49
 *
 * \author Hedong
 * \Contact: zenghedong@outlook.com
 *
 * \brief: 派生自Builder，并封装为导出类
 *
 * \TODO: 新增其他功能接口
 *
 * \note
*/
#pragma once
#include "yolov5_builder.hpp"
#define CLASS_DECLSPEC __declspec(dllexport)
namespace yolov5
{

	/**
	 * Build the YoloV5 TensorRT engine, which can be used for detection
	 *
	 * Before building a TensorRT engine, you should first initialize the
	 * builder by using the init() method.
	 *
	 * ### Basic usage example
	 * yolov5::GreatechBuilder* builder_ptr=new GreatechBuilder();
	 * builder_ptr->init();
	 * builder_ptr->buildEngine("yolov5.onnx", "yolov5.engine");
	 */
	class CLASS_DECLSPEC GreatechBuilder : public Builder
	{
	//重写基类纯虚函数
	public:
		GreatechBuilder()noexcept;
		~GreatechBuilder()noexcept;
		virtual Result init()noexcept;
		virtual Result buildEngine(const std::string& inputFilePath, const std::string& outputFilePath, Precision precision /* = PRECISION_FP32 */)const noexcept;
		virtual Result buildEngine(const std::string& inputFilePath, std::vector<char>* output, Precision precision /*=PRECISION_FP32*/)const noexcept;
		virtual Result setLogger(std::shared_ptr<Logger> logger) noexcept;
		virtual std::shared_ptr<Logger> logger() const noexcept;
	
		virtual Result _buildEngine(const std::string& inputFilePath,
			std::shared_ptr<nvinfer1::IHostMemory>* output,
			Precision precision) const noexcept;
	//自定义功能接口

	private:
		bool                               _initialized;
		std::shared_ptr<Logger>            _logger;
		std::unique_ptr<TensorRT_Logger>   _trtLogger;
	};
}