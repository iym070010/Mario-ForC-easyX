#pragma once
#include"graphics.h"
#include"mydefine.h"
#include"scene.h"

class inertia	//惯性类
{
	inertia(void);
	~inertia(void);
public:
	static double move(double &v, double t, double a);	//惯性系位移计算
};