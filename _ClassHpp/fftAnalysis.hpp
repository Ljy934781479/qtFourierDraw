#pragma once

#include<cmath>
#include<vector>
#include<cmath>
#include "readBmp.hpp"
#include"../_ClassHpp/struct.hpp"
#include"../myDefine.h"
using namespace std;

#define PI 3.14159265359
#define TIME_LEN 6*361  //时域总长度
#define TIME_DX 0.01	//时域微分
#define POINT_RATIO 1 //画一个点用的比例
#define COORD_RATIO 1 //复平面上单位1,用多少个像素画
//#define COUNT_CIRC  250 //周转圆个数*2

class CFFT
{
public:
	CFFT(Cpx* l, int listSize,int countCirc);
	~CFFT();
	Cpx Myexp(Cpx& c);
	//根据复数的大小,返回实际图像中的像素位置
	pair<int,int> getPixPos(Cpx* c);
	//积分，系数
	Cpx* integral(int k);
	//假设我想让欧拉单位圆模长放大K倍,从x弧度开始. 那么参数就是(k,x)
	Cpx getCpx(double len,double arc);
	Cpx mul(Cpx& c, tagMart& m);
	//复数乘实数
	Cpx mul2(Cpx& c, double i);
	int setPix2bmp(COLOR color, int x, int y);
	//角度转弧度
	double angToradian(double ang) { return (PI/180.0)*ang; };
	void work(int w, int h);
	//建立坐标系
	int creatCoord(double ratio);//缩放比例(1单位等于多少像素)
	int creatAxisRow();
	int creatAxisCol();
	//画一个复数
	int drawCpx(Cpx& c, bool bLineto, COLOR color, CReadBmp* bmp,int x = 0, int y = 0);
	int lineTo(int x,int y,Cpx& c, COLOR col,CReadBmp* bmp);
	//画一个点,因为比列关系,一个像素点表示一个f(t)很有可能看不清,所以会放大
	int drawPoint(COLOR color, CReadBmp* bmpFile,int x,int y,int exSize);
	//定义欧拉公式
	Cpx euler(double i);
	Cpx euler2(double i, int k);
	Cpx euler3(double t, int k);
	//画时域图像
	int drawRealDomain();
	//画平面图像
	int draw2d();
	//画自定义图像
	int drawPhoto(int n);
	bool delList(Cpx* p);
private:
	Cpx* _list = NULL;
	int _listSize = 0;
	//时域图像中的各个f(x)
	VEC(double) _RealIvalue;
	CReadBmp* _bmp;
	CReadBmp* _RealDomain;
	double _ratio = 1;
	int _midx = 0;
	int _midy = 0;
	int _exUseless = 0;//坐标系多余部分
	int _progress = 0;//画图进度
	int _w = 0;
	int _h = 0;
	int _countCirc = 0;
};
