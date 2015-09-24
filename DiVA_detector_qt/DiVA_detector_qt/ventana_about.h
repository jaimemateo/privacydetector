#ifndef VENTANA_ABOUT_H
#define VENTANA_ABOUT_H

#include <QWidget>
#include "ui_ventana_about.h"

class ventana_about : public QWidget
{
	Q_OBJECT

public:
	ventana_about(QWidget *parent = 0);
	~ventana_about();

private:
	Ui::ventana_about ui;
};

#endif // VENTANA_ABOUT_H
