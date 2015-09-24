#include "ventana_camaras.h"
#include <qstring.h>
#include <qdebug.h>


ventana_camaras::ventana_camaras(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setFixedSize(320,240);
	this->lista = new cam_list("cam.conf");
	this->lista->readFileCameras();
	this->newcam = 0;

	for(int i=0;i<lista->listado.size();i++)
	{
		if (i==0)
		{
			ui.camselector->addItem(QString::QString(lista->listado[i].nombre).append(" <default>"));
		}
		else
		{
			ui.camselector->addItem(lista->listado[i].nombre);
		}
	}
	
	if(!this->lista->listado.size())
	{
		ui.editcam->setEnabled(false);
	}
	else
	{
		ui.editcam->setEnabled(true);
	}

}

ventana_camaras::~ventana_camaras()
{

}


void ventana_camaras::on_acept_clicked()
{
	if(this->lista->listado.empty())
	{
		camara cam_aux;
		strcpy(cam_aux.nombre,"<No selected camera>");
		strcpy(cam_aux.ip,"-1");
		cam_aux.puerto = -1;
		emit selectedCamera(cam_aux);
		this->close();
	}
	else{
	emit selectedCamera(this->lista->listado[ui.camselector->currentIndex()]);//.puerto,this->lista->listado[ui.camselector->currentIndex()].ip);
	this->close();
	}
}

void ventana_camaras::on_cancel_clicked()
{
	this->close();
}

void ventana_camaras::on_delcam_clicked()
{
	QString nombre = this->lista->listado[ui.camselector->currentIndex()].nombre;
	QByteArray byteArray = nombre.toUtf8();
	const char* cString = byteArray.constData();
	this->lista->delCamera((char * )cString);
	ui.camselector->removeItem(ui.camselector->currentIndex());
	if(!this->lista->listado.size())
	{
		ui.editcam->setEnabled(false);
	}
	else
	{
		ui.editcam->setEnabled(true);
	}
}

void ventana_camaras::on_editcam_clicked()
{
	this->resizeWindow(640,240);
	ui.namein->setEnabled(true);
	ui.portin->setEnabled(true);
	ui.ipin->setEnabled(true);
	ui.savechange->setEnabled(true);
	ui.camselector->setEnabled(false);
	ui.namein->setText(QString::QString(this->lista->listado[ui.camselector->currentIndex()].nombre));
	ui.ipin->setText(QString::QString(this->lista->listado[ui.camselector->currentIndex()].ip));
	ui.portin->setText(QString::number(this->lista->listado[ui.camselector->currentIndex()].puerto));
	this->newcam = 0;
}

void ventana_camaras::on_newcam_clicked()
{
	this->resizeWindow(640,240);
	ui.namein->setEnabled(true);
	ui.portin->setEnabled(true);
	ui.ipin->setEnabled(true);	
	ui.namein->clear();
	ui.portin->clear();
	ui.ipin->clear();
	ui.savechange->setEnabled(true);
	ui.camselector->setEnabled(true);

	this->newcam=1;
	if(!this->lista->listado.size())
	{
		ui.editcam->setEnabled(false);
	}
	else
	{
		ui.editcam->setEnabled(true);
	}
}


void ventana_camaras::resizeWindow(int w,int h)
{
	this->setFixedSize(w,h);
	//this->setGeometry(this->x()+8,this->y()+30,w,h);

}

void ventana_camaras::on_savechange_clicked()
{
	if(!this->newcam)
	{
		QString nombre = ui.camselector->currentText();
		QByteArray byteArray = nombre.toUtf8();
		const char* cString = byteArray.constData();
		this->lista->delCamera((char * )cString);
		ui.camselector->removeItem(ui.camselector->currentIndex());
	}

	QString name = ui.namein->text();
	QByteArray byteArray2 = name.toUtf8();
	const char* char_name = byteArray2.constData();

	QString ip = ui.ipin->text();
	QByteArray byteArray3 = ip.toUtf8();
	const char* char_ip = byteArray3.constData();

	int int_port = ui.portin->text().toInt();

	this->lista->addCamera((char*)char_ip,int_port,(char*)char_name);
	this->lista->updateFileCameras();
	ui.camselector->addItem(name);

	this->resizeWindow(320,240);
	ui.namein->clear();
	ui.portin->clear();
	ui.ipin->clear();
	ui.namein->setEnabled(false);
	ui.portin->setEnabled(false);
	ui.ipin->setEnabled(false);
	ui.savechange->setEnabled(false);
	ui.camselector->setEnabled(true);
	this->newcam = 0;

	if(!this->lista->listado.size())
	{
		ui.editcam->setEnabled(false);
	}
	else
	{
		ui.editcam->setEnabled(true);
	}



}


void ventana_camaras::on_pushDefault_clicked()
{
	this->lista->selectDefaultCam(ui.camselector->currentIndex());
	delete lista;
	ui.camselector->clear();
	lista = new cam_list("cam.conf");
	this->lista->readFileCameras();

	for(int i=0;i<lista->listado.size();i++)
	{
		if (i==0)
		{
			ui.camselector->addItem(QString::QString(lista->listado[i].nombre).append(" <default>"));
		}
		else
		{
			ui.camselector->addItem(lista->listado[i].nombre);
		}
	}
	
	if(!this->lista->listado.size())
	{
		ui.editcam->setEnabled(false);
	}
	else
	{
		ui.editcam->setEnabled(true);
	}
}