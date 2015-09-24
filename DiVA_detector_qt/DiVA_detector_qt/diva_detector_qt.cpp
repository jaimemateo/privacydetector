#include "DiVA_detector_qt.h"
#include <qdebug.h>
#include <algorithm> 


DiVA_detector_qt::DiVA_detector_qt(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//char _ip[100]="127.0.0.1";
	//int   _port = 20252;

	strcpy(this->_ip,"0.0.0.0");
	this->_port = -1;

	
	
	ui.actionSeleccion_Parametros->setEnabled(false);
	//connect(ui.actionAyuda,&QAction::triggered,this,&DiVA_detector_qt::launchHelpWindow);
	connect(ui.actionAyuda_2,&QAction::triggered,this,&DiVA_detector_qt::launchHelpWindow);
	connect(ui.actionAyuda_3,&QAction::triggered,this,&DiVA_detector_qt::launchHelpWindow);
	connect(ui.actionAcerca_de,&QAction::triggered,this,&DiVA_detector_qt::launchAboutWindow);
	connect(ui.actionConfiguration,&QAction::triggered,this,&DiVA_detector_qt::launchPropsWindow);
	connect(ui.pushCam,SIGNAL(clicked()),this,SLOT(launchCamWindow()));
	connect(ui.actionSeleccion_de_camara,&QAction::triggered,this,&DiVA_detector_qt::launchCamWindow);
	connect(ui.actionSeleccion_Parametros,&QAction::triggered,this,&DiVA_detector_qt::launchPropsWindow);
	connect(ui.actionModo_compacto,SIGNAL(triggered()),this,SLOT(compactMode()));

	connect(ui.actionPlay,&QAction::triggered,this,&DiVA_detector_qt::on_pushIniciar_clicked);
	connect(ui.actionStop,&QAction::triggered,this,&DiVA_detector_qt::on_pushDetener_clicked);
	connect(ui.actionCamera,&QAction::triggered,this,&DiVA_detector_qt::launchCamWindow);

	connect(&this->CamWindow,&ventana_camaras::selectedCamera, this, &DiVA_detector_qt::updateCamInfo);

	this->CamWindow.on_acept_clicked();

	/* lectura logos */
	vpu_logo = new QImage(":/DiVA_detector_qt/Resources/Logos/VPU_logo-200.png");
	eps_logo = new QImage(":/DiVA_detector_qt/Resources/Logos/LogoOficial-EPS-200.png");
	uam_logo = new QImage(":/DiVA_detector_qt/Resources/Logos/logoUAM-200.png");
	min_logo = new QImage(":/DiVA_detector_qt/Resources/Logos/MinecoLogo-200.png");
	HAVideo_logo = new QImage(":/DiVA_detector_qt/Resources/Logos/HAVideo_logo.png");
	this->set_window_elements_position(320,240);

	
}

DiVA_detector_qt::~DiVA_detector_qt()
{

	delete vpu_logo;
	delete eps_logo;
	delete uam_logo;
	delete min_logo;
	delete HAVideo_logo;

}


void DiVA_detector_qt::set_window_elements_position(int frame_w,int frame_h)
{
	int buttons_w,logos_w,buttons_h,logos_h;

	/* calculo de ancho  y alto de ventana */
	//hmax = VERTICAL_SPACE+frame_h+VERTICAL_SPACE+ 
	//	max(ui.pushCam->height(),max(ui.pushDetener->height(),max(ui.pushIniciar->height(),max(ui.pushParam->height(),ui.pushROI->height()))))+
	//	VERTICAL_SPACE+
	//	max(this->vpu_logo->height(),max(this->eps_logo->height(),max(this->uam_logo->height(),this->min_logo->height())))+
	//	VERTICAL_SPACE;
	

	buttons_h = max(ui.pushCam->height(),max(ui.pushDetener->height(),max(ui.pushIniciar->height(),max(ui.pushPrivacidad->height(),0))));
	logos_h =  max(this->HAVideo_logo->height(),max(this->vpu_logo->height(),max(this->eps_logo->height(),max(this->uam_logo->height(),this->min_logo->height()))));



/*	wmax = max(frame_w,max(ui.pushCam->width()+ui.pushDetener->width()+ui.pushIniciar->width()+ui.pushParam->width()+ui.pushROI->width(),
							this->vpu_logo->width()+this->eps_logo->width()+this->uam_logo->width()+this->min_logo->width()));*/
	buttons_w = ui.pushCam->width()+ui.pushDetener->width()+ui.pushIniciar->width()+ui.pushPrivacidad->width()+4*HORIZONTAL_SPACE;
	logos_w = this->vpu_logo->width()+this->eps_logo->width()+this->uam_logo->width()+this->min_logo->width()+this->HAVideo_logo->width()+3*HORIZONTAL_SPACE;

	if ( frame_w >= logos_w && frame_w>=buttons_w)
	{
		this->setFixedSize(frame_w+2*RIGHT_LEFT_MARGIN,frame_h+buttons_h+logos_h+8*VERTICAL_SPACE);
		ui.camName->setGeometry(RIGHT_LEFT_MARGIN,VERTICAL_SPACE,ui.camName->fontMetrics().boundingRect(ui.camName->text()).width(),15);
		ui.display->setGeometry(RIGHT_LEFT_MARGIN,ui.camName->geometry().bottom(),frame_w,frame_h);
		this->setButtons((frame_w+2*RIGHT_LEFT_MARGIN-buttons_w)/2,ui.display->geometry().bottom()+VERTICAL_SPACE);
		this->setLogos((frame_w+2*RIGHT_LEFT_MARGIN-logos_w)/2,ui.pushCam->geometry().top()+buttons_h+VERTICAL_SPACE);
		
	}
	else if (logos_w >= buttons_w)
	{
		this->setFixedSize(logos_w+2*RIGHT_LEFT_MARGIN,frame_h+buttons_h+logos_h+8*VERTICAL_SPACE);
		ui.camName->setGeometry(RIGHT_LEFT_MARGIN,VERTICAL_SPACE,ui.camName->fontMetrics().boundingRect(ui.camName->text()).width(),15);
		ui.display->setGeometry((int)((logos_w+2*RIGHT_LEFT_MARGIN-frame_w)/2),ui.camName->geometry().bottom(),frame_w,frame_h);
		this->setButtons((logos_w+2*RIGHT_LEFT_MARGIN-buttons_w)/2,ui.display->geometry().bottom()+VERTICAL_SPACE);
		this->setLogos(RIGHT_LEFT_MARGIN,ui.pushCam->geometry().top()+buttons_h+VERTICAL_SPACE);
	}
	else
	{		
		this->setFixedSize(buttons_w+2*RIGHT_LEFT_MARGIN,frame_h+buttons_h+logos_h+8*VERTICAL_SPACE);
		ui.camName->setGeometry(RIGHT_LEFT_MARGIN,VERTICAL_SPACE,ui.camName->fontMetrics().boundingRect(ui.camName->text()).width(),15);
		ui.display->setGeometry(RIGHT_LEFT_MARGIN,ui.camName->geometry().bottom(),frame_w,frame_h);
		this->setButtons(RIGHT_LEFT_MARGIN,ui.display->geometry().bottom()+VERTICAL_SPACE);
		this->setLogos((buttons_w+2*RIGHT_LEFT_MARGIN-logos_w)/2,ui.pushCam->geometry().top()+buttons_h+VERTICAL_SPACE);
	}

}
void DiVA_detector_qt::setLogos(int x, int y)
{
	
	/*  LOGOS */
	ui.HAVideoLogo->setGeometry(x,y+VERTICAL_SPACE,HAVideo_logo->size().width(),HAVideo_logo->size().height());
	ui.HAVideoLogo->setPixmap(QPixmap::fromImage(*HAVideo_logo));

	//ui.supportedBy->move(ui.EventVideoLogo->geometry().right()+HORIZONTAL_SPACE,y);
	ui.MinEcoLogo->setGeometry(ui.HAVideoLogo->geometry().right()+HORIZONTAL_SPACE,y+VERTICAL_SPACE,min_logo->size().width(),min_logo->size().height());
	ui.MinEcoLogo->setPixmap(QPixmap::fromImage(*min_logo));

	ui.VPULogo->setGeometry(ui.MinEcoLogo->geometry().right()+HORIZONTAL_SPACE,y+VERTICAL_SPACE,vpu_logo->size().width(),vpu_logo->size().height());
	ui.VPULogo->setPixmap(QPixmap::fromImage(*vpu_logo));

	ui.EPSLogo->setGeometry(ui.VPULogo->geometry().right()+HORIZONTAL_SPACE,y+VERTICAL_SPACE,eps_logo->size().width(),eps_logo->size().height());
	ui.EPSLogo->setPixmap(QPixmap::fromImage(*eps_logo));
	
	ui.UAMLogo->setGeometry(ui.EPSLogo->geometry().right()+HORIZONTAL_SPACE,y+VERTICAL_SPACE,uam_logo->size().width(),uam_logo->size().height());
	ui.UAMLogo->setPixmap(QPixmap::fromImage(*uam_logo));
	

//	this->setMinimumSize(ui.EventVideoLogo->geometry().right()+20,ui.EPSLogo->geometry().bottom()+40);
	
	
}

void DiVA_detector_qt::setButtons(int x, int y)
{
	ui.pushIniciar->setGeometry(x,y,ui.pushIniciar->width(),ui.pushIniciar->height());
	ui.pushDetener->setGeometry(ui.pushIniciar->geometry().right()+HORIZONTAL_SPACE,y,ui.pushDetener->width(),ui.pushDetener->height());
	ui.pushCam->setGeometry(ui.pushDetener->geometry().right()+HORIZONTAL_SPACE,y,ui.pushCam->width(),ui.pushCam->height());
	ui.pushPrivacidad->setGeometry(ui.pushCam->geometry().right()+HORIZONTAL_SPACE,y,ui.pushPrivacidad->width(),ui.pushPrivacidad->height());
	
}



void DiVA_detector_qt::on_pushIniciar_clicked(){
	
	if(this->_port != -1)
	{

		/*if (this->principal)
		{
			this->principal->stop();
			//delete algoritmo;
		}*/
		
		this->principal = new DiVA_detector(this->_ip,this->_port,FALSE,TRUE);
		
		int ret = this->principal->init();
		if (ret<0)
		{
			principal->~DiVA_detector();
			QMessageBox box;
			box.setInformativeText("Error al iniciar el algoritmo.");
			box.setStandardButtons(QMessageBox::Ok);
			box.exec();
			return;
		}

		ret = principal->start();
		ui.actionSeleccion_Parametros->setEnabled(true);
		
	}
	else
	{
		QMessageBox box;
		box.setInformativeText("Debe seleccionar primero una camara.");
		box.setStandardButtons(QMessageBox::Ok);
		box.exec();
	}

	connect(principal->senales, SIGNAL(refresh_display(QPixmap)),this,SLOT(update_frame(QPixmap)));

}

void DiVA_detector_qt::on_pushPrivacidad_clicked(){

	if (this->principal->Algor->scram == 0)
		this->principal->Algor->scram = POSTSCRAM;
	else
		this->principal->Algor->scram = PRESCRAM;

	connect(principal->senales, SIGNAL(refresh_display(QPixmap)),this,SLOT(update_frame(QPixmap)));
}


void DiVA_detector_qt::on_pushDetener_clicked(){

	if (principal)
		this->principal->stop();
}

void DiVA_detector_qt::update_frame(const QPixmap &frame1)
{
	int i = 0;
	do{
		i++;
	}while (i<2);
	ui.display->setPixmap(frame1);
}

void DiVA_detector_qt::launchHelpWindow()
{
	this->HelpWindow.show();
}

void DiVA_detector_qt::launchAboutWindow()
{
	this->AboutWindow.show();
}

void DiVA_detector_qt::launchCamWindow()
{
	this->CamWindow.show();
}

void DiVA_detector_qt::updateCamInfo(camara nueva)
{
	this->_port = nueva.puerto;
	strcpy(this->_ip,nueva.ip);
	strcpy(this->_name,nueva.nombre);
	ui.camName->setText(_name);
	ui.camName->setGeometry(ui.camName->geometry().left(),ui.camName->geometry().top(),ui.camName->fontMetrics().boundingRect(ui.camName->text()).width(),ui.camName->geometry().height());

	this->setWindowTitle(QString(_name).append(" - Preservacion de privacidad"));
}

void DiVA_detector_qt::launchPropsWindow()
{
	if (!this->principal->Algor)
		return;

	connect(&this->PropsWindow,SIGNAL(sendParam(int,int,int,int,int,int,double,double,double,int)),this,SLOT(submitParam(int,int,int,int,int,int,double,double,double,int)));
	//this->PropsWindow.setParam(tam[0],tam[1],tam[2],tam[3],this->principal->Algor->getQ(),this->principal->Algor->getTime(),this->principal->Algor->getMHI_percentage(),this->algoritmo->Algor->getFG_Dec(),12);
	this->PropsWindow.show();
	
}

void DiVA_detector_qt::submitParam(int hmax,int hmin,int wmax,int wmin,int Q,int static_time,double MHI,double FG,double varnoise,int color)
{
	this->principal->Algor->color = color;
}

void DiVA_detector_qt::compactMode()
{
	if(ui.actionModo_compacto->isChecked())
	{
		ui.toolBar->hide();
		ui.camName->hide();
		this->setFixedSize(ui.display->geometry().width(),ui.display->geometry().height());
		ui.display->move(0,0);
	}
	else
	{
		ui.camName->show();
		ui.toolBar->show();
		this->set_window_elements_position(ui.display->geometry().width(),ui.display->geometry().height());
	}
}