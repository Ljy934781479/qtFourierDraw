#include"../myDefine.h"
#include"../_ClassHpp/readBmp.hpp"

CReadBmp::CReadBmp()
{
}

CReadBmp::~CReadBmp()
{
	Cpx* l = _OutlineList;
	Cpx* Pointer = NULL;
	while (NULL != l)
	{
		Pointer = l;
		l = l->next;  // 下一个节点
		free(Pointer);
	}
	free(_Buffer);
}

int CReadBmp::colorSet(COLOR target, COLOR toSet, int mode)
{
	for (int y = 0; y < getHigh(); y++)
		for (int x = 0; x < getWidth(); x++)
			if ( mode && getPixel(x, y) == target)
				setPixel(toSet, x, y);
			else if(!mode && getPixel(x, y) != target)
				setPixel(toSet, x, y);
	return 0;
}

bool CReadBmp::isClosePix(int x1, int y1, int x2, int y2)
{
	if (abs(x1 - x2) == 1 || abs(y1 - y2) == 1)
		return true;
	return false;
}

bool CReadBmp::readFile(const wchar_t* pFileName)
{
	FILE* file;
	long dpixeladd;
	if (_Buffer) {
		free(_Buffer);
		_Buffer = NULL;
	}
	if (_wfopen_s(&file, pFileName, L"rb")) 
		return 0;
	fread(&_bfh, sizeof(BITMAPFILEHEADER), 1, file);
	if (_bfh.bfType != 0x4D42) {
		fclose(file);
		return 0;
	}
	fread(&_bih, sizeof(BITMAPINFOHEADER), 1, file);

	dpixeladd = _bih.biBitCount / 8;
	_LineByteWidth = _bih.biWidth * (dpixeladd);
	if ((_LineByteWidth % 4) != 0)
		_LineByteWidth += 4 - (_LineByteWidth % 4);

	if ((_Buffer = (BYTE*)malloc(sizeof(BYTE) * _LineByteWidth * _bih.biHeight)) != NULL)
	{
		fread(_Buffer, _LineByteWidth * _bih.biHeight, 1, file);
		fclose(file);
		_midx = _bih.biWidth / 2;
		_midy = _bih.biHeight / 2;
		return 1;
	}
	fclose(file);
	return 0;
}

int CReadBmp::saveGray(const char* path, RECT* rc)
{
	int l = 0, t = 0, w = _bih.biWidth, h = _bih.biHeight;
	if (rc)
	{
		l = rc->left;
		w = rc->right - rc->left+1;
		t = rc->top;
		h = rc->bottom - rc->top+1;
	}
	FILE* BinFile;
	BITMAPFILEHEADER FileHeader;
	BITMAPINFOHEADER BmpHeader;
	int i, extend;
	bool Suc = true;
	BYTE p[4];
	BYTE* ex = NULL;
	extend = (w + 3) / 4 * 4 - w;

	if (extend)
	{
		ex = (BYTE*)malloc(extend);
		memset(ex, 0, extend);
	}
	// Open File
	if (fopen_s(&BinFile,path, "w+b"))
		return false;
	//参数填法见结构链接
	FileHeader.bfType = ((WORD)('M' << 8) | 'B');
	FileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + 256 * 4L;//2个头结构后加调色板
	FileHeader.bfSize = FileHeader.bfOffBits + (w + extend) * h;
	FileHeader.bfReserved1 = 0;
	FileHeader.bfReserved2 = 0;
	if (fwrite((void*)&FileHeader, 1, sizeof(FileHeader), BinFile) != sizeof(FileHeader)) Suc = false;
	// Fill the ImgHeader
	BmpHeader.biSize = 40;
	BmpHeader.biWidth = w;
	BmpHeader.biHeight = h;
	BmpHeader.biPlanes = 1;
	BmpHeader.biBitCount = 8;
	BmpHeader.biCompression = 0;
	BmpHeader.biSizeImage = 0;
	BmpHeader.biXPelsPerMeter = 0;
	BmpHeader.biYPelsPerMeter = 0;
	BmpHeader.biClrUsed = 0;
	BmpHeader.biClrImportant = 0;
	if (fwrite((void*)&BmpHeader, 1, sizeof(BmpHeader), BinFile) != sizeof(BmpHeader)) Suc = false;
	// 写入调色板
	for (i = 0, p[3] = 0; i < 256; i++)
	{
		p[0] = p[1] = p[2] = i;
		if (fwrite((void*)p, 1, 4, BinFile) != 4) { Suc = false; break; }
	}

	//write data
	for (int rowId = 0; rowId < h; rowId++)
	{
		int y = t+h-1 - rowId;
		//一行的数据
		BYTE* rowByte = (BYTE*)malloc(w);
		for (int xid = l; xid < l+w; xid++)
		{
			COLOR col = getPixel(xid, y);
			int b = GetBValue(col);   //Blue
			int g = GetGValue(col);   //Green
			int r = GetRValue(col);   //Red
            rowByte[xid - l] = (b+g+r)/3;
		}
		//写入真实的数据
		if (fwrite((void*)rowByte, 1, w, BinFile) != (unsigned int)w)
			Suc = false;
		free(rowByte);
		//扩充的数据 这里填充0
		if (extend)
			if (fwrite((void*)ex, 1, extend, BinFile) != 1)
				Suc = false;
	}
	fclose(BinFile);
	if (extend)
		free(ex);
	return Suc;
}

int CReadBmp::save24bit(const char* path, RECT* rc)
{
	int l = 0, t = 0, w = _bih.biWidth, h = _bih.biHeight;
	if (rc)
	{
		l = rc->left;
		w = rc->right - rc->left + 1;
		t = rc->top;
		h = rc->bottom - rc->top + 1;
	}
	// Define BMP Size
	int rowByte = 0;
	if ((w * 3 % 4) != 0)
		rowByte = (w * 3) + 4 - (w * 3) % 4;
	else
		rowByte = w * 3;

	const int size = rowByte * h;
	// Part.1 Create Bitmap File Header
	BITMAPFILEHEADER fileHeader;
	fileHeader.bfType = 0x4D42;
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	// Part.2 Create Bitmap Info Header
	BITMAPINFOHEADER bitmapHeader = { 0 };
	bitmapHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapHeader.biHeight = h;
	bitmapHeader.biWidth = w;
	bitmapHeader.biPlanes = 3;
	bitmapHeader.biBitCount = 24;
	bitmapHeader.biSizeImage = size;
	bitmapHeader.biCompression = 0; //BI_RGB
	// Part.3 Create Data
	BYTE* bits = NULL;
	if (rc)
	{
		bits = (BYTE*)malloc(size);
		memset(bits, 0x00, size);
		for (int y = t; y < t + h; y++)
		{
			for (int x = l; x < l + w; x++)
			{
				COLOR col = getPixel(x, y);
				//求该像素点在内存中的位置
				int a = h - (y - t) - 1;
				int index = (x - l) * 3;
				bits[a * rowByte + index + 0] = GetBValue(col);   //Blue
				bits[a * rowByte + index + 1] = GetGValue(col);   //Green
				bits[a * rowByte + index + 2] = GetRValue(col);   //Red
			}
		}
	}
	else
		bits = _Buffer;
	// Graph
	// Write to file
	FILE* output = NULL;
	fopen_s(&output, path, "wb");
	if (output == NULL)
		printf("Cannot open file!\n");
	else
	{
		fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, output);
		fwrite(&bitmapHeader, sizeof(BITMAPINFOHEADER), 1, output);
		fwrite(bits, size, 1, output);
		fclose(output);
	}
	if (rc)
	{
		memset(bits, 0x00, size);
		free(bits);
		bits = NULL;
	}
	return 0;
}

COLOR CReadBmp::getPixel(int x, int y)
{
	if (x < 0 || x >= _bih.biWidth || y < 0 || y >= _bih.biHeight)
		return -1;
	BYTE* ptr = _Buffer + (_bih.biHeight - y - 1) * _LineByteWidth + x * 3;
	if (_bih.biBitCount == 32) 
		return *(unsigned long*)ptr;
	return RGB(*(ptr + 2), *(ptr + 1), *ptr);
}

bool CReadBmp::setPixel(COLOR color, int x, int y)
{
	int dpixeladd;
	BYTE* ptr;
	if (x < 0 || x >= _bih.biWidth || y < 0 || y >= _bih.biHeight)
		return false;
	ptr = _Buffer + (_bih.biHeight - y - 1) * _LineByteWidth + x * 3;
	dpixeladd = _bih.biBitCount / 8;
	if (dpixeladd == 3) 
	{
		*ptr = GetBValue(color);
		*(ptr+1) = GetGValue(color);
		*(ptr+2) = GetRValue(color);
	}
	else
		*(unsigned long*)ptr = color;
	return true;
}

bool CReadBmp::creatNULLfile(int w, int h, COLOR color)
{
	// Define BMP Size
	int rowByte = 0;
	if ((w * 3 % 4) != 0)
		rowByte = (w * 3) + 4 - (w * 3) % 4;
	else
		rowByte = w * 3;
	const int size = rowByte * h;
	_LineByteWidth = rowByte;
	// Part.1 Create Bitmap File Header
	_bfh.bfType = 0x4D42;
	_bfh.bfReserved1 = 0;
	_bfh.bfReserved2 = 0;
	_bfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size;
	_bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	// Part.2 Create Bitmap Info Header
    BITMAPINFOHEADER bitmapHeader = { 0 };
	_bih.biSize = sizeof(BITMAPINFOHEADER);
	_bih.biHeight = h;
	_bih.biWidth = w;
	_bih.biPlanes = 3;
	_bih.biBitCount = 24;
	_bih.biSizeImage = size;
	_bih.biCompression = 0; //BI_RGB
	_Buffer = (BYTE*)malloc(size);
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{//求该像素点在内存中的位置
			int a = h - y - 1;
			int index = x * 3;
			if (y == 0 || y == h - 1 || x == 0 || x == w - 1)
			{
				_Buffer[a * rowByte + index + 0] = 0;   //Blue
				_Buffer[a * rowByte + index + 1] = 0;   //Green
				_Buffer[a * rowByte + index + 2] = 0;   //Red
			}
			else
			{
				_Buffer[a * rowByte + index + 0] = GetBValue(color);   //Blue
				_Buffer[a * rowByte + index + 1] = GetGValue(color);   //Green
				_Buffer[a * rowByte + index + 2] = GetRValue(color);   //Red
			}
		}
	}
	return false;
}

char* CReadBmp::wcharTochar(wchar_t* p)
{//用完这个函数之后要释放掉返回的结果
	size_t len = wcslen(p) + 1;
	size_t converted = 0;
	char* res = (char*)malloc(len * sizeof(char));
	wcstombs_s(&converted, res, len, p, _TRUNCATE);
	return nullptr;
}

wchar_t* CReadBmp::charTowchar(char* p)
{//用完这个函数之后要释放掉返回的结果
	size_t len = strlen(p) + 1;
	size_t converted = 0;
	wchar_t* res = (wchar_t*)malloc(len*sizeof(wchar_t));
	mbstowcs_s(&converted, res, len, p, _TRUNCATE);
	return res;
}

Cpx* CReadBmp::newCpx(pair<int, int>& p)
{
	return new Cpx(transX(p.first), transY(p.second)); 
}

void CReadBmp::binaryzation(COLOR l, COLOR r, double vHsv)
{
	for (int i = 0; i < getHigh(); i++)
		for (int j = 0; j < getWidth(); j++)
		{
			tagHsv t(getPixel(j, i));
			if (t.v < vHsv)
				setPixel(l, j, i);
			else
				setPixel(r, j, i);
		}
}

int CReadBmp::setOutline(COLOR need, COLOR background, COLOR set)
{
	int x = 0, y = 0;
	for (int i = 0; i < getHigh(); i++)
		for (int j = 0; j< getWidth(); j++)
		{
			if (getPixel(j, i) != need)
				continue;
			if (_beginX == -1)//定死遇到的第一个为轮廓开始点,以免后续分析问题摇摆太多
			{
				_beginX = j;
				_beginY = i;
			}
			bool bfind = false;
			//如果周围八个点有背景色就视为是轮廓点
			for (int y1 = -1; y1 < 2 && !bfind; y1++)
				for (int x1 = -1; x1 < 2 && !bfind; x1++)
					if (getPixel(j - x1, i - y1) == background) {
						setPixel(set, j, i);
						x = j;
						y = i;
						bfind = true;
					}
		}
	return 0;
}

int CReadBmp::creatOutlineList()
{
	save24bit("d:\\CodeWork\\轮廓识别结果.bmp");
	colorSet(BLUE, WHITE, 0);
	//存在出度不止一个的情况下,只走一个先
	bool bOkpath = false;
	int curx = _beginX;
	int cury = _beginY;
	int nextx = 0;
	int nexty = 0;
	int count = 0;
	setPixel(GREEN, curx, cury);
	//用一个栈记录所有多出度的点
	stack<pair<int, int> > staNode;
	//用一个栈记录路径
	stack<pair<int, int> > staPath;
	staPath.push(PAIR_2INT(curx, cury));
	auto lmb = [&](int x, int y) {
		if (getPixel(x, y) == BLUE){
			count++;
			if (!bOkpath) {
				staPath.push(PAIR_2INT(x, y));
				setPixel(GREEN, x, y);
				bOkpath = true;
				nextx = x; nexty = y;
			}
		}
	};
	while (1)
	{//检查周围八个点
		for(int i = 0;i<9;i++)//这里还是能优化的? 多出度的点回溯经历这里就有重复操作了.如果用map记录每个点的出度,这样优化会明显吗? 空间肯定要牺牲了
			lmb(curx+(i%3)-1, cury + i/3 -1);
		if (count == 0){//如果没有出度并且临近最开始的点,则完成
			if(isClosePix(curx,cury,_beginX,_beginY))
				break;
			//维护双栈,并且涂白放弃这条线路,直到回到上一个多出度的点
			while ( staPath.top().first != staNode.top().first
				|| staPath.top().second != staNode.top().second)
			{
				pair<int, int>& del = staPath.top();
				staPath.pop();
				setPixel(WHITE, del.first, del.second);
			}
			//裁剪完成之后以当前节点重新寻找,清理，不在认为他是多出度的点
			curx = staPath.top().first;
			cury = staPath.top().second;
			staNode.pop();
		}
		else
		{
			if (count > 1)
				staNode.push(pair<int, int>(curx, cury));
			curx = nextx;
			cury = nexty;
			count = 0;
			bOkpath = false;
		}
	}
	//创建链表
	Cpx* l = newCpx(staPath.top());
	_listSize++;
	staPath.pop();
	while (staPath.size())
	{
		Cpx* tt = newCpx(staPath.top());
		_listSize++;
		tt->next = l;
		l = tt;
		staPath.pop();
	}
	_OutlineList = l;
	colorSet(BLUE, WHITE, 1);
	save24bit("d:\\CodeWork\\轮廓链表建立结果.bmp");
	return 0;
}

bool CReadBmp::isOutdegree(int x, int y,COLOR need)
{
	if (getPixel(x, y) == need)
		return true;
	return false;
}

Cpx* CReadBmp::getList()
{
	return _OutlineList;
}

int CReadBmp::getListSize()
{
	return _listSize;
}
