#ifndef VENTANA_AYUDA_H
#define VENTANA_AYUDA_H

#include <QWidget>
#include "ui_ventana_ayuda.h"

class ventana_ayuda : public QWidget
{
	Q_OBJECT

public:
	ventana_ayuda(QWidget *parent = 0);
	~ventana_ayuda();

private:
	Ui::ventana_ayuda ui;
};

#endif // VENTANA_AYUDA_H
