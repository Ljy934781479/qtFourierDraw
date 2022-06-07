#include"../_ClassHpp/fftAnalysis.hpp"
#include <string>
CFFT::CFFT(Cpx* l, int listSize,int countCirc)
{
	_countCirc = countCirc;
	_listSize = listSize;
	_list = l;
	_bmp = new CReadBmp();
	_RealDomain = new CReadBmp();
}

CFFT::~CFFT()
{
	delete _bmp;
	delete _RealDomain;
}

int CFFT::draw2d()
{
	Cpx* pre = _list = NULL;
	_listSize = 0;
	//假设这个是系数
	//Cpx qq = getCpx(1, -2 * PI * 0);
	Cpx qq(2,0);
	for (double i = 0.0; i <= 1; i += 0.01)
	{
		Cpx c = euler(1 * -2 * PI * i);
		c = c * qq;
		drawCpx(c, false,RGB(255,70,15),_bmp);
		//if (i >= 0 && i <= 0.125)
		//	drawCpx(d, false, RED);
		//else
		//	drawCpx(d, false, BLACK);
		_RealIvalue.push_back(c.i);
		Cpx* node = new Cpx(c.r, c.i);
		_listSize++;
		if (!_list)
			pre = _list = node;
		else
		{
			pre->next = node;
			pre = node;
		}
	}
	//drawPhoto(1);
	return 0;
}

Cpx CFFT::euler(double i)
{
	Cpx res(cos(i), sin(i));
	return res;
}

Cpx CFFT::mul2(Cpx& c, double i)
{
	Cpx res(c.r * i, c.i * i);
	return res;
}

Cpx* CFFT::integral(int k)
{//链表的每个节点值相当于f(t)
	double dt = 1.0 / _listSize;
	Cpx* l = _list;
	Cpx* ck = new Cpx(0,0);
	for (double i = 0.0; i <= 1; i += dt)
	{
		Cpx add(l->r, l->i);//f(t)
		add = add * euler(k * -2 * PI * i);//e^-2Πkt
		add = mul2(add,dt);//*dt
		*ck = *ck + add;//∫积分
		l = l->next;//指向下一个f(t)
		if (!l)
			break;
	}
	return ck;
}

Cpx CFFT::getCpx(double len, double arc)
{
	double r = cos(arc);
	double i = sin(arc);
	return Cpx(r * len, i*len);
}

Cpx CFFT::mul(Cpx& c, tagMart& m)
{
	Cpx res;
	res.r = c.r * m.a + c.i * m.c;
	res.i = c.r * m.b + c.i * m.d;
	return res;
}

int CFFT::setPix2bmp(COLOR color, int x, int y)
{
	_bmp->setPixel(color, x, y);
	_RealDomain->setPixel(color, x, y );
	return 0;
}

void CFFT::work(int w,int h)
{
	_bmp->creatNULLfile(w, h, RGB(255, 255, 255));
	_RealDomain->creatNULLfile(w, h, RGB(255, 255, 255));
	_midx = w / 2;
	_midy = h / 2;
	creatCoord(COORD_RATIO);
	_ratio = COORD_RATIO;
	//draw2d();
	drawPhoto(_countCirc);
	string str = "d:\\CodeWork\\";
	str += to_string(_countCirc) + ".bmp";
	_bmp->save24bit(str.c_str());
}

int CFFT::creatCoord(double ratio)
{
	int h = _bmp->getHigh();
	int w = _bmp->getWidth();
	_midx = w / 2;
	_midy = h / 2;
	_ratio = ratio;
	creatAxisRow();
	creatAxisCol();
	//平面图过原点的
	for (int y = 0; y < h; y++)
		_bmp->setPixel(RED, _midx, y);
	for (int x = 0; x < w; x++)
		_bmp->setPixel(RED, x, _midy);
	//时域图过原点的
	for (int y = 0; y < h; y++)
		_RealDomain->setPixel(RED, _exUseless, y);
	for (int x = _exUseless; x < w; x++)
		_RealDomain->setPixel(RED, x, _midy);
	return 0;
}

int CFFT::creatAxisRow()
{
	int h = _bmp->getHigh();
	int w = _bmp->getWidth();
	for (int y = _midy + _ratio; y < h; y += _ratio)
		for (int x = 0; x < w; x++)
			setPix2bmp(BLACK, x, y);
	for (int y = _midy - _ratio; y >= 0; y -= _ratio)
		for (int x = 0; x < w; x++)
			setPix2bmp(BLACK, x, y);
	return 0;
}

int CFFT::creatAxisCol()
{
	int h = _bmp->getHigh();
	int w = _bmp->getWidth();
	int count = 0;
	for (int x = _midx + _ratio; x<w;x+= _ratio ,count++)
		for (int y = 0; y < h; y++)
			setPix2bmp(BLACK, x, y);
	for (int x = _midx; x >= 0; x -= _ratio)
		for (int y = 0; y < h; y++)
			setPix2bmp(BLACK, x, y);
	_exUseless = w - (count * _ratio);
	return 0;
}

int CFFT::drawCpx(Cpx& c, bool bLineto, COLOR color, CReadBmp* bmp, int x, int y)
{
	if (!bLineto)
	{
		pair<int, int> p = getPixPos(&c);
		drawPoint(color, bmp, x + p.first, y + p.second,0);
	}
	else
	{
		lineTo(x, y, c, color, bmp);
	}
	return 0;
}

int CFFT::lineTo(int x, int y, Cpx& c, COLOR col, CReadBmp* bmp)
{
	//lineto的时候要保持这个比例
	double ratio = abs(c.i) / abs(c.r);
	if (ratio < 1)
		return -1;
	int ofx = c.r > 0 ? 1 : -1;
	int ofy = c.i > 0 ? 1 : -1;
	int prey = y;
	int endx = x + c.r * _ratio;
	int len = c.r * _ratio;
	for (int x1 = x; abs(x1-endx)<= len; x1 += ofx)
	{
		int h = abs(x1 - x) * ratio;//根据X的偏移量求出Y的偏移量
		int ey = (ofy == 1) ? y - h : y + h;
		for (int dy = ey; abs(dy-prey); dy += ofy)
			bmp->setPixel(col, x1, dy);
		prey = ey;
	}
	return 0;
}

int CFFT::drawPoint(COLOR color,CReadBmp* bmpFile, int x, int y, int exSize)
{
	for (int i = x - exSize; i <= x + exSize; i++)
		for (int j = y - exSize; j <= y + exSize; j++)
			bmpFile->setPixel(color, i, j);
	return 0;
}

Cpx CFFT::euler2(double i, int k)
{
	return Cpx(cos(i*k), sin(i*k));
}

Cpx CFFT::euler3(double t, int k)
{
	return Cpx(cos(-2 * k * PI * t), sin(-2 * k * PI * t ));
}

int CFFT::drawRealDomain()
{
	//(可用横轴像素数/一共需要的点数)即为为每个点分配的大小,方便可视化
	double tNodesize = (_bmp->getWidth() - 2 * _exUseless) / (TIME_LEN / TIME_DX);
	//如果画的周期很多,微分很多,最小点数为1
	int nodeSize = (tNodesize >= 1) ?  tNodesize : 1;
	//尽可能的要在图像宽度内展示完.如果微分的量太小数量太多,那么就忽略掉其中一些值不显示了
	int ratioValue = 1 / tNodesize;
	int pointmidX = _exUseless + nodeSize;
	int prey = -1;
	//一个周期需要用到的像素数目
	int roundPixSize = (360 / TIME_DX);
	//浮点数有偏差,计算周期去模不好使
	int curOff = 0;
	for (int i = 0; i < _RealIvalue.size(); i+= ratioValue, curOff+= ratioValue)
	{
		int y = _midy + (_RealIvalue[i] * -1) * _ratio;
		drawPoint(RGB(0, 0, 255), _RealDomain, pointmidX,y, nodeSize);
		if (prey != -1 && abs(y - prey) > 2 * nodeSize)
		{//这里是为了尽可能的看起来圆滑
			int y2 = (y > prey) ? y - nodeSize : y+ nodeSize;
			int ofy = (y > prey) ? -1 : 1;
			for (; y2 != prey; y2 += ofy)
				for (int i = pointmidX - nodeSize; i < pointmidX + nodeSize; i++)
					_RealDomain->setPixel(RGB(0, 0, 255),i,y2);
		}
		if ( abs(curOff - roundPixSize) <= ratioValue)
		{//一个周期,再画一条竖线显眼一下
			for(int ty = y-_ratio;ty<y+_ratio;ty++)
				_RealDomain->setPixel(RGB(255, 0, 0), pointmidX, ty);
			curOff = 0;
		}
		pointmidX += nodeSize;
		prey = y;
	}
	return 0;
}


Cpx CFFT::Myexp(Cpx& c)
{
	Cpx res(exp(c.r)*cos(c.i), exp(c.r)*sin(c.i));
	return res;
}

pair<int, int> CFFT::getPixPos(Cpx* c)
{
	int x = _midx + c->r * _ratio;
	int y = _midy + c->i * _ratio * -1;
	return pair<int, int>(x,y);
}

int CFFT::drawPhoto(int n)
{
	Cpx* list = NULL;//周转圆系数链表
	Cpx* t = NULL;
	for (int k = -n; k <= n; k++)
		if(!list)
			list = t = integral(k);
		else
		{
			t->next = integral(k);
			t = t->next;
		}
	//开始画图
	double dt = 1.0 / _listSize;
	for (double t = 0.0; t < 1; t += dt)
	{
		int k = -n;
		Cpx sum(0, 0);
		Cpx* c = list;
		while (c)
		{
			sum = sum + (*c) * euler(k * -2 * PI * t);
			k++;
			c = c->next;
		}
		drawCpx(sum, false, GREEN, _bmp);
		_progress++;
	}
	delList(list);
	return 0;
}

bool CFFT::delList(Cpx* p)
{
	Cpx* n = p->next;
	if (p)
		delete p;
	while (n)
	{
		Cpx* del = n;
		n = n->next;
		delete del;
	}
	return false;
}
