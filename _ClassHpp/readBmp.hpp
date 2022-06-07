#pragma once
#include<windows.h>
#include<stdio.h>
#include<iostream>
#include<stack>
#include<vector>
#include"struct.hpp"

using namespace std;
typedef COLORREF COLOR;

class CReadBmp
{
public:
    CReadBmp();
    ~CReadBmp();
    int colorSet(COLOR target, COLOR toSet,int mode = 0);
    //,
    bool isClosePix(int x1, int y1, int x2, int y2);
    bool readFile(const wchar_t* pFileName);
    int saveGray(const char* path, RECT* rc = NULL);
    int save24bit(const char* path, RECT* rc = NULL);
    COLOR getPixel(int x, int y);
    bool setPixel(COLOR color, int x, int y);
    bool creatNULLfile(int w,int h,COLOR color);
    char* wcharTochar(wchar_t* p);
    wchar_t* charTowchar(char* p);
    int getHigh() { return _bih.biHeight;};
    int getWidth() { return _bih.biWidth;};
    int transX(int x) { return x - _midx; };
    int transY(int y) { return _midy - y; };
    Cpx* newCpx(pair<int, int>& p);
    //
    void binaryzation(COLOR l, COLOR r, double vHsv);
    //
    int setOutline(COLOR need, COLOR background, COLOR set);
    //,
    int creatOutlineList();
    bool isOutdegree(int x,int y,COLOR need);
    Cpx* getList();
    int getListSize();
private:
    stack<pair<int, int> > _stack;
    int _beginX = -1;
    int _beginY = -1;
    int _listSize = 0;
    Cpx* _OutlineList = NULL;
    int _midx = 0;
    int _midy = 0;
    BITMAPINFOHEADER _bih;
    BITMAPFILEHEADER _bfh;
    BYTE* _Buffer = NULL;
    long _LineByteWidth = 0;
};

