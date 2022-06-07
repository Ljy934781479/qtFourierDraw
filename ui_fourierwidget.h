/********************************************************************************
** Form generated from reading UI file 'fourierwidget.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FOURIERWIDGET_H
#define UI_FOURIERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FourierWidget
{
public:
    QPushButton *pbt1;
    QPushButton *pbt3;
    QLineEdit *ed1;
    QLineEdit *ed2;
    QPushButton *pbt2;
    QPlainTextEdit *ped1;
    QPlainTextEdit *ped2;

    void setupUi(QWidget *FourierWidget)
    {
        if (FourierWidget->objectName().isEmpty())
            FourierWidget->setObjectName(QString::fromUtf8("FourierWidget"));
        FourierWidget->resize(464, 395);
        pbt1 = new QPushButton(FourierWidget);
        pbt1->setObjectName(QString::fromUtf8("pbt1"));
        pbt1->setEnabled(true);
        pbt1->setGeometry(QRect(20, 340, 131, 31));
        pbt1->setCursor(QCursor(Qt::ArrowCursor));
        pbt3 = new QPushButton(FourierWidget);
        pbt3->setObjectName(QString::fromUtf8("pbt3"));
        pbt3->setGeometry(QRect(290, 340, 101, 31));
        ed1 = new QLineEdit(FourierWidget);
        ed1->setObjectName(QString::fromUtf8("ed1"));
        ed1->setGeometry(QRect(130, 130, 113, 20));
        ed2 = new QLineEdit(FourierWidget);
        ed2->setObjectName(QString::fromUtf8("ed2"));
        ed2->setGeometry(QRect(130, 210, 113, 20));
        pbt2 = new QPushButton(FourierWidget);
        pbt2->setObjectName(QString::fromUtf8("pbt2"));
        pbt2->setGeometry(QRect(170, 340, 101, 31));
        ped1 = new QPlainTextEdit(FourierWidget);
        ped1->setObjectName(QString::fromUtf8("ped1"));
        ped1->setGeometry(QRect(130, 80, 191, 41));
        ped2 = new QPlainTextEdit(FourierWidget);
        ped2->setObjectName(QString::fromUtf8("ped2"));
        ped2->setGeometry(QRect(130, 180, 201, 21));

        retranslateUi(FourierWidget);

        QMetaObject::connectSlotsByName(FourierWidget);
    } // setupUi

    void retranslateUi(QWidget *FourierWidget)
    {
        FourierWidget->setWindowTitle(QCoreApplication::translate("FourierWidget", "FourierWidget", nullptr));
        pbt1->setText(QCoreApplication::translate("FourierWidget", "\346\211\223\345\274\200\345\233\276\347\211\207(.bmp)", nullptr));
        pbt3->setText(QCoreApplication::translate("FourierWidget", "\345\205\263\351\227\255", nullptr));
        pbt2->setText(QCoreApplication::translate("FourierWidget", "\350\276\223\345\205\245\345\217\202\346\225\260\345\220\216\347\241\256\350\256\244.", nullptr));
        ped1->setPlainText(QCoreApplication::translate("FourierWidget", "n:\346\257\217\344\270\252\345\203\217\347\264\240rgb\350\275\254hsv\n"
"\345\260\217\344\272\216n\347\232\204\350\247\206\344\270\272\345\233\276\345\275\242\357\274\214\345\217\215\344\271\213\350\247\206\344\270\272\350\203\214\346\231\257", nullptr));
        ped2->setPlainText(QCoreApplication::translate("FourierWidget", "k:\345\221\250\350\275\254\345\234\206\347\232\204\351\242\221\347\216\207,\344\274\232\344\273\216[-k,k]\345\217\226\345\200\274.", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FourierWidget: public Ui_FourierWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FOURIERWIDGET_H
