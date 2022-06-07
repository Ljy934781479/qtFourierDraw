#ifndef _H
#define _H

#include<QWidget>
#include<QFileDialog>
#include<QMessageBox>
#include<QLabel>
#include<QPixmap>
#include<QScreen>
#include<QLineEdit>
#include<stack>
#include"myDefine.h"
#include"_ClassHpp/struct.hpp"
#include"_GuiClassHpp/clockDemo.hpp"

#define PI 3.14159265359

using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class FourierWidget; }
QT_END_NAMESPACE

class  FourierWidget : public QWidget
{
    Q_OBJECT

public:
    FourierWidget(QWidget *parent = nullptr);
    ~FourierWidget();

private slots:
    void on_pbt1_clicked();
    void on_pbt2_clicked();
    void on_pbt3_clicked();

private:
    Cpx mul2(Cpx& c, double i);
    Cpx euler(double i);
    Cpx* integral(int k,Cpx* ftList,int listSize);

    void hideControl();
    int discernBmp(double n, int k);
    int animation(int k, Cpx* ftList, int size);
    void guiWindow(vector<Cpx*>& vL,vector<Cpx*>& vR);
    void closeEvent(QCloseEvent* event);
private:
    int clear();
    Cpx* newCpx(pair<int, int>& p);
    int transX(int x) { return x - _midx; };
    int transY(int y) { return _midy - y; };
    bool isClosePix(int x1, int y1, int x2, int y2);
    void setPixel(int x,int y,QRgb col);
    QRgb getPix(int x,int y){return _img->pixel(x,y);};
    int colorSet(QRgb target, QRgb toSet,int mode = 0);
    bool binaryzation(QRgb l, QRgb r, double vHsv);
    int setOutline(int& bx,int& by,QRgb need, QRgb background, QRgb set);
    Cpx* creatOutlineList(int bx,int by,int& resSize);
    int getScreenRect();
    int whenOpenFile();//打开文件后做一些自适应调整.
private:
    QLabel* _photoLabel = NULL;
    QImage* _img = NULL;
    Ui::FourierWidget *ui;
    vector<Cpx*> _vL;
    vector<Cpx*> _vR;
    int _midx = 0;
    int _midy = 0;
    AnalogClockWindow* _clock = NULL;
};
#endif // _H
