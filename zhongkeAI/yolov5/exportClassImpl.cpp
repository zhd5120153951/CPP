#include "pch.h"
#include "GreatechAI.h"
extern "C"
{
	CLASS_DECLSPEC GreatechAI* createExportClass()
	{
		return new GreatechAI();//创建一个GreatechAI导出类
	}
	CLASS_DECLSPEC void destroyExportClass(GreatechAI* ptr)
	{
		if (!ptr)
		{
			std::cout << "该对象指针为空，无法删除." << std::endl;
		}
		delete ptr;
	}
}