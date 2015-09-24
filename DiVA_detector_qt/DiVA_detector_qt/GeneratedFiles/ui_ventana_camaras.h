/********************************************************************************
** Form generated from reading UI file 'ventana_camaras.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VENTANA_CAMARAS_H
#define UI_VENTANA_CAMARAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ventana_camaras
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QComboBox *camselector;
    QPushButton *editcam;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *newcam;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *delcam;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *pushDefault;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QPushButton *acept;
    QPushButton *cancel;
    QWidget *layoutWidget_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QLabel *name;
    QSpacerItem *horizontalSpacer_6;
    QLineEdit *namein;
    QHBoxLayout *horizontalLayout_6;
    QLabel *ip;
    QSpacerItem *horizontalSpacer_4;
    QLineEdit *ipin;
    QHBoxLayout *horizontalLayout_7;
    QLabel *port;
    QSpacerItem *horizontalSpacer_5;
    QLineEdit *portin;
    QHBoxLayout *horizontalLayout_8;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *savechange;

    void setupUi(QWidget *ventana_camaras)
    {
        if (ventana_camaras->objectName().isEmpty())
            ventana_camaras->setObjectName(QStringLiteral("ventana_camaras"));
        ventana_camaras->resize(617, 234);
        layoutWidget = new QWidget(ventana_camaras);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(30, 20, 241, 181));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        camselector = new QComboBox(layoutWidget);
        camselector->setObjectName(QStringLiteral("camselector"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(camselector->sizePolicy().hasHeightForWidth());
        camselector->setSizePolicy(sizePolicy);
        camselector->setMinimumSize(QSize(90, 0));

        horizontalLayout_2->addWidget(camselector);

        editcam = new QPushButton(layoutWidget);
        editcam->setObjectName(QStringLiteral("editcam"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(editcam->sizePolicy().hasHeightForWidth());
        editcam->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(editcam);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        newcam = new QPushButton(layoutWidget);
        newcam->setObjectName(QStringLiteral("newcam"));

        horizontalLayout_3->addWidget(newcam);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        delcam = new QPushButton(layoutWidget);
        delcam->setObjectName(QStringLiteral("delcam"));

        horizontalLayout_4->addWidget(delcam);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_7);

        pushDefault = new QPushButton(layoutWidget);
        pushDefault->setObjectName(QStringLiteral("pushDefault"));

        horizontalLayout_9->addWidget(pushDefault);


        verticalLayout->addLayout(horizontalLayout_9);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        acept = new QPushButton(layoutWidget);
        acept->setObjectName(QStringLiteral("acept"));
        sizePolicy1.setHeightForWidth(acept->sizePolicy().hasHeightForWidth());
        acept->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(acept);

        cancel = new QPushButton(layoutWidget);
        cancel->setObjectName(QStringLiteral("cancel"));
        sizePolicy1.setHeightForWidth(cancel->sizePolicy().hasHeightForWidth());
        cancel->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(cancel);


        verticalLayout->addLayout(horizontalLayout);

        layoutWidget_2 = new QWidget(ventana_camaras);
        layoutWidget_2->setObjectName(QStringLiteral("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(370, 40, 211, 111));
        verticalLayout_2 = new QVBoxLayout(layoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        name = new QLabel(layoutWidget_2);
        name->setObjectName(QStringLiteral("name"));

        horizontalLayout_5->addWidget(name);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_6);

        namein = new QLineEdit(layoutWidget_2);
        namein->setObjectName(QStringLiteral("namein"));
        namein->setEnabled(false);
        namein->setMinimumSize(QSize(130, 0));
        namein->setMaximumSize(QSize(130, 16777215));

        horizontalLayout_5->addWidget(namein);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        ip = new QLabel(layoutWidget_2);
        ip->setObjectName(QStringLiteral("ip"));

        horizontalLayout_6->addWidget(ip);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);

        ipin = new QLineEdit(layoutWidget_2);
        ipin->setObjectName(QStringLiteral("ipin"));
        ipin->setEnabled(false);
        ipin->setMinimumSize(QSize(130, 0));
        ipin->setMaximumSize(QSize(130, 16777215));

        horizontalLayout_6->addWidget(ipin);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        port = new QLabel(layoutWidget_2);
        port->setObjectName(QStringLiteral("port"));

        horizontalLayout_7->addWidget(port);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_5);

        portin = new QLineEdit(layoutWidget_2);
        portin->setObjectName(QStringLiteral("portin"));
        portin->setEnabled(false);
        portin->setMinimumSize(QSize(130, 0));
        portin->setMaximumSize(QSize(130, 16777215));

        horizontalLayout_7->addWidget(portin);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_3);

        savechange = new QPushButton(layoutWidget_2);
        savechange->setObjectName(QStringLiteral("savechange"));
        savechange->setEnabled(false);

        horizontalLayout_8->addWidget(savechange);


        verticalLayout_2->addLayout(horizontalLayout_8);


        retranslateUi(ventana_camaras);

        QMetaObject::connectSlotsByName(ventana_camaras);
    } // setupUi

    void retranslateUi(QWidget *ventana_camaras)
    {
        ventana_camaras->setWindowTitle(QApplication::translate("ventana_camaras", "ventana_camaras", 0));
        editcam->setText(QApplication::translate("ventana_camaras", "Editar", 0));
        newcam->setText(QApplication::translate("ventana_camaras", "Nueva", 0));
        delcam->setText(QApplication::translate("ventana_camaras", "Eliminar", 0));
        pushDefault->setText(QApplication::translate("ventana_camaras", "Default", 0));
        acept->setText(QApplication::translate("ventana_camaras", "Aceptar", 0));
        cancel->setText(QApplication::translate("ventana_camaras", "Cancelar", 0));
        name->setText(QApplication::translate("ventana_camaras", "Nombre", 0));
        ip->setText(QApplication::translate("ventana_camaras", "IP", 0));
        port->setText(QApplication::translate("ventana_camaras", "Puerto", 0));
        savechange->setText(QApplication::translate("ventana_camaras", "Guardar cambios", 0));
    } // retranslateUi

};

namespace Ui {
    class ventana_camaras: public Ui_ventana_camaras {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VENTANA_CAMARAS_H
