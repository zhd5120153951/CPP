#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#else
#include <dlfcn.h>
#endif
// 定义导出类的函数指针类型
typedef int (*funcPtr)(int, int);
int main() 
{
    // 加载动态库
#ifdef _WIN32
    HINSTANCE hDll = LoadLibrary("zhongkeAI.dll");
#else
    void* handle = dlopen("./zhongkeAI.so", RTLD_LAZY);
#endif
    if (!hDll) 
    {
        return -1;//dll加载失败
    }
    // 获取导出类的函数指针
    funcPtr func_add = (funcPtr)GetProcAddress(hDll, "add");
    if (!func_add) 
    {
        // 处理获取函数指针失败的情况
        return -2;
    }
    // 调用导出类的方法
    int result = func_add(42,28);

    // 打印结果
    printf("加法结果：%d\n", result);
    system("pause");
    // 卸载动态库
#ifdef _WIN32
    FreeLibrary(hDll);
#else
    dlclose(handle);
#endif

    return 0;
}