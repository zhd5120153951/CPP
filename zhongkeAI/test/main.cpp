#include <iostream>
#include <windows.h>
#include "GreatechAI.h"
using namespace std;
int main() 
{
    //����dll
    HINSTANCE hDLL = LoadLibrary("yolov5.dll");
    if (!hDLL)
    {
        cerr << "����yolov5.dllʧ��..." << endl;
        return -1;
    }
    //��ȡ����ָ��
    typedef GreatechAI* (*CreateGreatechAI)();
    CreateGreatechAI createFunc = (CreateGreatechAI)GetProcAddress(hDLL, "createExportClass");
    //ʹ�õ�����
	GreatechAI* greatechAI = createFunc();
    greatechAI->builder_init();
    greatechAI->builder_buildEngine("yolov5s.onnx", "yolov5s.engine");
    
    system("pause");
    return 0;
}