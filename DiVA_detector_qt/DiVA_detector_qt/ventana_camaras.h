#ifndef VENTANA_CAMARAS_H
#define VENTANA_CAMARAS_H

#include <QtWidgets/QDialog>
#include "ui_ventana_camaras.h"
#include "camaras.h"

class ventana_camaras : public QDialog
{
	Q_OBJECT

public:
	ventana_camaras(QWidget *parent = 0);
	~ventana_camaras();
	public slots:
		void on_editcam_clicked();
		void on_newcam_clicked();
		void on_delcam_clicked();
		void on_acept_clicked();
		void on_cancel_clicked();
		void on_savechange_clicked();
		void on_pushDefault_clicked();
signals:
	void selectedCamera(camara seleccionada);
private:
	Ui::ventana_camaras ui;
	cam_list *lista;
	void resizeWindow(int w,int h);
	int newcam;
};

#endif // VENTANA_CAMARAS_H
