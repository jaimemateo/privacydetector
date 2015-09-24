/********************************************************************************
** Form generated from reading UI file 'ventana_ayuda.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VENTANA_AYUDA_H
#define UI_VENTANA_AYUDA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ventana_ayuda
{
public:
    QTextBrowser *textBrowser;

    void setupUi(QWidget *ventana_ayuda)
    {
        if (ventana_ayuda->objectName().isEmpty())
            ventana_ayuda->setObjectName(QStringLiteral("ventana_ayuda"));
        ventana_ayuda->resize(543, 385);
        textBrowser = new QTextBrowser(ventana_ayuda);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(20, 10, 441, 341));

        retranslateUi(ventana_ayuda);

        QMetaObject::connectSlotsByName(ventana_ayuda);
    } // setupUi

    void retranslateUi(QWidget *ventana_ayuda)
    {
        ventana_ayuda->setWindowTitle(QApplication::translate("ventana_ayuda", "Ayuda", 0));
        textBrowser->setHtml(QApplication::translate("ventana_ayuda", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Descripci\303\263n de los par\303\241metros:</span></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">	- </span><span style=\" font-size:10pt; font-weight:600;\">FG_Dec: </span><span style=\" font-size:10pt;\">Valor de decremento de la imagen acumulaci\303\263n de frente cuando un p\303\255xel es de fondo. Debe ajustarse junto con el tiempo est\303\241tico. </span></p>\n"
"<p "
                        "align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">	- VarNoise: </span><span style=\" font-size:10pt;\">Varianza del ruido para el algoritmo de sustracci\303\263n de fondo.</span></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">	- Q: </span><span style=\" font-size:10pt;\">Tama\303\261o de la ventana centrada en el p\303\255xel sobre la que se promedia SSIM para asignar el valor de la caracter\303\255stica de estructura. Un valor de 1 es el m\303\251todo SSIM original.</span></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">	- Tama\303\261o de objetos detectables:</span><span style=\" f"
                        "ont-size:10pt;\"> dimensiones m\303\241ximas y m\303\255nimas (en p\303\255xeles) que tendr\303\241n los objetos que detectar\303\241 el algoritmo.</span></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">	- MHI_percentage: </span><span style=\" font-size:10pt;\">Porcentaje de aportaci\303\263n de la acumulaci\303\263n de movimiento a la im\303\241gen de acumulaci\303\263n final. Puede variar entre 0 y 100. Valor recomendado: 27%.</span></p>\n"
"<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">	- Tiempo est\303\241tico: </span><span style=\" font-size:10pt;\">tiempo m\303\255nimo (en segundos) que debe permanecer un objeto est\303\241tico para poder ser detectado por el algoritmo. Puede variar entre 10 y 60 segundos.</span></p>\n"
""
                        "<p align=\"justify\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">	-   </span><span style=\" font-size:10pt; font-weight:600;\">Color mode: </span><span style=\" font-size:10pt;\">canales de color a los que se les aplica la t\303\251cnica de </span><span style=\" font-size:10pt; font-style:italic;\">scrambling: </span><span style=\" font-size:10pt;\">RGB, GR, YCrCb o Y</span></p>\n"
"<p align=\"justify\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:8pt;\"><br /></p></body></html>", 0));
    } // retranslateUi

};

namespace Ui {
    class ventana_ayuda: public Ui_ventana_ayuda {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VENTANA_AYUDA_H
