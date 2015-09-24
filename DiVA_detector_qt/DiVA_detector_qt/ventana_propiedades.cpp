#include "ventana_propiedades.h"
#include <iostream>
#include "DiVA_detector_qt.h"

ventana_propiedades::ventana_propiedades(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.sliderMHI,SIGNAL(valueChanged(int)),this,SLOT(on_sliderchanged(int)));
	connect(ui.lineMHI,&QLineEdit::textChanged,this,&ventana_propiedades::change_slider);
	connect(ui.scrollStaticTime,&QScrollBar::valueChanged,this,&ventana_propiedades::on_scrollchanged);
	connect(ui.lineStaticTime,&QLineEdit::textChanged,this,&ventana_propiedades::change_scroll);

	connect(ui.sliderColor,SIGNAL(valueChanged(int)),this,SLOT(on_horizontalSlider_valueChanged(int)));
	connect(ui.lineColor,&QLineEdit::textChanged,this,&ventana_propiedades::change_color);
	
}

ventana_propiedades::~ventana_propiedades()
{

}


void ventana_propiedades::setParam(int hmax,int hmin,int wmax,int wmin,int Q,int static_time,double MHI,double FG,double varnoise,int color)
{
	ui.lineFG_DEC->setText(QString::number(FG,'g',2));

	ui.lineMHI->setText(QString::number(MHI,'g',2));
	ui.sliderMHI->setRange(0,100);
	ui.sliderMHI->setValue(MHI*100);

	ui.lineStaticTime->setText(QString::number(static_time));
	ui.scrollStaticTime->setRange(5,20);
	ui.scrollStaticTime->setValue(static_time);

	ui.lineVarnoise->setText(QString::number(varnoise,'g',2));

	ui.spinQ->setValue(Q);
	ui.spinQ->setMaximum(30);
	ui.spinQ->setMinimum(1);
	ui.lineMaxH->setText(QString::number(hmax));
	ui.lineMaxW->setText(QString::number(wmax));
	ui.lineMinH->setText(QString::number(hmin));
	ui.lineMinW->setText(QString::number(wmin));

	ui.lineColor->setText(QString::number(color,'g',2));
	ui.sliderColor->setRange(1,4);
	ui.sliderColor->setValue(color);
}

void ventana_propiedades::on_okButton_clicked()
{
	emit sendParam(ui.lineMaxH->text().toInt(),ui.lineMinH->text().toInt(),ui.lineMaxW->text().toInt(),ui.lineMinW->text().toInt(),ui.spinQ->value(),ui.scrollStaticTime->value(),((double)ui.sliderMHI->value()/100.0),ui.lineFG_DEC->text().toDouble(),0.1,ui.sliderColor->value());
	this->close();
}

void ventana_propiedades::on_cancelButton_clicked()
{
	this->close();
}

void ventana_propiedades::on_sliderchanged(int valor)
{
	ui.lineMHI->setText(QString::number(valor/100.0,'g',2));
}

void ventana_propiedades::change_slider()
{
	QString auxiliar(ui.lineMHI->text());
	ui.sliderMHI->setValue(auxiliar.toDouble()*100);
}

void ventana_propiedades::on_scrollchanged(int valor)
{
	ui.lineStaticTime->setText(QString::number(valor));
}

void ventana_propiedades::change_scroll()
{
	QString auxiliar(ui.lineStaticTime->text());
	ui.scrollStaticTime->setValue(auxiliar.toInt());
}

void ventana_propiedades::on_horizontalSlider_valueChanged(int param){

	ui.lineColor->setText(QString::number(param,'g',2));
}

void ventana_propiedades::change_color()
{
	QString auxiliar(ui.lineColor->text());
	ui.sliderColor->setValue(auxiliar.toDouble());
}