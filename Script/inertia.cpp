#pragma once
#include"inertia.h"
#include"math.h"

inertia::inertia(void)
{}

inertia::~inertia(void)
{}

double inertia::move(double &v, double t, double a)	//º∆À„Œª“∆
{
	double X = 0;
	X=v*t+ 1.0 / 2.0*a*pow(t, 2);
	v = v + a*t;
	return X;
}