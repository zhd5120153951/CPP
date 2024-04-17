#pragma once

#ifdef _WIN32
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif
int DLL_EXPORT add(int a, int b);//不是导出类的函数
class DLL_EXPORT GreatechAI {
public:
    int add(int a, int b);
    int subtract(int a, int b);
};
