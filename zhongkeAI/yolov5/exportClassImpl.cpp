#include "pch.h"
#include "GreatechAI.h"
extern "C"
{
	CLASS_DECLSPEC GreatechAI* createExportClass()
	{
		return new GreatechAI();//����һ��GreatechAI������
	}
	CLASS_DECLSPEC void destroyExportClass(GreatechAI* ptr)
	{
		if (!ptr)
		{
			std::cout << "�ö���ָ��Ϊ�գ��޷�ɾ��." << std::endl;
		}
		delete ptr;
	}
}