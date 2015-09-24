/********************************************************************************
** Form generated from reading UI file 'ventana_about.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VENTANA_ABOUT_H
#define UI_VENTANA_ABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ventana_about
{
public:
    QLabel *label_9;
    QLabel *label_2;
    QLabel *label_11;
    QLabel *label_5;
    QLabel *label_10;
    QLabel *label_3;
    QLabel *label;
    QLabel *label_4;
    QLabel *label_6;
    QLabel *label_8;

    void setupUi(QWidget *ventana_about)
    {
        if (ventana_about->objectName().isEmpty())
            ventana_about->setObjectName(QStringLiteral("ventana_about"));
        ventana_about->resize(400, 300);
        label_9 = new QLabel(ventana_about);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(90, 200, 201, 16));
        label_2 = new QLabel(ventana_about);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(70, 140, 251, 16));
        label_11 = new QLabel(ventana_about);
        label_11->setObjectName(QStringLiteral("label_11"));
        label_11->setGeometry(QRect(140, 220, 111, 16));
        QFont font;
        font.setUnderline(true);
        font.setStrikeOut(false);
        label_11->setFont(font);
        label_11->setOpenExternalLinks(true);
        label_5 = new QLabel(ventana_about);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(100, 70, 211, 16));
        label_10 = new QLabel(ventana_about);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(140, 240, 101, 16));
        label_3 = new QLabel(ventana_about);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(130, 160, 131, 16));
        label = new QLabel(ventana_about);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 10, 371, 31));
        QFont font1;
        font1.setPointSize(11);
        label->setFont(font1);
        label_4 = new QLabel(ventana_about);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(110, 90, 181, 16));
        label_6 = new QLabel(ventana_about);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(180, 40, 141, 16));
        QFont font2;
        font2.setPointSize(10);
        label_6->setFont(font2);
        label_8 = new QLabel(ventana_about);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(100, 40, 71, 16));
        label_8->setFont(font2);

        retranslateUi(ventana_about);

        QMetaObject::connectSlotsByName(ventana_about);
    } // setupUi

    void retranslateUi(QWidget *ventana_about)
    {
        ventana_about->setWindowTitle(QApplication::translate("ventana_about", "ventana_about", 0));
        label_9->setText(QApplication::translate("ventana_about", "Video Processing and Understanding Lab", 0));
        label_2->setText(QApplication::translate("ventana_about", "Aplicacion desarrollada por: Carlos Sanchez Bueno", 0));
        label_11->setText(QApplication::translate("ventana_about", "<a href=\"http://www-vpu.eps.uam.es\">www-vpu.eps.uam.es</a>", 0));
        label_5->setText(QApplication::translate("ventana_about", "Desarrollado por: Jaime Mateo Herrero", 0));
        label_10->setText(QApplication::translate("ventana_about", "vpu-info-l@uam.es", 0));
        label_3->setText(QApplication::translate("ventana_about", "carlos.sanchezb@uam.es", 0));
        label->setText(QApplication::translate("ventana_about", "Algoritmo Preservaci\303\263n de Personas en V\303\255deo Seguridad", 0));
        label_4->setText(QApplication::translate("ventana_about", "jaime.mateoh@estudiante.uam.es", 0));
        label_6->setText(QApplication::translate("ventana_about", "Septiembre - 2015", 0));
        label_8->setText(QApplication::translate("ventana_about", "Version 1.0", 0));
    } // retranslateUi

};

namespace Ui {
    class ventana_about: public Ui_ventana_about {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VENTANA_ABOUT_H
