#include "pch.h"
#include "yoloTask.h"
#include <vector>
#include <thread>
using namespace yolov5;
yoloTask::yoloTask()
{

}

yoloTask::~yoloTask()
{

}

Result yoloTask::detectTask(Detector& detector)
{
	std::cout << "���̺߳�����..." << std::endl;
	detector.init();
	detector.loadEngine("yolov5s.engine");
	std::cout << "isInited:" << detector.isInitialized() << std::endl;
	std::cout << "isLoaded:" << detector.isEngineLoaded() << std::endl;
	std::cout << "batchsize:" << detector.batchSize() << std::endl;
	std::cout << "numClasses:" << detector.numClasses() << std::endl;
	std::cout << "score:" << detector.scoreThreshold() << std::endl;
	std::cout << "nms:" << detector.nmsThreshold() << std::endl;
	std::cout << "detector�������" << std::endl;
}
Result yoloTask::detector_task()
{
	std::thread* th=new std::thread(yoloTask::detectTask, this->detector);
	th->detach();
	std::cout << "�첽���̣߳����������߳���..." << std::endl;
	delete th;
	return RESULT_SUCCESS;
}
