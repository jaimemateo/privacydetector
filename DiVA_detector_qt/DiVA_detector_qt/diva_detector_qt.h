#ifndef DIVA_DETECTOR_QT_H
#define DIVA_DETECTOR_QT_H

#define	VERTICAL_SPACE		20
#define HORIZONTAL_SPACE	10
#define RIGHT_LEFT_MARGIN	30

#include <QtWidgets/QMainWindow>
#include "ui_diva_detector_qt.h"
#include "DiVA_DETECTOR.h"
#include "DiVA_QtSignals.h"
#include <Qimage>
#include <QPixmap>
#include <qmessagebox.h>

#include "ventana_ayuda.h"
#include "ventana_about.h"
#include "ventana_propiedades.h"
#include "ventana_camaras.h"

/*
#include <qmessagebox.h>

*/

class DiVA_detector_qt : public QMainWindow
{
	Q_OBJECT

public:
	DiVA_detector_qt(QWidget *parent = 0);
	~DiVA_detector_qt();
	DiVA_detector *principal;

public slots:
	void on_pushIniciar_clicked();
	void on_pushPrivacidad_clicked();
	void on_pushDetener_clicked();


private:

	int _port;
	char _ip[255];
	char _name[255];

	QImage *vpu_logo;
	QImage *eps_logo;
	QImage *uam_logo;
	QImage *min_logo;
	QImage *HAVideo_logo;
	Ui::DiVA_detector_qtClass ui;

	ventana_ayuda HelpWindow;
	ventana_about AboutWindow;
	ventana_propiedades PropsWindow;
	ventana_camaras CamWindow;

private slots:

	void update_frame(const QPixmap &frame);

	void launchHelpWindow();
	void launchAboutWindow();
	void launchPropsWindow();
	void launchCamWindow();
	void submitParam(int hmax,int hmin,int wmax,int wmin,int Q,int static_time,double MHI,double FG,double varnoise,int color);
	void updateCamInfo(camara nueva);
	void setLogos(int x, int y);
	void setButtons(int x, int y);
	void compactMode();

	void set_window_elements_position(int frame_w,int frame_h);
	
};

#endif // DIVA_DETECTOR_QT_H
