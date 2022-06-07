#pragma once
#include"../myDefine.h"
#include<QPixmap>
#include<QtMath>
#include <QtCore/qmath.h>
#include<cmath>

struct tagMart
{//a,b是一个基  c,d是一个基
	double a;
	double b;
	double c;
	double d;
	tagMart() : a(0), b(0), c(0), d(0) {}
	tagMart(double _a, double _b, double _c, double _d) : a(_a), b(_b), c(_c), d(_d) {}
};

// 定义一个复数结构体和复数运算法则
struct Cpx 
{
	double r, i,len;
	Cpx* next;
	Cpx() : r(0), i(0), len(0) { next = NULL; }
	Cpx(double _r, double _i)
	{
		r = _r; i = _i;
		len = sqrt(abs(pow(r, 2)) + abs(pow(i, 2)) );
		next = NULL;
	}


	Cpx operator + (Cpx a)
	{
		return Cpx(a.r + r, a.i + i);
	}
	Cpx operator - (Cpx a)
	{
		return Cpx(r - a.r, i - a.i);
	}
	Cpx operator * (Cpx a)
	{
		return Cpx(a.r *  r - a.i *  i, a.r *  i + a.i *  r);
	}
	Cpx operator / (Cpx a)
	{
		Cpx res;
		res.r = (a.r *  r + a.i *  i) / ( a.r *  a.r +  a.i *  a.i);
		res.i = (i*a.r - r*a.i) / (a.r * a.r + a.i * a.i);
		return res;
	}
};

struct tagHsv
{
	double h, s, v;
	tagHsv() : h(0), s(0), v(0) {}
	tagHsv(int r, int g, int b)
	{
		double r1 = r / 255.0;
		double g1 = g / 255.0;
		double b1 = b / 255.0;
        double maxV = qMax(r1, qMax(g1, b1));
        double minV = qMin(r1, qMin(g1, b1));
		double dt = maxV - minV;
		v = maxV;
		s = maxV ? (dt / maxV) : 0;
		if (dt == 0)
			h = 0;
		else if (maxV == r1)
			h = 60 * (g1 - b1) / dt;
		else if (maxV == g1)
			h = 60 * (b1 - r1) / dt + 2;
		else if (maxV == b1)
			h = 60 * (r1 - g1) / dt + 4;
	}
    tagHsv(QRgb col)
	{
        new(this) tagHsv(qRed(col), qGreen(col), qBlue(col) );
	}
};
