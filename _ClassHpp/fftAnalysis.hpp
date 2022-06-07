#pragma once

#include<cmath>
#include<vector>
#include<cmath>
#include "readBmp.hpp"
#include"../_ClassHpp/struct.hpp"
#include"../myDefine.h"
using namespace std;

#define PI 3.14159265359
#define TIME_LEN 6*361  //ʱ���ܳ���
#define TIME_DX 0.01	//ʱ��΢��
#define POINT_RATIO 1 //��һ�����õı���
#define COORD_RATIO 1 //��ƽ���ϵ�λ1,�ö��ٸ����ػ�
//#define COUNT_CIRC  250 //��תԲ����*2

class CFFT
{
public:
	CFFT(Cpx* l, int listSize,int countCirc);
	~CFFT();
	Cpx Myexp(Cpx& c);
	//���ݸ����Ĵ�С,����ʵ��ͼ���е�����λ��
	pair<int,int> getPixPos(Cpx* c);
	//���֣�ϵ��
	Cpx* integral(int k);
	//����������ŷ����λԲģ���Ŵ�K��,��x���ȿ�ʼ. ��ô��������(k,x)
	Cpx getCpx(double len,double arc);
	Cpx mul(Cpx& c, tagMart& m);
	//������ʵ��
	Cpx mul2(Cpx& c, double i);
	int setPix2bmp(COLOR color, int x, int y);
	//�Ƕ�ת����
	double angToradian(double ang) { return (PI/180.0)*ang; };
	void work(int w, int h);
	//��������ϵ
	int creatCoord(double ratio);//���ű���(1��λ���ڶ�������)
	int creatAxisRow();
	int creatAxisCol();
	//��һ������
	int drawCpx(Cpx& c, bool bLineto, COLOR color, CReadBmp* bmp,int x = 0, int y = 0);
	int lineTo(int x,int y,Cpx& c, COLOR col,CReadBmp* bmp);
	//��һ����,��Ϊ���й�ϵ,һ�����ص��ʾһ��f(t)���п��ܿ�����,���Ի�Ŵ�
	int drawPoint(COLOR color, CReadBmp* bmpFile,int x,int y,int exSize);
	//����ŷ����ʽ
	Cpx euler(double i);
	Cpx euler2(double i, int k);
	Cpx euler3(double t, int k);
	//��ʱ��ͼ��
	int drawRealDomain();
	//��ƽ��ͼ��
	int draw2d();
	//���Զ���ͼ��
	int drawPhoto(int n);
	bool delList(Cpx* p);
private:
	Cpx* _list = NULL;
	int _listSize = 0;
	//ʱ��ͼ���еĸ���f(x)
	VEC(double) _RealIvalue;
	CReadBmp* _bmp;
	CReadBmp* _RealDomain;
	double _ratio = 1;
	int _midx = 0;
	int _midy = 0;
	int _exUseless = 0;//����ϵ���ಿ��
	int _progress = 0;//��ͼ����
	int _w = 0;
	int _h = 0;
	int _countCirc = 0;
};
