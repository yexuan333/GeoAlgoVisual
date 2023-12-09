#pragma once
#include "pch.h"

class GeoAlgo_V : public QObject
{
	Q_OBJECT
public:
	GeoAlgo_V(const Handle(AIS_InteractiveContext)& contex):m_contex(contex), m_isValid(false){}
	void update(int time) {
		Q_EMIT updateView();
		Sleep(time);
	}
public Q_SLOTS:
	virtual void process() = 0;
Q_SIGNALS:
	void finished();
	void error(QString err);
	void updateView();
	void clearView();
protected:
	bool m_isValid;
	const Handle(AIS_InteractiveContext) m_contex;
};

