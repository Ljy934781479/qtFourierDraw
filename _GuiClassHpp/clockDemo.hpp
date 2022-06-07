#pragma once
#include<vector>
#include"../_GuiClassHpp/rasterwindow.h"
#include"../_ClassHpp/struct.hpp"

#define PI 3.14159265359
using namespace std;

class AnalogClockWindow : public RasterWindow
{
public:
    AnalogClockWindow();
    ~AnalogClockWindow();
    void setPara(vector<Cpx*>* vl, vector<Cpx*>* vr);
protected:
    void timerEvent(QTimerEvent*) override;
    void render(QPainter* p) override;
private:
    bool _bOk = false;
    int _count = 0;
    QTime _beginTime;
    QTime _clearTime;
    int m_timerId;
    QPainterPath* _sumPath = NULL;
    vector<Cpx*>* _vL;
    vector<Cpx*>* _vR;
};