#include <vector>
#include <string>

// 此处为热更函数包含的头文件
#include <cstdio>
#include "patch_wind.h"

extern "C"
{

// 先编译patch_test，nm patch_test | grep GlobalFunc来获取函数的名字

typedef std::vector<std::string> FUNC_NAME_LIST;
void get_patch_func_name(FUNC_NAME_LIST &func_name_list)
{
	func_name_list.push_back("_Z10GlobalFuncii");
	func_name_list.push_back("_ZN9PatchWind10StaticFuncEii");
	func_name_list.push_back("_ZN9PatchWind10MemberFuncEdd");
	func_name_list.push_back("_ZN9PatchWind17PrivateMemberFuncEv");
}


/* 此处以下为热更函数 */

int _Z10GlobalFuncii(int a, int b)
{
	return a + b + 1000;
}

int _ZN9PatchWind10StaticFuncEii(int a, int b)
{
	return a + b + 1000;
}

double _ZN9PatchWind10MemberFuncEdd(PatchWind *__this, double a, double b)
{
	if (__this->PrivateMemberFunc())
	{
		return a + b;
	}
	else
	{
		return a + b + 1000;
	}
}

bool _ZN9PatchWind17PrivateMemberFuncEv(PatchWind *__this)
{
	return (__this->m_data < 0);
}

}
