#include <iostream>
#include <windows.h>
//#include "GreatechBuilder.h"
//#include "GreatechDetector.h"
//#include "GreatechDetectionClasses.h"
#include "yolov5_builder.hpp"
#include "yolov5_detector.hpp"
#include "yolov5_detection.hpp"
#include "yoloTask.h"

using namespace std;
using namespace yolov5;
int main(int argc, char** argv)
{
	//加载dll
	//HINSTANCE hDLL = LoadLibrary("yolov5_v1.dll");

	HINSTANCE hDLL = LoadLibrary("zhdYOLO.dll");
	if (!hDLL)
	{
		cerr << "加载zhdYOLO.dll失败..." << endl;
		return -1;
	}
	////获取函数指针--导出构造GreatechBuilder类的函数和析构GreatechBuilder类的函数
	//typedef GreatechBuilder* (*CreateBuilder)();
	//typedef void (*DestroyBuilder)(GreatechBuilder*);
	//CreateBuilder createBuilderFunc = (CreateBuilder)GetProcAddress(hDLL, "createBuilder");
	//DestroyBuilder destroyBuilderFunc = (DestroyBuilder)GetProcAddress(hDLL, "destroyBuilder");
	////获取函数指针--导出构造GreatechDetector类的函数和析构GreatechDetector类的函数
	//typedef GreatechDetector* (*CreateDetector)();
	//typedef void (*DestroyDetector)(GreatechDetector*);
	//CreateDetector createDetectorFunc = (CreateDetector)GetProcAddress(hDLL, "createDetector");
	//DestroyDetector destroyDetectorFunc = (DestroyDetector)GetProcAddress(hDLL, "destroyDetector");
	////获取函数指针--导出构造GreatechDetection类的函数和析构GreatechDetection类的函数
	//typedef GreatechDetection* (*CreateDetection)();
	//typedef void (*DestroyDetection)(GreatechDetection*);
	//CreateDetection createDetectionFunc = (CreateDetection)GetProcAddress(hDLL, "createDetection");
	//DestroyDetection destroyDetectionFunc = (DestroyDetection)GetProcAddress(hDLL, "destroyDetection");
	////获取函数指针--导出构造GreatechClass类的函数和析构GreatechClass类的函数
	//typedef GreatechClasses* (*CreateClasses)();
	//typedef void (*DestroyClasses)(GreatechClasses*);
	//CreateClasses createClassesFunc = (CreateClasses)GetProcAddress(hDLL, "createClasses");
	//DestroyClasses destroyClassesFunc = (DestroyClasses)GetProcAddress(hDLL, "destroyClasses");
	//yolov5_v2
	typedef yoloTask* (*CreateYoloTask)();
	typedef void (*DestroyYoloTask)(yoloTask*);
	CreateYoloTask createYolotaskFunc = (CreateYoloTask)GetProcAddress(hDLL, "createYoloTask");
	DestroyYoloTask destroyYoloTaskFunc = (DestroyYoloTask)GetProcAddress(hDLL, "destroyYoloTask");
	////获取函数指针--导出构造Builder类的函数和析构Builder类的函数
	//typedef Builder* (*CreateBuilder)();
	//typedef void (*DestroyBuilder)(Builder*);
	//CreateBuilder createBuilderFunc = (CreateBuilder)GetProcAddress(hDLL, "createBuilder");
	//DestroyBuilder destroyBuilderFunc = (DestroyBuilder)GetProcAddress(hDLL, "destroyBuilder");
	////获取函数指针--导出构造Detector类的函数和析构Detector类的函数
	//typedef Detector* (*CreateDetector)();
	//typedef void (*DestroyDetector)(Detector*);
	//CreateDetector createDetectorFunc = (CreateDetector)GetProcAddress(hDLL, "createDetector");
	//DestroyDetector destroyDetectorFunc = (DestroyDetector)GetProcAddress(hDLL, "destroyDetector");
	////获取函数指针--导出构造Detection类的函数和析构Detection类的函数
	//typedef Detection* (*CreateDetection)();
	//typedef void (*DestroyDetection)(Detection*);
	//CreateDetection createDetectionFunc = (CreateDetection)GetProcAddress(hDLL, "createDetection");
	//DestroyDetection destroyDetectionFunc = (DestroyDetection)GetProcAddress(hDLL, "destroyDetection");
	////获取函数指针--导出构造Class类的函数和析构Class类的函数
	//typedef Classes* (*CreateClasses)();
	//typedef void (*DestroyClasses)(Classes*);
	//CreateClasses createClassesFunc = (CreateClasses)GetProcAddress(hDLL, "createClasses");
	//DestroyClasses destroyClassesFunc = (DestroyClasses)GetProcAddress(hDLL, "destroyClasses");

	//使用导出类
	yoloTask* yolov5_ptr = createYolotaskFunc();
	/*Builder* builder_ptr = createBuilderFunc();
	Detector* detector_ptr = createDetectorFunc();
	Detection* detection_ptr = createDetectionFunc();
	Classes* classes_ptr = createClassesFunc();*/
	
#if false
	//builder_ptr->init();
	//builder_ptr->buildEngine("yolov5s.onnx", "yolov5s.engine");
	detector_ptr->init();
	detector_ptr->loadEngine("yolov5s.engine");
	std::cout << "engine模型信息如下：" << std::endl;
	std::cout << "engine batch-size:" << detector_ptr->batchSize() << std::endl;
	std::cout << "engine inference-size:" << detector_ptr->inferenceSize() << std::endl;
	std::cout << "engine isLoaded:" << detector_ptr->isEngineLoaded() << std::endl;
	std::cout << "engine isInitilized:" << detector_ptr->isInitialized() << std::endl;
	std::cout << "engine NMS:" << detector_ptr->nmsThreshold() << std::endl;
	std::cout << "engine classesNum:" << detector_ptr->numClasses() << std::endl;
	std::cout << "engine scoreThreshold:" << detector_ptr->scoreThreshold() << std::endl;

	detector_ptr->setScoreThreshold(0.5);
	detector_ptr->setNmsThreshold(0.5);
	//推理部分
	cv::VideoCapture cap = cv::VideoCapture(0);
	if (!cap.isOpened())
	{
		std::cout << "无法打开视频流..." << std::endl;
		return -1;
	}
	//暂时不加帧差
	int seq = 1;
	cv::Mat frame;
	std::vector<Detection>detections;
	while (true)
	{
		cap >> frame;
		if (frame.empty())
		{
			break;
		}
		detector_ptr->detect(frame, &detections);
		for (int i = 0; i < detections.size(); i++)
		{
			if (detections[i].classId() == 0)
			{
				std::cout << "检测到人..." << std::endl;
				cv::Rect rectPerson = detections[i].boundingBox();
				cv::rectangle(frame, rectPerson, cv::Scalar(0, 255, 0), 1);
				cv::imwrite("save/" + std::to_string(seq) + ".jpg", frame);
				seq += 1;
			}
		}

	}
#else
//	greatechAI->detector_init();
//	greatechAI->detector_loadEngine("yolov5s.engine");
//	std::cout << "engine模型信息如下：" << std::endl;
//	std::cout << "engine batch-size:" << greatechAI->detector_batchSize() << std::endl;
//	std::cout << "engine inference-size:" << greatechAI->detector_inferenceSize() << std::endl;
//	std::cout << "engine isLoaded:" << greatechAI->detector_isEngineLoaded() << std::endl;
//	std::cout << "engine isInitilized:" << greatechAI->detector_isInitialized() << std::endl;
//	std::cout << "engine NMS:" << greatechAI->detector_nmsThreshold() << std::endl;
//	std::cout << "engine classesNum:" << greatechAI->detector_numClasses() << std::endl;
//	std::cout << "engine scoreThreshold:" << greatechAI->detector_scoreThreshold() << std::endl;
//
//	greatechAI->detector_setScoreThreshold(0.5);
//	greatechAI->detector_setNmsThreshold(0.5);
	std::thread::id mainID = std::this_thread::get_id();
	std::cout << "主线程id:" << mainID << std::endl;
	std::string input = "yolov5s.onnx";
	std::string output = "yolov5s.engine";
	yolov5_ptr->buildEngine(input, output);
	yolov5_ptr->detect(0, "yolov5s.engine");
	/*double ret = yolov5_ptr->add(10, 5);
	std::cout << "add retsult:" << ret << std::endl;
	ret = yolov5_ptr->subtract(10, 5);
	std::cout << "add retsult:" << ret << std::endl;*/

#endif
	/*destroyBuilderFunc(builder_ptr);
	destroyDetectorFunc(detector_ptr);
	destroyDetectionFunc(detection_ptr);
	destroyClassesFunc(classes_ptr);*/
	int i = 1;
	while (!yolov5_ptr->shouldExit)
	{
		//子线程没工作完成，主线程等待
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "主线程在等待子线程结束....已等待：" << i << "秒" << std::endl;
		i++;
	}
	
	destroyYoloTaskFunc(yolov5_ptr);
	FreeLibrary(hDLL);
	return 0;
}

