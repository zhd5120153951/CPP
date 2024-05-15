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
	//����dll
	//HINSTANCE hDLL = LoadLibrary("yolov5_v1.dll");

	HINSTANCE hDLL = LoadLibrary("zhdYOLO.dll");
	if (!hDLL)
	{
		cerr << "����zhdYOLO.dllʧ��..." << endl;
		return -1;
	}
	////��ȡ����ָ��--��������GreatechBuilder��ĺ���������GreatechBuilder��ĺ���
	//typedef GreatechBuilder* (*CreateBuilder)();
	//typedef void (*DestroyBuilder)(GreatechBuilder*);
	//CreateBuilder createBuilderFunc = (CreateBuilder)GetProcAddress(hDLL, "createBuilder");
	//DestroyBuilder destroyBuilderFunc = (DestroyBuilder)GetProcAddress(hDLL, "destroyBuilder");
	////��ȡ����ָ��--��������GreatechDetector��ĺ���������GreatechDetector��ĺ���
	//typedef GreatechDetector* (*CreateDetector)();
	//typedef void (*DestroyDetector)(GreatechDetector*);
	//CreateDetector createDetectorFunc = (CreateDetector)GetProcAddress(hDLL, "createDetector");
	//DestroyDetector destroyDetectorFunc = (DestroyDetector)GetProcAddress(hDLL, "destroyDetector");
	////��ȡ����ָ��--��������GreatechDetection��ĺ���������GreatechDetection��ĺ���
	//typedef GreatechDetection* (*CreateDetection)();
	//typedef void (*DestroyDetection)(GreatechDetection*);
	//CreateDetection createDetectionFunc = (CreateDetection)GetProcAddress(hDLL, "createDetection");
	//DestroyDetection destroyDetectionFunc = (DestroyDetection)GetProcAddress(hDLL, "destroyDetection");
	////��ȡ����ָ��--��������GreatechClass��ĺ���������GreatechClass��ĺ���
	//typedef GreatechClasses* (*CreateClasses)();
	//typedef void (*DestroyClasses)(GreatechClasses*);
	//CreateClasses createClassesFunc = (CreateClasses)GetProcAddress(hDLL, "createClasses");
	//DestroyClasses destroyClassesFunc = (DestroyClasses)GetProcAddress(hDLL, "destroyClasses");
	//yolov5_v2
	typedef yoloTask* (*CreateYoloTask)();
	typedef void (*DestroyYoloTask)(yoloTask*);
	CreateYoloTask createYolotaskFunc = (CreateYoloTask)GetProcAddress(hDLL, "createYoloTask");
	DestroyYoloTask destroyYoloTaskFunc = (DestroyYoloTask)GetProcAddress(hDLL, "destroyYoloTask");
	////��ȡ����ָ��--��������Builder��ĺ���������Builder��ĺ���
	//typedef Builder* (*CreateBuilder)();
	//typedef void (*DestroyBuilder)(Builder*);
	//CreateBuilder createBuilderFunc = (CreateBuilder)GetProcAddress(hDLL, "createBuilder");
	//DestroyBuilder destroyBuilderFunc = (DestroyBuilder)GetProcAddress(hDLL, "destroyBuilder");
	////��ȡ����ָ��--��������Detector��ĺ���������Detector��ĺ���
	//typedef Detector* (*CreateDetector)();
	//typedef void (*DestroyDetector)(Detector*);
	//CreateDetector createDetectorFunc = (CreateDetector)GetProcAddress(hDLL, "createDetector");
	//DestroyDetector destroyDetectorFunc = (DestroyDetector)GetProcAddress(hDLL, "destroyDetector");
	////��ȡ����ָ��--��������Detection��ĺ���������Detection��ĺ���
	//typedef Detection* (*CreateDetection)();
	//typedef void (*DestroyDetection)(Detection*);
	//CreateDetection createDetectionFunc = (CreateDetection)GetProcAddress(hDLL, "createDetection");
	//DestroyDetection destroyDetectionFunc = (DestroyDetection)GetProcAddress(hDLL, "destroyDetection");
	////��ȡ����ָ��--��������Class��ĺ���������Class��ĺ���
	//typedef Classes* (*CreateClasses)();
	//typedef void (*DestroyClasses)(Classes*);
	//CreateClasses createClassesFunc = (CreateClasses)GetProcAddress(hDLL, "createClasses");
	//DestroyClasses destroyClassesFunc = (DestroyClasses)GetProcAddress(hDLL, "destroyClasses");

	//ʹ�õ�����
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
	std::cout << "engineģ����Ϣ���£�" << std::endl;
	std::cout << "engine batch-size:" << detector_ptr->batchSize() << std::endl;
	std::cout << "engine inference-size:" << detector_ptr->inferenceSize() << std::endl;
	std::cout << "engine isLoaded:" << detector_ptr->isEngineLoaded() << std::endl;
	std::cout << "engine isInitilized:" << detector_ptr->isInitialized() << std::endl;
	std::cout << "engine NMS:" << detector_ptr->nmsThreshold() << std::endl;
	std::cout << "engine classesNum:" << detector_ptr->numClasses() << std::endl;
	std::cout << "engine scoreThreshold:" << detector_ptr->scoreThreshold() << std::endl;

	detector_ptr->setScoreThreshold(0.5);
	detector_ptr->setNmsThreshold(0.5);
	//������
	cv::VideoCapture cap = cv::VideoCapture(0);
	if (!cap.isOpened())
	{
		std::cout << "�޷�����Ƶ��..." << std::endl;
		return -1;
	}
	//��ʱ����֡��
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
				std::cout << "��⵽��..." << std::endl;
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
//	std::cout << "engineģ����Ϣ���£�" << std::endl;
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
	std::cout << "���߳�id:" << mainID << std::endl;
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
		//���߳�û������ɣ����̵߳ȴ�
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "���߳��ڵȴ����߳̽���....�ѵȴ���" << i << "��" << std::endl;
		i++;
	}
	
	destroyYoloTaskFunc(yolov5_ptr);
	FreeLibrary(hDLL);
	return 0;
}

