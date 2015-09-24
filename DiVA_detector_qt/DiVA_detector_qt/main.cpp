#include "diva_detector_qt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	DiVA_detector_qt w;
	w.show();
	return a.exec();
}
