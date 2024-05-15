#pragma once
#include "yolov5_detection.hpp"
#define CLASS_DECLSPEC __declspec(dllexport)

namespace yolov5
{
	class CLASS_DECLSPEC GreatechDetection : public Detection
	{
	public:
		GreatechDetection()noexcept;
		//GreatechDetection(const int& classId, const cv::Rect& boundingBox, const double& score())noexcept;
		GreatechDetection(const int& classId, const cv::Rect& boundingBox, const double& score) noexcept;
		~GreatechDetection()noexcept;

		virtual const int32_t& classId() const noexcept;
		virtual const cv::Rect& boundingBox() const noexcept;
		virtual const double& score() const noexcept;
		virtual const std::string& score_str() const noexcept;
		virtual const std::string& className() const noexcept;
		virtual bool setClassName(const std::string& name) noexcept;

	private:
		int32_t             _classId;
		std::string         _className;

		cv::Rect            _boundingBox;
		double              _score;
	};
	Result visualizeDetection(const GreatechDetection& detection, cv::Mat* image,
		const cv::Scalar& color,
		const double& fontScale) noexcept;//想要此功能，只有在子类中单独定义

	class CLASS_DECLSPEC GreatechClasses : public Classes
	{
	public:
		GreatechClasses()noexcept;
		~GreatechClasses()noexcept;

		virtual Result load(const std::vector<std::string>& names) noexcept;
		virtual Result loadFromFile(const std::string& filepath) noexcept;
		virtual bool isLoaded() const noexcept;
		virtual Result getName(const int& classId, std::string* out) const noexcept;
		virtual void setLogger(std::shared_ptr<Logger> logger) noexcept;
	private:
		std::shared_ptr<Logger>         _logger;
		std::vector<std::string>        _names;
	};
}