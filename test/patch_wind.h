#pragma once

#ifndef PATCH_WIND_H
#define PATCH_WIND_H

extern int GlobalFunc(int a, int b);

class PatchWind
{
public:
	PatchWind() {}
	PatchWind(int data) { m_data = data; }
	virtual ~PatchWind() {}

	static int StaticFunc(int a, int b);
	double MemberFunc(double a, double b);

private:
	bool PrivateMemberFunc();

protected:
	int m_data = 0;
};

#endif
