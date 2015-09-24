#ifndef VENTANA_PROPIEDADES_H
#define VENTANA_PROPIEDADES_H
#include <QtWidgets\QDialog>
#include "ui_ventana_propiedades.h"


class ventana_propiedades : public QDialog
{
	Q_OBJECT

public:
	ventana_propiedades(QWidget *parent = 0);
	~ventana_propiedades();
	void setParam(int hmax,int hmin,int wmax,int wmin,int Q,int static_time,double MHI,double FG,double varnoise,int color);

public slots:
	void on_okButton_clicked();
	void on_cancelButton_clicked();
	void on_sliderchanged(int valor);
	void change_slider();
	void on_scrollchanged(int valor);
	void change_scroll();
	void on_horizontalSlider_valueChanged(int param);
	void change_color();

signals:
	void sendParam(int hmax,int hmin,int wmax,int wmin,int Q,int static_time,double MHI,double FG,double varnoise,int color);
private:
	Ui::ventana_propiedades ui;
};

#endif //VENTANA_PROPIEDADES_H