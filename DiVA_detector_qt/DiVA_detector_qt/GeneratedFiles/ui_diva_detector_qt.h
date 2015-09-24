/********************************************************************************
** Form generated from reading UI file 'diva_detector_qt.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIVA_DETECTOR_QT_H
#define UI_DIVA_DETECTOR_QT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DiVA_detector_qtClass
{
public:
    QAction *actionAyuda;
    QAction *actionCamera;
    QAction *actionPlay;
    QAction *actionStop;
    QAction *actionConfiguration;
    QAction *actionAyuda_3;
    QAction *actionSeleccion_Parametros;
    QAction *actionSeleccion_de_camara;
    QAction *actionSeleccion_de_region_de_analisis;
    QAction *actionModo_compacto;
    QAction *actionAyuda_2;
    QAction *actionAcerca_de;
    QWidget *centralWidget;
    QPushButton *pushIniciar;
    QPushButton *pushPrivacidad;
    QLabel *display;
    QLabel *VPULogo;
    QLabel *EPSLogo;
    QLabel *UAMLogo;
    QLabel *MinEcoLogo;
    QPushButton *pushCam;
    QPushButton *pushDetener;
    QLabel *HAVideoLogo;
    QLabel *camName;
    QMenuBar *menuBar;
    QMenu *menuHerramientas;
    QMenu *menuAyuda;
    QStatusBar *statusBar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *DiVA_detector_qtClass)
    {
        if (DiVA_detector_qtClass->objectName().isEmpty())
            DiVA_detector_qtClass->setObjectName(QStringLiteral("DiVA_detector_qtClass"));
        DiVA_detector_qtClass->resize(893, 678);
        actionAyuda = new QAction(DiVA_detector_qtClass);
        actionAyuda->setObjectName(QStringLiteral("actionAyuda"));
        actionCamera = new QAction(DiVA_detector_qtClass);
        actionCamera->setObjectName(QStringLiteral("actionCamera"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/DiVA_detector_qt/Resources/Iconos/cam.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionCamera->setIcon(icon);
        actionPlay = new QAction(DiVA_detector_qtClass);
        actionPlay->setObjectName(QStringLiteral("actionPlay"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/DiVA_detector_qt/Resources/Iconos/Play.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlay->setIcon(icon1);
        actionStop = new QAction(DiVA_detector_qtClass);
        actionStop->setObjectName(QStringLiteral("actionStop"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/DiVA_detector_qt/Resources/Iconos/stop.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionStop->setIcon(icon2);
        actionConfiguration = new QAction(DiVA_detector_qtClass);
        actionConfiguration->setObjectName(QStringLiteral("actionConfiguration"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/DiVA_detector_qt/Resources/Iconos/engranaje.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConfiguration->setIcon(icon3);
        actionAyuda_3 = new QAction(DiVA_detector_qtClass);
        actionAyuda_3->setObjectName(QStringLiteral("actionAyuda_3"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/DiVA_detector_qt/Resources/Iconos/ayda.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAyuda_3->setIcon(icon4);
        actionSeleccion_Parametros = new QAction(DiVA_detector_qtClass);
        actionSeleccion_Parametros->setObjectName(QStringLiteral("actionSeleccion_Parametros"));
        actionSeleccion_de_camara = new QAction(DiVA_detector_qtClass);
        actionSeleccion_de_camara->setObjectName(QStringLiteral("actionSeleccion_de_camara"));
        actionSeleccion_de_region_de_analisis = new QAction(DiVA_detector_qtClass);
        actionSeleccion_de_region_de_analisis->setObjectName(QStringLiteral("actionSeleccion_de_region_de_analisis"));
        actionModo_compacto = new QAction(DiVA_detector_qtClass);
        actionModo_compacto->setObjectName(QStringLiteral("actionModo_compacto"));
        actionAyuda_2 = new QAction(DiVA_detector_qtClass);
        actionAyuda_2->setObjectName(QStringLiteral("actionAyuda_2"));
        actionAcerca_de = new QAction(DiVA_detector_qtClass);
        actionAcerca_de->setObjectName(QStringLiteral("actionAcerca_de"));
        centralWidget = new QWidget(DiVA_detector_qtClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushIniciar = new QPushButton(centralWidget);
        pushIniciar->setObjectName(QStringLiteral("pushIniciar"));
        pushIniciar->setGeometry(QRect(40, 380, 101, 41));
        pushPrivacidad = new QPushButton(centralWidget);
        pushPrivacidad->setObjectName(QStringLiteral("pushPrivacidad"));
        pushPrivacidad->setGeometry(QRect(410, 380, 101, 41));
        display = new QLabel(centralWidget);
        display->setObjectName(QStringLiteral("display"));
        display->setGeometry(QRect(40, 40, 531, 251));
        QFont font;
        font.setPointSize(20);
        font.setBold(false);
        font.setWeight(50);
        display->setFont(font);
        display->setCursor(QCursor(Qt::CrossCursor));
        display->setFrameShape(QFrame::StyledPanel);
        display->setTextFormat(Qt::PlainText);
        display->setAlignment(Qt::AlignCenter);
        VPULogo = new QLabel(centralWidget);
        VPULogo->setObjectName(QStringLiteral("VPULogo"));
        VPULogo->setGeometry(QRect(50, 470, 101, 61));
        VPULogo->setMaximumSize(QSize(101, 61));
        VPULogo->setAcceptDrops(false);
        VPULogo->setAutoFillBackground(true);
        EPSLogo = new QLabel(centralWidget);
        EPSLogo->setObjectName(QStringLiteral("EPSLogo"));
        EPSLogo->setGeometry(QRect(170, 480, 121, 41));
        EPSLogo->setMaximumSize(QSize(121, 41));
        EPSLogo->setAutoFillBackground(true);
        UAMLogo = new QLabel(centralWidget);
        UAMLogo->setObjectName(QStringLiteral("UAMLogo"));
        UAMLogo->setGeometry(QRect(300, 470, 131, 71));
        UAMLogo->setAutoFillBackground(true);
        MinEcoLogo = new QLabel(centralWidget);
        MinEcoLogo->setObjectName(QStringLiteral("MinEcoLogo"));
        MinEcoLogo->setGeometry(QRect(500, 470, 111, 71));
        MinEcoLogo->setAutoFillBackground(true);
        pushCam = new QPushButton(centralWidget);
        pushCam->setObjectName(QStringLiteral("pushCam"));
        pushCam->setGeometry(QRect(280, 380, 101, 41));
        pushDetener = new QPushButton(centralWidget);
        pushDetener->setObjectName(QStringLiteral("pushDetener"));
        pushDetener->setGeometry(QRect(160, 380, 101, 41));
        HAVideoLogo = new QLabel(centralWidget);
        HAVideoLogo->setObjectName(QStringLiteral("HAVideoLogo"));
        HAVideoLogo->setGeometry(QRect(670, 450, 111, 71));
        HAVideoLogo->setAutoFillBackground(true);
        camName = new QLabel(centralWidget);
        camName->setObjectName(QStringLiteral("camName"));
        camName->setGeometry(QRect(670, 120, 81, 61));
        DiVA_detector_qtClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DiVA_detector_qtClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 893, 21));
        menuHerramientas = new QMenu(menuBar);
        menuHerramientas->setObjectName(QStringLiteral("menuHerramientas"));
        menuAyuda = new QMenu(menuBar);
        menuAyuda->setObjectName(QStringLiteral("menuAyuda"));
        DiVA_detector_qtClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(DiVA_detector_qtClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DiVA_detector_qtClass->setStatusBar(statusBar);
        toolBar = new QToolBar(DiVA_detector_qtClass);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        DiVA_detector_qtClass->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menuHerramientas->menuAction());
        menuBar->addAction(menuAyuda->menuAction());
        menuHerramientas->addSeparator();
        menuHerramientas->addAction(actionSeleccion_Parametros);
        menuHerramientas->addAction(actionSeleccion_de_camara);
        menuHerramientas->addAction(actionSeleccion_de_region_de_analisis);
        menuHerramientas->addAction(actionModo_compacto);
        menuAyuda->addAction(actionAyuda_2);
        menuAyuda->addAction(actionAcerca_de);
        toolBar->addSeparator();
        toolBar->addAction(actionCamera);
        toolBar->addAction(actionPlay);
        toolBar->addAction(actionStop);
        toolBar->addAction(actionConfiguration);
        toolBar->addAction(actionAyuda_3);

        retranslateUi(DiVA_detector_qtClass);

        QMetaObject::connectSlotsByName(DiVA_detector_qtClass);
    } // setupUi

    void retranslateUi(QMainWindow *DiVA_detector_qtClass)
    {
        DiVA_detector_qtClass->setWindowTitle(QApplication::translate("DiVA_detector_qtClass", "DiVA_detector_qt", 0));
        actionAyuda->setText(QApplication::translate("DiVA_detector_qtClass", "Ayuda", 0));
        actionCamera->setText(QApplication::translate("DiVA_detector_qtClass", "camera", 0));
        actionPlay->setText(QApplication::translate("DiVA_detector_qtClass", "play", 0));
        actionStop->setText(QApplication::translate("DiVA_detector_qtClass", "stop", 0));
        actionConfiguration->setText(QApplication::translate("DiVA_detector_qtClass", "configuration", 0));
        actionAyuda_3->setText(QApplication::translate("DiVA_detector_qtClass", "ayuda", 0));
#ifndef QT_NO_TOOLTIP
        actionAyuda_3->setToolTip(QApplication::translate("DiVA_detector_qtClass", "ayuda", 0));
#endif // QT_NO_TOOLTIP
        actionSeleccion_Parametros->setText(QApplication::translate("DiVA_detector_qtClass", "Configuracion de parametros", 0));
        actionSeleccion_de_camara->setText(QApplication::translate("DiVA_detector_qtClass", "Seleccion de camara", 0));
        actionSeleccion_de_region_de_analisis->setText(QApplication::translate("DiVA_detector_qtClass", "Seleccion de region de analisis", 0));
        actionModo_compacto->setText(QApplication::translate("DiVA_detector_qtClass", "Modo compacto", 0));
        actionAyuda_2->setText(QApplication::translate("DiVA_detector_qtClass", "Ayuda", 0));
        actionAcerca_de->setText(QApplication::translate("DiVA_detector_qtClass", "Acerca de...", 0));
        pushIniciar->setText(QApplication::translate("DiVA_detector_qtClass", "Iniciar", 0));
        pushPrivacidad->setText(QApplication::translate("DiVA_detector_qtClass", "Privacy", 0));
        display->setText(QApplication::translate("DiVA_detector_qtClass", "                                                                                  Video", 0));
#ifndef QT_NO_TOOLTIP
        VPULogo->setToolTip(QApplication::translate("DiVA_detector_qtClass", "<html><head/><body><p><img src=\":/DiVA_detector_qt/Resources/Logos/eventVideo_logo.png\"/></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        VPULogo->setText(QString());
#ifndef QT_NO_TOOLTIP
        EPSLogo->setToolTip(QApplication::translate("DiVA_detector_qtClass", "<html><head/><body><p><img src=\":/DiVA_detector_qt/Resources/Logos/MinecoLogo-200.png\"/></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        EPSLogo->setText(QString());
#ifndef QT_NO_TOOLTIP
        UAMLogo->setToolTip(QApplication::translate("DiVA_detector_qtClass", "<html><head/><body><p><img src=\":/DiVA_detector_qt/Resources/Logos/VPU_logo-200.png\"/></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        UAMLogo->setText(QString());
#ifndef QT_NO_TOOLTIP
        MinEcoLogo->setToolTip(QApplication::translate("DiVA_detector_qtClass", "<html><head/><body><p><img src=\":/DiVA_detector_qt/Resources/Logos/LogoOficial-EPS-200.png\"/></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        MinEcoLogo->setText(QString());
        pushCam->setText(QApplication::translate("DiVA_detector_qtClass", "Camara", 0));
        pushDetener->setText(QApplication::translate("DiVA_detector_qtClass", "Detener", 0));
#ifndef QT_NO_TOOLTIP
        HAVideoLogo->setToolTip(QApplication::translate("DiVA_detector_qtClass", "<html><head/><body><p><img src=\":/DiVA_detector_qt/Resources/Logos/LogoOficial-EPS-200.png\"/></p></body></html>", 0));
#endif // QT_NO_TOOLTIP
        HAVideoLogo->setText(QString());
        camName->setText(QApplication::translate("DiVA_detector_qtClass", "Camara", 0));
        menuHerramientas->setTitle(QApplication::translate("DiVA_detector_qtClass", "Herramientas", 0));
        menuAyuda->setTitle(QApplication::translate("DiVA_detector_qtClass", "Ayuda", 0));
        toolBar->setWindowTitle(QApplication::translate("DiVA_detector_qtClass", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class DiVA_detector_qtClass: public Ui_DiVA_detector_qtClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIVA_DETECTOR_QT_H
