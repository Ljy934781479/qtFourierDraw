#include "fourierwidget.h"
#include "ui_fourierwidget.h"
#include"_ClassHpp/struct.hpp"

FourierWidget::FourierWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FourierWidget)
{
    ui->setupUi(this);
    hideControl();
    _img=new QImage;
    _photoLabel = new QLabel();
    getScreenRect();
}

FourierWidget::~FourierWidget()
{
    delete _img;
    delete _photoLabel;
    delete ui;
    delete _clock;
}

void FourierWidget::on_pbt1_clicked()
{
    QLabel* lb = _photoLabel;
    QString filename;
    lb->close();
    filename=QFileDialog::getOpenFileName(this,tr("选择图像"),"",tr("Images (*.bmp)"));//可打开的文件类型
    if(!filename.isEmpty())
    {
        if( 0 == _img->load(filename) ) {//加载图像
            QMessageBox::information(this,tr("打开图像失败"),tr("打开图像失败!"));
            return;
        }
        QPixmap p = QPixmap(filename);
        lb->setPixmap(p);
        lb->resize(p.width(),p.height());
        lb->setFixedSize(p.width(),p.height());
        lb->move(0,0);
        lb->show();
        _midx = _img->width() / 2;
        _midy = _img->height() / 2;
        whenOpenFile();
    }
    else
        hideControl();
}

void FourierWidget::on_pbt2_clicked()
{
    double n = ui->ed1->text().toDouble();
    int k = ui->ed2->text().toInt();
    if (n && k)
        discernBmp(n, k);
    else
        QMessageBox::warning(this, QStringLiteral("Error!"), QStringLiteral("输入不正确。"), QMessageBox::Ok);
}

void FourierWidget::on_pbt3_clicked()
{
    _photoLabel->close();
    _clock->close();
    close();
}

void FourierWidget::closeEvent(QCloseEvent* event)
{
    on_pbt3_clicked();
    event->accept();
}

int FourierWidget::getScreenRect()
{
    QScreen *scr = QGuiApplication::primaryScreen();
    QRect mm = scr->availableGeometry();
    setFixedSize(this->width(), this->height());
    move(mm.width() / 2, mm.height() / 2);
    return 0;
}

int FourierWidget::whenOpenFile()
{
    ui->ed1->show();
    ui->ed2->show();
    ui->ped1->show();
    ui->ped2->show();
    ui->pbt2->show();
    if (_clock)
    {
        _clock->close();
        delete _clock;
    }
    return 0;
}

Cpx FourierWidget::euler(double i)
{
    Cpx res(cos(i), sin(i));
    return res;
}

Cpx FourierWidget::mul2(Cpx& c, double i)
{
    Cpx res(c.r * i, c.i * i);
    return res;
}

void FourierWidget::hideControl()
{
    ui->pbt2->hide();
    ui->ed1->hide();
    ui->ed2->hide();
    ui->ped1->hide();
    ui->ped2->hide();
}

int FourierWidget::discernBmp(double n, int k)
{
    int bx = 0; int by = 0;
    QImage* temp = new QImage(*_img);
    binaryzation(RED,WHITE,n);
    setOutline(bx,by,RED, WHITE, BLUE);
    int size = 0;
    Cpx* l = creatOutlineList(bx,by,size);
    delete _img;
    _img = temp;
    animation(k,l,size);
    guiWindow(_vL, _vR);
    return 0;
}

Cpx* FourierWidget::integral(int k,Cpx* ftList, int listSize)
{//链表的每个节点值相当于f(t)
    double dt = 1.0 / listSize;
    Cpx* l = ftList;
    Cpx* ck = new Cpx(0, 0);
    for (double i = 0.0; i <= 1; i += dt)
    {
        Cpx add(l->r, l->i);//f(t)
        add = add * euler(k * -2 * PI * i);//e^-2Πkt
        add = mul2(add, dt);//*dt
        *ck = *ck + add;//∫积分
        l = l->next;//指向下一个f(t)
        if (!l)
            break;
    }
    return ck;
}

int FourierWidget::animation(int k, Cpx* ftList, int size)
{//周转圆系数
    clear();
    for (int k1 = -1; k1 >= -k; k1--)//从低频开始存
        _vL.push_back(integral(k1, ftList, size));
    for(int k1 = 0;k1<= k;k1++)
        _vR.push_back(integral(k1, ftList, size));
    return 0;
}

void FourierWidget::guiWindow(vector<Cpx*>& vL, vector<Cpx*>& vR)
{
    _clock = new AnalogClockWindow;
    _clock->setPara(&vL, &vR);
    QSize s(_img->width(), _img->height());
    _clock->setMinimumSize(s);
    _clock->setMaximumSize(s);
    _clock->show();
}

Cpx* FourierWidget::creatOutlineList(int bx, int by, int& resSize)
{
    if(bx ==-1 || by ==-1)
        return NULL;
    colorSet(BLUE, WHITE, 0);
    //存在出度不止一个的情况下,只走一个先
    bool bOkpath = false;
    int curx = bx;
    int cury = by;
    int nextx = 0;
    int nexty = 0;
    int count = 0;
    setPixel(curx, cury,GREEN);
    //用一个栈记录所有多出度的点
    stack<pair<int, int> > staNode;
    //用一个栈记录路径
    stack<pair<int, int> > staPath;
    staPath.push(PAIR_2INT(curx, cury));
    auto lmb = [&](int x, int y) {
        if (getPix(x, y) == BLUE){
            count++;
            if (!bOkpath) {
                staPath.push(PAIR_2INT(x, y));
                setPixel(x, y,GREEN);
                bOkpath = true;
                nextx = x; nexty = y;
            }
        }
    };
    while (1)
    {
        //检查正方向
        for(int i = 1;i<9;i+=2)
            lmb(curx+(i%3)-1, cury + i/3 -1);
        //如果没有正方向的出度在检查斜方向
        if(!count)
            for (int i = 0; i < 9; i+=2)
                lmb(curx + (i % 3) - 1, cury + i / 3 - 1);
        if (count == 0){//如果没有出度并且临近最开始的点,则完成
            if(isClosePix(curx,cury, bx, by))
                break;
            //维护双栈,并且涂白放弃这条线路,直到回到上一个多出度的点
            while ( staPath.top().first != staNode.top().first
                || staPath.top().second != staNode.top().second)
            {
                pair<int, int>& del = staPath.top();
                setPixel(del.first, del.second,WHITE);
                staPath.pop();
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
    resSize++;
    staPath.pop();
    while (staPath.size())
    {
        Cpx* tt = newCpx(staPath.top());
        resSize++;
        tt->next = l;
        l = tt;
        staPath.pop();
    }
    colorSet(BLUE, WHITE, 1);
    _img->save("d:\\CodeWork\\outLineList.bmp");
    return l;
}

int FourierWidget::clear()
{
    for (Cpx* d : _vL)
        delete d;
    for (Cpx* d : _vR)
        delete d;
    _vL.clear();
    _vR.clear();
    return 0;
}

Cpx *FourierWidget::newCpx(pair<int, int> &p)
{
    return new Cpx(transX(p.first), transY(p.second));
}

bool FourierWidget::isClosePix(int x1, int y1, int x2, int y2)
{
    if (abs(x1 - x2) <=1 && abs(y1 - y2) <= 1)
        return true;
    return false;
}

void FourierWidget::setPixel(int x, int y,QRgb col)
{
    _img->setPixel(x,y,col);
}

int FourierWidget::colorSet(QRgb target, QRgb toSet, int mode)
{
    for (int y = 0; y < _img->height(); y++)
        for (int x = 0; x < _img->width(); x++)
            if ( mode && _img->pixel(x, y) == target)
                setPixel(x, y,toSet);
            else if(!mode && _img->pixel(x, y) != target)
                setPixel(x, y,toSet);
    return 0;
}

bool FourierWidget::binaryzation(QRgb l, QRgb r, double vHsv)
{
    for (int x = 0; x < _img->width(); x++)
        for (int y = 0; y < _img->height(); y++)
        {
            tagHsv t(_img->pixel(x, y));
            if (t.v < vHsv)
                setPixel(x,y,l);
            else
                setPixel(x,y,r);
        }
    _img->save("d:\\CodeWork\\Binarization.bmp");
    return true;
}

int FourierWidget::setOutline(int& bx, int& by, QRgb need, QRgb background, QRgb set)
{//这个算法好像想得太烂了,以后再优化
    vector<pair<int, int> > v;
     for (int y = 0; y< _img->height(); y++)
        for (int x = 0; x < _img->width(); x++)
        {
            if (_img->pixel(x, y) != need)
                continue;
            //定死遇到的第一个为轮廓开始点,以免后续分析问题摇摆太多
            if (bx == 0)
            {
                bx = x;
                by = y;
            }
            //如果周围八个点有背景色就视为是轮廓点
            bool bfind = false;
            for (int k = 0; k < 9 && !bfind; k++)
                if (_img->pixel(x + (k % 3) - 1, y + k / 3 - 1) == background) {
                    setPixel(x,y,set);
                    bfind = true;
                    v.push_back(pair<int,int>(x, y));
                }
        }
     //第二遍,要求每个点的周围八个点都必须存在背景色和图形色至少一个
     for (auto it : v)
     {
         bool bFindBack = false;
         bool bFindNeed = false;
         for (int k = 0; k < 9 && !(bFindBack && bFindNeed); k++)
             if (_img->pixel(it.first + (k % 3) - 1, it.second + k / 3 - 1) == background)
                 bFindBack = true;
             else if (_img->pixel(it.first + (k % 3) - 1, it.second + k / 3 - 1) == need)
                 bFindNeed = true;
         if (!(bFindBack && bFindNeed))
             setPixel(it.first, it.second,WHITE);
     }
    _img->save("d:\\CodeWork\\outLine.bmp");
    return 0;
}

