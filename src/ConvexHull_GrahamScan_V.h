#pragma once
#include "pch.h"

#include "ConvexHull_GrahamScan.h"

class ConvexHull_GrahamScan_V : public QObject, public ConvexHull_GrahamScan
{
	Q_OBJECT
public:
	ConvexHull_GrahamScan_V(const Handle(AIS_InteractiveContext)& contex,std::vector<gp_Pnt> points)
		:ConvexHull_GrahamScan(points),m_contex(contex) {}
	~ConvexHull_GrahamScan_V() {}


public Q_SLOTS:
	void process();

Q_SIGNALS:
	void finished();
	void error(QString err);
	void updateView();
	void clearView();

private:
	void displayPoints();
	void displayline(gp_Pnt s, gp_Pnt e);
	void removeLastLine();
	void update(int time);
	const Handle(AIS_InteractiveContext)& m_contex;
	std::vector<AIS_Shape*> m_aisConvex;
};