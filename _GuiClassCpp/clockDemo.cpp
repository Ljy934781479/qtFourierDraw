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
    setTitle("Analog Clock");
    resize(200, 200);
    m_timerId = startTimer(1);
}


AnalogClockWindow::~AnalogClockWindow()
{
    delete _sumPath;
}

//! [6]
//! [7]
void AnalogClockWindow::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == m_timerId)
        renderLater();
}
void AnalogClockWindow::render2(QPainter* p)
{
 //! [14]
 //! [8]
    static const QPoint hourHand[4] = {
        QPoint(-1, 0),
        QPoint(1, 0),
        QPoint(1, -40),
        QPoint(-1, -40)
    };
    static const QPoint minuteHand[4] = {
        QPoint(-1, 0),
        QPoint(1, 0),
        QPoint(1, -60),
        QPoint(-1, -60)
    };

    static const QPoint secHand[4] = {
        QPoint(-1, 0),
        QPoint(1, 0),
        QPoint(1, -70),
        QPoint(-1, -70)
    };

    QColor hourColor(127, 0, 127);
    QColor minuteColor(0, 127, 127, 191);
    QColor secColor(255, 255, 255);
    //! [8]

    //! [9]
    p->setRenderHint(QPainter::Antialiasing);
    //! [9] //! [10]
    p->translate(width() / 2, height() / 2);

    int side = qMin(width(), height());
    p->scale(side / 200.0, side / 200.0);
    //! [1] //! [10]

    //! [11]
    p->setPen(Qt::NoPen);
    p->setBrush(hourColor);
    //! [11]

    //! [2]
    QTime time = QTime::currentTime();

    p->save();
    p->rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
    p->drawConvexPolygon(hourHand, 4);
    p->restore();
    //! [2]

    //! [12]
    p->setPen(hourColor);

    for (int i = 0; i < 12; ++i) {
        p->drawLine(88, 0, 96, 0);
        p->rotate(30.0);
    }
    //! [12] //! [13]
    p->setPen(Qt::NoPen);
    p->setBrush(minuteColor);
    //! [13]

    //! [3]
    p->save();
    p->rotate(6.0 * (time.minute() + time.second() / 60.0));
    p->drawConvexPolygon(minuteHand, 4);
    p->restore();
    //! [3]

    //! [4]
    p->setPen(minuteColor);

    for (int j = 0; j < 60; ++j) {
        if ((j % 5) != 0) {
            p->drawLine(92, 0, 96, 0);
        }
        p->rotate(6.0);
    }
    //! [4]
    p->setPen(Qt::NoPen);
    p->setBrush(secColor);
    p->save();
    p->rotate(6 * time.second() + 1);
    p->drawConvexPolygon(secHand, 4);
    p->restore();
}
//! [7]

void AnalogClockWindow::render(QPainter* p)
{//用drawPath逐个画每一个向量，动画刷新的时候很刺眼。用它自带的旋转吧,所以下面画的时候要很多转换。。。。
    p->setRenderHint(QPainter::Antialiasing);
    p->translate(width() / 2, height() / 2);
    int offestTime = _beginTime.msecsTo(QTime::currentTime());
    qDebug() << offestTime;
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
        pt.moveTo(sum.r, -sum.i);
        sum = sum + (*c) * euler(oo * -2 * PI * (offestTime/10000.0));
        pt.lineTo(sum.r, -sum.i);
        p->drawPath(pt);
        p->restore();
        k++;
    }
    if (!_count)
    {
        _sumPath->moveTo(sum.r, -sum.i);
        _bx = sum.r;
        _by = sum.i;
    }
    else
    {
        if (sum.r == _bx && sum.i == _by)
            _bOk = true;
        if(!_bOk)
            _sumPath->lineTo(sum.r, -sum.i);
    }
    _count++;
    p->setPen(c2);
    p->save();
    p->drawPath(*_sumPath);
    //复原状态
    p->restore();
}

void AnalogClockWindow::setPara(vector<Cpx*>* vl, vector<Cpx*>* vr)
{
    _beginTime = QTime::currentTime();
    _sumPath = new QPainterPath;
    _vL = vl;
    _vR = vr;
}
