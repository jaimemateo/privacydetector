#ifndef DIVA_QTSIGNALS_H
#define DIVA_QTSIGNALS_H

#include <QObject>


class DiVA_QtSignals : public QObject
{
Q_OBJECT
public:
	DiVA_QtSignals(){};
	~DiVA_QtSignals(){};
	
signals:
	void refresh_display(const QPixmap &frame);
	void refresh_display_comparation(const QPixmap &_template, const QPixmap &result);
	void changeValue(int value);
};

#endif