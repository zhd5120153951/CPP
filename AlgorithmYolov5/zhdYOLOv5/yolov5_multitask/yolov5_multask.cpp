#ifndef YOLOV5_DET_ONNX_TENSORRT_DEEPSORT_H
#define _CRT_SECURE_NO_WARNINGS
#include "yolov5_multask.h"
#endif

#include "yolov5.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>
#include <sstream>
#include <numeric>
#include "NvInfer.h"
#include <memory>
#include <NvOnnxParser.h>
#include <vector>
#include <numeric>
#include <sys/stat.h>
#include <unordered_set>
#include <cuda_runtime_api.h>


#if defined(_WIN32)
#	include <sys\timeb.h>
#	include <corecrt_io.h>
#	include <Windows.h>
#   include <wingdi.h>
#	include <Shlwapi.h>
#	include "imagehlp.h"
#	pragma comment(lib, "shlwapi.lib")
#   pragma comment(lib, "ole32.lib")
#   pragma comment(lib, "gdi32.lib")
#	pragma comment(lib,"imagehlp.lib")
#	undef min
#	undef max
#else
#	include <dirent.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <unistd.h>
#   include <stdarg.h>
#endif

using namespace std;
using namespace cv;
using namespace nvinfer1;

static BOOL s_is_startup = FALSE;

static bool exists(const string& path) {

#ifdef _WIN32
    return ::PathFileExistsA(path.c_str());
#else
    return access(path.c_str(), R_OK) == 0;
#endif
}

#ifndef WIN32
#define _MAX_DRIVE 260
#define _MAX_DIR 255
#define _MAX_FNAME 255
#define _MAX_EXT 128
void _splitpath(const char* path, char* drive, char* dir, char* fname, char* ext);
static void _split_whole_name(const char* whole_name, char* fname, char* ext);
#endif

#ifndef WIN32
void _splitpath(const char* path, char* drive, char* dir, char* fname, char* ext)
{
    char* p_whole_name;

    drive[0] = '\0';
    if (NULL == path)
    {
        dir[0] = '\0';
        fname[0] = '\0';
        ext[0] = '\0';
        return;
    }

    if ('/' == path[strlen(path)])
    {
        strcpy(dir, path);
        fname[0] = '\0';
        ext[0] = '\0';
        return;
    }

    p_whole_name = rindex((char*)path, '/');
    if (NULL != p_whole_name)
    {
        p_whole_name++;
        _split_whole_name(p_whole_name, fname, ext);

        snprintf(dir, p_whole_name - path, "%s", path);
    }
    else
    {
        _split_whole_name(path, fname, ext);
        dir[0] = '\0';
    }

    if (dir != NULL) {
        dir[strlen(dir)] = '/';
    }
}

static void _split_whole_name(const char* whole_name, char* fname, char* ext)
{
    char* p_ext;

    p_ext = rindex((char*)whole_name, '.');
    if (NULL != p_ext)
    {
        strcpy(ext, p_ext);
        snprintf(fname, p_ext - whole_name + 1, "%s", whole_name);
    }
    else
    {
        ext[0] = '\0';
        strcpy(fname, whole_name);
    }
}

#endif

using RESULT_INFO = std::pair<int, std::string>;

typedef struct _Yolov5DetRecogTask {
    bool								m_status = false;						        //任务状态，false销毁不可用，true可用
    char								m_enginefile[MAX_PATH] = {0};		            //模型cfg
    char								m_namefile[MAX_PATH] = { 0 };			        //模型name
    double								m_conf_thres = 0.25;				            //Confidence threshold
    double								m_nms_thres = 0.45;				                //Non-maximum suppression threshold
    int									gpu_id = 0;						                //指定运行gpu编号
    shared_ptr<SimpleYolo::Infer>		psimpleyolo = nullptr;				            //类指针
    std::vector<RESULT_INFO>*								pvecName = NULL;			//名称
    _Yolov5DetRecogTask(const char* _enginefile, const char* _classname, double _conf_thres = 0.4, double _nms_thres = 0.4, int _gpu_id = 0)
    {
		if (!(exists(_enginefile)) || (!exists(_classname)))
		{
            m_status = false;
            return;
		}

		memcpy(m_enginefile, _enginefile, MAX_PATH);
		memcpy(m_namefile, _classname, MAX_PATH);
		m_conf_thres = _conf_thres;
		m_nms_thres = _nms_thres;
		pvecName = new std::vector<RESULT_INFO>;
        gpu_id = _gpu_id;

		std::ifstream f(m_namefile);
		std::string name = "";
		int i = 0;
		while (std::getline(f, name))
		{
			pvecName->push_back(std::make_pair(i, name));
			i++;
		}

		SimpleYolo::set_device(0);

		psimpleyolo = SimpleYolo::create_infer(m_enginefile, gpu_id, m_conf_thres, m_nms_thres);
		if (psimpleyolo == nullptr) {
			m_status = false;
			return;
		}

		m_status = true;
    }
    ~_Yolov5DetRecogTask() 
    {
        m_status = false;
		if (psimpleyolo != NULL)
		{
			SimpleYolo::destroy_infer(psimpleyolo);
			psimpleyolo = nullptr;
		}

		if (pvecName != NULL)
		{
			pvecName->clear();
			pvecName->shrink_to_fit();
			delete pvecName;
			pvecName = NULL;
		}
    }

	/**
	@ingroup
	@brief 画识别结果到图
	@param pdetector任务指针
	@param _resultimg结果图像
	@param obj_info 检测到的目标信息
	@retval
	*/
	void draw_objects(cv::Mat& _resultimg, std::vector<RecTrackResult> obj_info);

	/**
	@brief task_process_img 图像处理,检测，并报警
	@param pdetector 任务指针
	@param _ImgSrc 图片数据
	@param resultMat 结果图片
	@param _vecalarminfo 报警目标
	@description
	*/
	bool task_process_img(cv::Mat _ImgSrc, OUT cv::Mat& _resultSrc, OUT std::vector<RecTrackResult>& _vecalarminfo);
    
    //打电话检测
    bool task_process_img_phonecall(cv::Mat _ImgSrc, OUT cv::Mat& _resultSrc, OUT std::vector<RecTrackResult>& _vecalarminfo);

    /**
	@brief set_conf_iou 设置阈值
	@param _conf_thres 检测阈值
	@param _nms_thres nms值
	@description
	*/
    void set_conf_iou(double _conf_thres = 0.4, double _nms_thres = 0.5);

} Yolov5DetRecogTask;


static std::tuple<uint8_t, uint8_t, uint8_t> hsv2bgr(float h, float s, float v) {
    const int h_i = static_cast<int>(h * 6);
    const float f = h * 6 - h_i;
    const float p = v * (1 - s);
    const float q = v * (1 - f * s);
    const float t = v * (1 - (1 - f) * s);
    float r, g, b;
    switch (h_i) {
    case 0:r = v; g = t; b = p; break;
    case 1:r = q; g = v; b = p; break;
    case 2:r = p; g = v; b = t; break;
    case 3:r = p; g = q; b = v; break;
    case 4:r = t; g = p; b = v; break;
    case 5:r = v; g = p; b = q; break;
    default:r = 1; g = 1; b = 1; break;
    }
    return make_tuple(static_cast<uint8_t>(b * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(r * 255));
}

static std::tuple<uint8_t, uint8_t, uint8_t> random_color(int id) {
    float h_plane = ((((unsigned int)id << 2) ^ 0x937151) % 100) / 100.0f;;
    float s_plane = ((((unsigned int)id << 3) ^ 0x315793) % 100) / 100.0f;
    return hsv2bgr(h_plane, s_plane, 1);
}

void _Yolov5DetRecogTask::draw_objects(cv::Mat& _resultimg, std::vector<RecTrackResult> obj_info)
{
	if (!m_status)
	{
		return;
	}

    for (size_t i = 0; i < obj_info.size(); i++) {
        RecTrackResult& obj = obj_info[i];

        uint8_t b, g, r;
        tie(b, g, r) = random_color(obj.classID);
        cv::Scalar curobjclor = cv::Scalar(b, g, r);//黑色

        //name
        std::string objname = "unknown ";
        if ((pvecName->size() > 0) && (obj.classID >= 0) && (obj.classID < pvecName->size()))
        {
            objname = pvecName->at(obj.classID).second;
        }
        else{
            objname = "id:" + to_string(obj.classID);
        }

        //score
        std::stringstream ss_score;
        ss_score << std::setprecision(2) << obj.score;

        cv::Rect brect(cv::Point2f(obj.left, obj.top), cv::Point2f(obj.right, obj.bottom));
        cv::rectangle(_resultimg, brect, curobjclor, 1);

		//cv::Point center_pt(obj.centen_x, obj.centen_y);//中心点
		//cv::circle(_resultimg, center_pt, 8, curobjclor, -1);

		//char c_cent_pt[56] = {0};
		//sprintf(c_cent_pt, "(x:%d,y:%d)", obj.centen_x, obj.centen_y);

        auto font_obj = cv::FONT_HERSHEY_DUPLEX;
        auto font_scale = 1;
        int thickness = 1;
        //int baseline = 0;
        std::string lable = objname + ":" + ss_score.str();
        //auto s_size = cv::getTextSize(lable, font_obj, font_scale, thickness, &baseline);
        //cv::rectangle(_resultimg,
        //    cv::Point(brect.tl().x, brect.tl().y - s_size.height - 5),
        //    cv::Point(brect.tl().x + s_size.width, brect.tl().y),
        //    cv::Scalar(0, 0, 255), -1);//框出文字区域，用红色填充
        cv::putText(_resultimg, lable, cv::Point(brect.x, brect.y - 5),
            font_obj, font_scale, cv::Scalar::all(0), thickness);

		/*cv::putText(_resultimg, c_cent_pt, cv::Point(obj.centen_x, obj.centen_y - 5),
			font_obj, font_scale, curobjclor, thickness);*/
    }
}

bool _Yolov5DetRecogTask::task_process_img(cv::Mat _ImgSrc, OUT cv::Mat& _resultSrc, OUT std::vector<RecTrackResult>& _vecalarminfo)
{
	if (_resultSrc.empty())
	{
		_ImgSrc.copyTo(_resultSrc);
	}

	if (!m_status)
	{
		return false;
	}

	auto objs = psimpleyolo->commit(_ImgSrc).get();
	for (auto& obj : objs) {

		RecTrackResult objbox;
		objbox.left = std::max(0, (int)obj.left);
		objbox.top = std::max(0, (int)obj.top);
		objbox.right = std::min((int)(obj.right), _ImgSrc.cols);
		objbox.bottom = std::min((int)(obj.bottom), _ImgSrc.rows);
		objbox.centen_x = (objbox.left + objbox.right) / 2;
		objbox.centen_y = (objbox.top + objbox.bottom) / 2;
		objbox.classID = (int)obj.class_label;
		objbox.score = obj.confidence;

		memset(objbox.label_text, 0, MAX_CHAR_LEN);
		if ((pvecName->size() > 0) && (objbox.classID >= 0) && (objbox.classID < pvecName->size()))
		{
			objbox.label_len = pvecName->at(objbox.classID).second.length();
			memcpy(objbox.label_text, pvecName->at(objbox.classID).second.c_str(), objbox.label_len);
		}
		_vecalarminfo.push_back(objbox);
	}

	if ((_vecalarminfo.size() > 0) && (!_resultSrc.empty()))
	{
		draw_objects(_resultSrc, _vecalarminfo);
        return true;
	}
	return false;//没有检测到
}

bool _Yolov5DetRecogTask::task_process_img_phonecall(cv::Mat _ImgSrc, OUT cv::Mat& _resultSrc, OUT std::vector<RecTrackResult>& _vecalarminfo)
{
	if (_resultSrc.empty())
	{
		_ImgSrc.copyTo(_resultSrc);
	}

	if (!m_status)
	{
		return false;
	}

	auto objs = psimpleyolo->commit(_ImgSrc).get();
	for (auto& obj : objs)
	{
		if(obj.class_label!=0)
			continue;//不是人
		RecTrackResult objbox;
		objbox.left = std::max(0, (int)obj.left);
		objbox.top = std::max(0, (int)obj.top);
		objbox.right = std::min((int)(obj.right), _ImgSrc.cols);
		objbox.bottom = std::min((int)(obj.bottom), _ImgSrc.rows);
		objbox.centen_x = (objbox.left + objbox.right) / 2;
		objbox.centen_y = (objbox.top + objbox.bottom) / 2;
		objbox.classID = (int)obj.class_label;
		objbox.score = obj.confidence;

		memset(objbox.label_text, 0, MAX_CHAR_LEN);
		if ((pvecName->size() > 0) && (objbox.classID >= 0) && (objbox.classID < pvecName->size()))
		{
			objbox.label_len = pvecName->at(objbox.classID).second.length();
			memcpy(objbox.label_text, pvecName->at(objbox.classID).second.c_str(), objbox.label_len);
		}
		_vecalarminfo.push_back(objbox);
	}

	if ((_vecalarminfo.size() > 0) && (!_resultSrc.empty()))
	{
		for (size_t i = 0; i < _vecalarminfo.size(); i++)
		{
			RecTrackResult& obj = _vecalarminfo[i];

			cv::Rect brectPerson(cv::Point2f(obj.left, obj.top), cv::Point2f(obj.right, obj.bottom));
			cv::rectangle(_resultSrc, brectPerson, cv::Scalar(0, 255, 0), 1);
			cv::Mat detImg = _resultSrc(brectPerson).clone();//截取人的区域

			auto objs_phone = psimpleyolo->commit(detImg).get();

			for (auto& obj:objs_phone)
			{
				if (obj.class_label==1)//是手机
				{
					RecTrackResult objbox_phone;
					objbox_phone.left = std::max(0, (int)obj.left);
					objbox_phone.top = std::max(0, (int)obj.top);
					objbox_phone.right = std::min((int)(obj.right), _ImgSrc.cols);
					objbox_phone.bottom = std::min((int)(obj.bottom), _ImgSrc.rows);
					objbox_phone.centen_x = (objbox_phone.left + objbox_phone.right) / 2;
					objbox_phone.centen_y = (objbox_phone.top + objbox_phone.bottom) / 2;
					objbox_phone.classID = (int)obj.class_label;
					objbox_phone.score = obj.confidence;

					cv::Rect brectPhone(cv::Point2f(obj.left, obj.top), cv::Point2f(obj.right, obj.bottom));
					cv::rectangle(_resultSrc, cv::Rect(brectPerson.x + brectPhone.x, brectPerson.y + brectPhone.y, brectPhone.width, brectPhone.height), cv::Scalar(0, 0, 255), 1);//画手机
					//...接下来是确定标签名
					
					
				}
				
			}
			

			auto font_obj = cv::FONT_HERSHEY_DUPLEX;
			auto font_scale = 1;
			int thickness = 1;
			//int baseline = 0;
			//auto s_size = cv::getTextSize(lable, font_obj, font_scale, thickness, &baseline);
			//cv::rectangle(_resultimg,
			//    cv::Point(brect.tl().x, brect.tl().y - s_size.height - 5),
			//    cv::Point(brect.tl().x + s_size.width, brect.tl().y),
			//    cv::Scalar(0, 0, 255), -1);//框出文字区域，用红色填充
			cv::putText(_resultimg, lable, cv::Point(brect.x, brect.y - 5),
				font_obj, font_scale, cv::Scalar::all(0), thickness);
		}


		draw_objects(_resultSrc, _vecalarminfo);
		return true;
	}
	return false;//没有检测到
}

void _Yolov5DetRecogTask::set_conf_iou(double _conf_thres /*= 0.4*/, double _nms_thres /*= 0.5*/)
{
    m_conf_thres = _conf_thres;
    m_nms_thres = _nms_thres;
}

//线程安全操作
typedef std::shared_ptr<Yolov5DetRecogTask>							PYolov5DetRecogTask_ptr;
typedef std::map<int, PYolov5DetRecogTask_ptr>						PYolov5DetRecogTask_ptrMap;
PYolov5DetRecogTask_ptrMap											s_PYolov5DetRecogTaskMap;
std::mutex															s_PYolov5DetRecogTask_ptrMapLock;
uint64_t															s_nYolov5DetRecogTaskNumber = 1;

//获取智能指针
PYolov5DetRecogTask_ptr GetHasPtr(int taskindex)
{
	std::lock_guard<std::mutex> lock(s_PYolov5DetRecogTask_ptrMapLock);
    PYolov5DetRecogTask_ptrMap::iterator iterator;
    PYolov5DetRecogTask_ptr   pClient = NULL;
	iterator = s_PYolov5DetRecogTaskMap.find(taskindex);
	if (iterator != s_PYolov5DetRecogTaskMap.end())
	{
		pClient = (*iterator).second;
		return pClient;
	}
	else
	{
		return NULL;
	}
}

X_API bool STDCALL yolov5_onnxtrt_multask_cleanup(void)
{
	//智能互斥锁
	std::lock_guard<std::mutex> lock(s_PYolov5DetRecogTask_ptrMapLock);

	//挨着销毁
    PYolov5DetRecogTask_ptrMap::iterator iterator1;
    PYolov5DetRecogTask_ptr   pClient = NULL;
	for (iterator1 = s_PYolov5DetRecogTaskMap.begin(); iterator1 != s_PYolov5DetRecogTaskMap.end(); ++iterator1)
	{
		pClient = (*iterator1).second;
		pClient.reset();
	}
    s_PYolov5DetRecogTaskMap.clear();

    return true;
}

X_API int STDCALL Yolov5_onnxtrt_onnx2trt(const char* _onnxmodelfile, const char* _enginesavefile /*= ""*/, int _converttype /*= 0*/, int _gpu_id /*= 0*/, int _batch_size /*= 1*/)
{
    if (!exists(_onnxmodelfile))
    {
        return -1;
    }
    char drive[_MAX_DRIVE] = { 0 };
    char dir[_MAX_DIR] = { 0 };
    char fname[_MAX_FNAME] = { 0 };
    char ext[_MAX_EXT] = { 0 };
    _splitpath(_onnxmodelfile, drive, dir, fname, ext);

    if ((strcmp(ext, ".onnx") != 0))
    {
        std::cout << "_onnxmodelfile:" << _onnxmodelfile << "not onnxfile";
        return -2;
    }

    char enginepath[256] = { 0 };
    if (strlen(_enginesavefile) == 0)
    {
        sprintf(enginepath, "%s%s%s.engine", drive, dir, fname);
    }
    else {
        sprintf(enginepath, "%s", _enginesavefile);
    }

    //gpu个数
    int nGpuNumber = 0;
    cudaGetDeviceCount(&nGpuNumber);
    if (_gpu_id >= nGpuNumber)
    {
        std::cout << "checked " << nGpuNumber << "gpu card, but current " << "_gpu_id:" << _gpu_id << "not in rande of GPU list";
        return -3;
    }
    std::cout << "_gpu_id:" << _gpu_id;

    SimpleYolo::Mode modeltype = SimpleYolo::Mode::FP32;
    switch (_converttype)
    {
    case 0: {
        {
            modeltype = SimpleYolo::Mode::FP32;
            std::cout << "modeltype:Convert_Type::FP32";
            break;
        }
    }
    case 1: {
        {
            modeltype = SimpleYolo::Mode::FP16;
            std::cout << "modeltype:Convert_Type::FP16";
            break;
        }
    }
    case 2: {
        {
            modeltype = SimpleYolo::Mode::INT8;
            std::cout << "modeltype:Convert_Type::INT8";
            break;
        }
    }
    default:
        std::cout << "modeltype error, not in range";
        return -5;
    }

    std::cout << "_batch_size:" << _batch_size;

    std::cout << "convert onnx model file:" << _onnxmodelfile;
    std::cout << "save trt engine file:" << enginepath;

    cudaSetDevice(_gpu_id);

    bool convertflg = false;
    if (!exists(enginepath)) {
        convertflg = compile(modeltype, _batch_size, _onnxmodelfile, enginepath,1<<30);
    }
    else {
        std::cout << "_enginesavefile:" << enginepath << "is exists";
    }

    if (convertflg)
    {
        return 0;
    }
    return -6;
}

X_API uint64_t STDCALL yolov5_onnxtrt_multask_create(const char* _enginefile, const char* _classname, double _conf_thres, double _nms_thres, int _gpu_id)
{
    if (!(exists(_enginefile)) || (!exists(_classname)))
    {
        return 0;
    }

	//智能互斥锁
	std::lock_guard<std::mutex> lock(s_PYolov5DetRecogTask_ptrMapLock);
    PYolov5DetRecogTask_ptr pshapedet = NULL;
	try
	{
		pshapedet = std::make_shared<Yolov5DetRecogTask>(_enginefile, _classname, _conf_thres, _nms_thres);
		if (!pshapedet->m_status)
		{
			pshapedet.reset();
            return 0;
		}
	}
	catch (const std::exception& e)
	{
        return 0;
	}

	//创建成功，加入map
	int curindex = s_nYolov5DetRecogTaskNumber;
	auto ret = s_PYolov5DetRecogTaskMap.insert(std::make_pair(curindex, pshapedet));
	if (!ret.second)
	{
		pshapedet.reset();
        return 0;
	}
    s_nYolov5DetRecogTaskNumber++;//每创建成功一个推理任务就++

	return curindex;
}

X_API bool STDCALL yolov5_onnxtrt_multask_destroy(uint64_t _taskindex)
{
	//智能互斥锁
	std::lock_guard<std::mutex> lock(s_PYolov5DetRecogTask_ptrMapLock);

	//先查找
    PYolov5DetRecogTask_ptrMap::iterator iterator_pt;
	iterator_pt = s_PYolov5DetRecogTaskMap.find(_taskindex);
	if (iterator_pt != s_PYolov5DetRecogTaskMap.end())
	{
		//找到后销毁,会自动调用析构函数
        s_PYolov5DetRecogTaskMap.erase(iterator_pt);
		return true;
	}
	return false;
}


X_API void STDCALL yolov5_onnxtrt_multask_set_conf_iou(uint64_t _taskindex, double _conf_thres /*= 0.4*/, double _nms_thres /*= 0.5*/)
{
	//获取智能指针
    PYolov5DetRecogTask_ptr ptcurdettask = GetHasPtr(_taskindex);
	if (ptcurdettask)
	{
		//执行参数设置操作
		ptcurdettask->set_conf_iou(_conf_thres, _nms_thres);
        ptcurdettask->psimpleyolo->set_threshold(_conf_thres, _nms_thres);
	}
}

X_API bool STDCALL yolov5_onnxtrt_multask_process(uint64_t _taskindex, cv::Mat _ImgSrc, OUT cv::Mat& _resultimg, OUT std::vector<RecTrackResult>& _vecalarminfo)
{
    if(_resultimg.empty()){
        _ImgSrc.copyTo(_resultimg);
    }

	//获取智能指针
	PYolov5DetRecogTask_ptr ptcurdettask = GetHasPtr(_taskindex);
	if (ptcurdettask)
	{
		if (_ImgSrc.channels() == 1)
		{
			cvtColor(_ImgSrc, _ImgSrc, cv::COLOR_GRAY2BGR);
		}
		else if (_ImgSrc.channels() == 4) {
			cvtColor(_ImgSrc, _ImgSrc, cv::COLOR_BGRA2BGR);
		}

		//执行操作
		return ptcurdettask->task_process_img(_ImgSrc, _resultimg, _vecalarminfo);
	}
	return false;
}

X_API bool STDCALL yolov5_onnxtrt_multask_process_phonecall(uint64_t _taskindex, cv::Mat _ImgSrc, OUT cv::Mat& _resultimg, OUT std::vector<RecTrackResult>& _vecalarminfo)
{
	if (_resultimg.empty()) 
    {
		_ImgSrc.copyTo(_resultimg);
	}

	//获取智能指针
	PYolov5DetRecogTask_ptr ptcurdettask = GetHasPtr(_taskindex);
	if (ptcurdettask)
	{
		if (_ImgSrc.channels() == 1)
		{
			cvtColor(_ImgSrc, _ImgSrc, cv::COLOR_GRAY2BGR);
		}
		else if (_ImgSrc.channels() == 4) {
			cvtColor(_ImgSrc, _ImgSrc, cv::COLOR_BGRA2BGR);
		}

		//执行推理
		return ptcurdettask->task_process_img(_ImgSrc, _resultimg, _vecalarminfo);
	}
	return false;
}
