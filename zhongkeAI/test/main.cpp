#include <iostream>
#include <windows.h>
#include "GreatechAI.h"
using namespace std;
int main() 
{
    //加载dll
    HINSTANCE hDLL = LoadLibrary("yolov5.dll");
    if (!hDLL)
    {
        cerr << "加载yolov5.dll失败..." << endl;
        return -1;
    }
    //获取函数指针
    typedef GreatechAI* (*CreateGreatechAI)();
    CreateGreatechAI createFunc = (CreateGreatechAI)GetProcAddress(hDLL, "createExportClass");
    //使用导出类
	GreatechAI* greatechAI = createFunc();
    greatechAI->builder_init();
    greatechAI->builder_buildEngine("yolov5s.onnx", "yolov5s.engine");
    
    system("pause");
    return 0;
}