#ifndef YOLOV5_DET_ONNX_TENSORRT_DEEPSORT_H
#define YOLOV5_DET_ONNX_TENSORRT_DEEPSORT_H

#ifndef X_TYPEH
#include "x_type.h"
#endif

#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

/**
@brief yolov5_onnxtrt_multask_cleanup 注销函数
@param void
@retval TRUE 成功
@retval FALSE 失败
@description
*/
X_API bool STDCALL yolov5_onnxtrt_multask_cleanup(void);

/**
@brief Yolov5_onnxtrt_onnx2trt 模型onnx导出成trt
@param _onnxmodelfile 模型onnx的路径
@param _enginesavefile 生成的engine的文件全路径，最后不带反斜杠，比如“E:\\Item\\xxx.engine”;或者为空，则默认同目录下生成xxx.engine
@param __converttype 转换类型，0 UES_FP32;1 UES_FP16,咱不支持8位
@param _gpu_id gpu编号
@param _batch_size 尺度
@retval -1 onnx路径为空，-2为非onnx后缀文件; -3表示gpu id不正确； -4表示模型类型id不正确； -5表示转换类型不正确；-6 engine路径打开失败，0 成功
@description
*/
X_API int STDCALL Yolov5_onnxtrt_onnx2trt(const char* _onnxmodelfile, const char* _enginesavefile = "",
	int _converttype = 0,
	int _gpu_id = 0,
	int _batch_size = 1);

/**
@brief yolov5_onnxtrt_multask_create 创建识别任务,自动分配gpu
@param _engine文件，_classname objname文件
@param _conf_thres 置性度阈值，_nms_thres 非最大抑制阈值
@param _gpu_id gpu编号
@retval 任务索引，0创建失败，大于0则成功
@description
*/
X_API uint64_t STDCALL yolov5_onnxtrt_multask_create(const char* _enginefile, const char* _classname, double _conf_thres = 0.4, double _nms_thres = 0.4, int _gpu_id = 0);

/**
@brief yolov5_onnxtrt_multask_destroy 销毁指定任务
@param _taskindex 任务索引
@retval TRUE 成功
@retval FALSE 失败
@description
*/
X_API bool STDCALL yolov5_onnxtrt_multask_destroy(uint64_t _taskindex);

/**
@brief yolov5_onnxtrt_multask_set_conf_iou 设置检测置性度和iou值
@param _taskindex 任务索引
@param _conf_thres 置性度
@param _iou_thres IOU值
@description
*/
X_API void STDCALL yolov5_onnxtrt_multask_set_conf_iou(uint64_t _taskindex, double _conf_thres = 0.4, double _nms_thres = 0.5);

/**
@brief yolov5_onnxtrt_multask_process 检测
@param _taskindex 任务索引
@param _ImgSrc 识别图像
@param _resultimg 结果图像
@param _resultimg 目标图
@param _vecalarminfo结果数据
@description
*/
X_API bool STDCALL yolov5_onnxtrt_multask_process(uint64_t _taskindex, cv::Mat _ImgSrc, OUT cv::Mat& _resultimg, OUT std::vector<RecTrackResult>& _vecalarminfo);

#ifdef __cplusplus
}
#endif
#endif //YOLOV5_DET_ONNX_TENSORRT_DEEPSORT_H

