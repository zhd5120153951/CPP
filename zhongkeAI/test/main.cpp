#ifdef _WIN32
#include <windows.h>
#include <stdio.h>
#else
#include <dlfcn.h>
#endif
// ���嵼����ĺ���ָ������
typedef int (*funcPtr)(int, int);
int main() 
{
    // ���ض�̬��
#ifdef _WIN32
    HINSTANCE hDll = LoadLibrary("zhongkeAI.dll");
#else
    void* handle = dlopen("./zhongkeAI.so", RTLD_LAZY);
#endif
    if (!hDll) 
    {
        return -1;//dll����ʧ��
    }
    // ��ȡ������ĺ���ָ��
    funcPtr func_add = (funcPtr)GetProcAddress(hDll, "add");
    if (!func_add) 
    {
        // �����ȡ����ָ��ʧ�ܵ����
        return -2;
    }
    // ���õ�����ķ���
    int result = func_add(42,28);

    // ��ӡ���
    printf("�ӷ������%d\n", result);
    system("pause");
    // ж�ض�̬��
#ifdef _WIN32
    FreeLibrary(hDll);
#else
    dlclose(handle);
#endif

    return 0;
}