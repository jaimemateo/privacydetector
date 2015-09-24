/********************************************************************************
** Form generated from reading UI file 'ventana_propiedades.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VENTANA_PROPIEDADES_H
#define UI_VENTANA_PROPIEDADES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ventana_propiedades
{
public:
    QLineEdit *lineMinW;
    QFrame *line;
    QLabel *label_9;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QLineEdit *lineMHI;
    QSlider *sliderMHI;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_3;
    QLineEdit *lineStaticTime;
    QScrollBar *scrollStaticTime;
    QLabel *label_7;
    QLineEdit *lineMaxW;
    QLabel *label_8;
    QWidget *layoutWidget_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *lineFG_DEC;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QLineEdit *lineVarnoise;
    QHBoxLayout *horizontalLayout;
    QLabel *label_6;
    QSpinBox *spinQ;
    QLabel *label_5;
    QLabel *label_10;
    QLineEdit *lineMinH;
    QLineEdit *lineMaxH;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_11;
    QLineEdit *lineColor;
    QSlider *sliderColor;
    QTextBrowser *textBrowser;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QWidget *ventana_propiedades)
    {
        if (ventana_propiedades->objectName().isEmpty())
            ventana_propiedades->setObjectName(QStringLiteral("ventana_propiedades"));
        ventana_propiedades->resize(664, 414);
        lineMinW = new QLineEdit(ventana_propiedades);
        lineMinW->setObjectName(QStringLiteral("lineMinW"));
        lineMinW->setGeometry(QRect(150, 320, 31, 20));
        line = new QFrame(ventana_propiedades);
        line->setObjectName(QStringLiteral("line"));
        line->setGeometry(QRect(80, 260, 241, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_9 = new QLabel(ventana_propiedades);
        label_9->setObjectName(QStringLiteral("label_9"));
        label_9->setGeometry(QRect(160, 300, 46, 13));
        layoutWidget = new QWidget(ventana_propiedades);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(80, 110, 237, 54));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_2 = new QLabel(layoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_4->addWidget(label_2);

        lineMHI = new QLineEdit(layoutWidget);
        lineMHI->setObjectName(QStringLiteral("lineMHI"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineMHI->sizePolicy().hasHeightForWidth());
        lineMHI->setSizePolicy(sizePolicy);
        lineMHI->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_4->addWidget(lineMHI);

        sliderMHI = new QSlider(layoutWidget);
        sliderMHI->setObjectName(QStringLiteral("sliderMHI"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(sliderMHI->sizePolicy().hasHeightForWidth());
        sliderMHI->setSizePolicy(sizePolicy1);
        sliderMHI->setMinimumSize(QSize(100, 0));
        sliderMHI->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(sliderMHI);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label_3 = new QLabel(layoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_5->addWidget(label_3);

        lineStaticTime = new QLineEdit(layoutWidget);
        lineStaticTime->setObjectName(QStringLiteral("lineStaticTime"));
        sizePolicy.setHeightForWidth(lineStaticTime->sizePolicy().hasHeightForWidth());
        lineStaticTime->setSizePolicy(sizePolicy);
        lineStaticTime->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_5->addWidget(lineStaticTime);

        scrollStaticTime = new QScrollBar(layoutWidget);
        scrollStaticTime->setObjectName(QStringLiteral("scrollStaticTime"));
        sizePolicy1.setHeightForWidth(scrollStaticTime->sizePolicy().hasHeightForWidth());
        scrollStaticTime->setSizePolicy(sizePolicy1);
        scrollStaticTime->setMinimumSize(QSize(100, 0));
        scrollStaticTime->setOrientation(Qt::Horizontal);

        horizontalLayout_5->addWidget(scrollStaticTime);


        verticalLayout_2->addLayout(horizontalLayout_5);

        label_7 = new QLabel(ventana_propiedades);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(83, 279, 154, 16));
        lineMaxW = new QLineEdit(ventana_propiedades);
        lineMaxW->setObjectName(QStringLiteral("lineMaxW"));
        lineMaxW->setGeometry(QRect(190, 320, 31, 20));
        label_8 = new QLabel(ventana_propiedades);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(120, 350, 46, 13));
        layoutWidget_2 = new QWidget(ventana_propiedades);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(80, 20, 91, 80));
        verticalLayout = new QVBoxLayout(layoutWidget_2);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label = new QLabel(layoutWidget_2);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        lineFG_DEC = new QLineEdit(layoutWidget_2);
        lineFG_DEC->setObjectName(QStringLiteral("lineFG_DEC"));
        sizePolicy.setHeightForWidth(lineFG_DEC->sizePolicy().hasHeightForWidth());
        lineFG_DEC->setSizePolicy(sizePolicy);
        lineFG_DEC->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_3->addWidget(lineFG_DEC);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_4 = new QLabel(layoutWidget_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_2->addWidget(label_4);

        lineVarnoise = new QLineEdit(layoutWidget_2);
        lineVarnoise->setObjectName(QStringLiteral("lineVarnoise"));
        sizePolicy.setHeightForWidth(lineVarnoise->sizePolicy().hasHeightForWidth());
        lineVarnoise->setSizePolicy(sizePolicy);
        lineVarnoise->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_2->addWidget(lineVarnoise);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_6 = new QLabel(layoutWidget_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout->addWidget(label_6);

        spinQ = new QSpinBox(layoutWidget_2);
        spinQ->setObjectName(QStringLiteral("spinQ"));

        horizontalLayout->addWidget(spinQ);


        verticalLayout->addLayout(horizontalLayout);

        label_5 = new QLabel(ventana_propiedades);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(110, 320, 46, 13));
        label_10 = new QLabel(ventana_propiedades);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setGeometry(QRect(190, 300, 46, 13));
        lineMinH = new QLineEdit(ventana_propiedades);
        lineMinH->setObjectName(QStringLiteral("lineMinH"));
        lineMinH->setGeometry(QRect(150, 350, 31, 20));
        lineMaxH = new QLineEdit(ventana_propiedades);
        lineMaxH->setObjectName(QStringLiteral("lineMaxH"));
        lineMaxH->setGeometry(QRect(190, 350, 31, 20));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(lineMaxH->sizePolicy().hasHeightForWidth());
        lineMaxH->setSizePolicy(sizePolicy2);
        layoutWidget1 = new QWidget(ventana_propiedades);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(80, 170, 312, 81));
        horizontalLayout_6 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalLayout_6->setContentsMargins(0, 0, 0, 0);
        label_11 = new QLabel(layoutWidget1);
        label_11->setObjectName(QStringLiteral("label_11"));

        horizontalLayout_6->addWidget(label_11);

        lineColor = new QLineEdit(layoutWidget1);
        lineColor->setObjectName(QStringLiteral("lineColor"));
        sizePolicy.setHeightForWidth(lineColor->sizePolicy().hasHeightForWidth());
        lineColor->setSizePolicy(sizePolicy);
        lineColor->setMaximumSize(QSize(40, 16777215));

        horizontalLayout_6->addWidget(lineColor);

        sliderColor = new QSlider(layoutWidget1);
        sliderColor->setObjectName(QStringLiteral("sliderColor"));
        sizePolicy1.setHeightForWidth(sliderColor->sizePolicy().hasHeightForWidth());
        sliderColor->setSizePolicy(sizePolicy1);
        sliderColor->setMinimumSize(QSize(100, 0));
        sliderColor->setMinimum(1);
        sliderColor->setMaximum(4);
        sliderColor->setOrientation(Qt::Horizontal);

        horizontalLayout_6->addWidget(sliderColor);

        textBrowser = new QTextBrowser(layoutWidget1);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));

        horizontalLayout_6->addWidget(textBrowser);

        cancelButton = new QPushButton(ventana_propiedades);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));
        cancelButton->setGeometry(QRect(331, 330, 75, 23));
        okButton = new QPushButton(ventana_propiedades);
        okButton->setObjectName(QStringLiteral("okButton"));
        okButton->setGeometry(QRect(250, 330, 75, 23));

        retranslateUi(ventana_propiedades);

        QMetaObject::connectSlotsByName(ventana_propiedades);
    } // setupUi

    void retranslateUi(QWidget *ventana_propiedades)
    {
        ventana_propiedades->setWindowTitle(QApplication::translate("ventana_propiedades", "ventana_propiedades", 0));
        label_9->setText(QApplication::translate("ventana_propiedades", "Min", 0));
        label_2->setText(QApplication::translate("ventana_propiedades", "MHI_percentage", 0));
        label_3->setText(QApplication::translate("ventana_propiedades", "Tiempo estatico  ", 0));
        label_7->setText(QApplication::translate("ventana_propiedades", "Tama\303\261o de objetos detectables:", 0));
        label_8->setText(QApplication::translate("ventana_propiedades", "Alto:", 0));
        label->setText(QApplication::translate("ventana_propiedades", "FG_Dec", 0));
        label_4->setText(QApplication::translate("ventana_propiedades", "VarNoise", 0));
        label_6->setText(QApplication::translate("ventana_propiedades", "Q", 0));
        label_5->setText(QApplication::translate("ventana_propiedades", "Ancho:", 0));
        label_10->setText(QApplication::translate("ventana_propiedades", "Max", 0));
        label_11->setText(QApplication::translate("ventana_propiedades", "Color Mode         ", 0));
        textBrowser->setHtml(QApplication::translate("ventana_propiedades", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt;\">RGB - 1     GR - 2    YCrCb - 3    Y - 4</span></p></body></html>", 0));
        cancelButton->setText(QApplication::translate("ventana_propiedades", "Cancel", 0));
        okButton->setText(QApplication::translate("ventana_propiedades", "OK", 0));
    } // retranslateUi

};

namespace Ui {
    class ventana_propiedades: public Ui_ventana_propiedades {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VENTANA_PROPIEDADES_H
