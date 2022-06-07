#include"../_GuiclassHpp/clockDemo.hpp"
#include<stack>
#include<cmath>
#include<qDebug>

//弧度转角度
double arcToang(double n)
{
    return (180.0 / PI) * n;
}

Cpx euler(double i)
{
    Cpx res(cos(i), sin(i));
    return res;
}

AnalogClockWindow::AnalogClockWindow()
{
    setTitle("FourierDraw");
    resize(200, 200);
    m_timerId = startTimer(1);
}

AnalogClockWindow::~AnalogClockWindow()
{
    delete _sumPath;
}

void AnalogClockWindow::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_timerId)
        renderLater();
}

void AnalogClockWindow::render(QPainter* p)
{
    p->setRenderHint(QPainter::Antialiasing);
    p->translate(width() / 2, height() / 2);
    int offestTime = _beginTime.msecsTo(QTime::currentTime());
    //清理一下
    if (_clearTime.msecsTo(QTime::currentTime()) > 11000)
    {
        _clearTime = QTime::currentTime();
        _count = 0;
        _sumPath->clear();
    }
    QColor col(255, 0, 0);
    QColor c2(255, 255, 255);
    int k = 0;
    Cpx sum(0, 0);
    auto itr = _vR->begin();
    auto itl = _vL->begin();
    int z1 = 0;
    int z2 = -1;
    int oo = 0;
    while (k < _vR->size() * 2 - 1)
    {
        Cpx* c;
        if (k % 2)
        {
            c = *itl;
            itl++;
            z1--;
            oo = z1;
        }
        else
        {
            c = *itr;
            itr++;
            z2++;
            oo = z2;
        }
        //保存状态
        p->setPen(col);
        p->save();
        QPainterPath pt;
        pt.moveTo(sum.r,-sum.i);
        sum = sum + (*c) * euler(oo * -2 * PI * (offestTime/10000.0));
        pt.lineTo(sum.r,-sum.i);
        p->drawPath(pt);
        p->restore();
        k++;
    }
    if (!_count)
        _sumPath->moveTo(sum.r, -sum.i);
    else
        _sumPath->lineTo(sum.r, -sum.i);
    p->setPen(c2);
    p->save();
    p->drawPath(*_sumPath);
    //复原状态
    p->restore();
    _count++;
}

void AnalogClockWindow::setPara(vector<Cpx*>* vl, vector<Cpx*>* vr)
{
    _clearTime = _beginTime = QTime::currentTime();
    _sumPath = new QPainterPath;
    _vL = vl;
    _vR = vr;
}
