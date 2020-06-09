#include "patch_wind.h"

int GlobalFunc(int a, int b)
{
	return a + b;
}

int PatchWind::StaticFunc(int a, int b)
{
	return a + b;
}

double PatchWind::MemberFunc(double a, double b)
{
	if (PrivateMemberFunc())
	{
		return a + b;
	}
	else
	{
		return a + b + 100;
	}
}

bool PatchWind::PrivateMemberFunc()
{
	return (m_data > 0);
}
