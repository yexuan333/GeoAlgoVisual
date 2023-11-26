#pragma once
#include "pch.h"
#include "ch.h"

class ConvexHull3D_V : public QObject
{
	Q_OBJECT
public:
	ConvexHull3D_V(const Handle(AIS_InteractiveContext)& contex, std::vector<gp_Pnt> points)
		:m_points(points),m_contex(contex) {}
	~ConvexHull3D_V() {}

public Q_SLOTS:
	void process();

Q_SIGNALS:
	void finished();
	void error(QString err);
	void updateView();
	void clearView();

private:
	//void displayPoints();
	//void displayline(gp_Pnt s, gp_Pnt e);
	//void removeLastLine();
	//void update(int time);
	const Handle(AIS_InteractiveContext)& m_contex;
	std::vector<AIS_Shape*> m_aisConvex;
	std::vector<gp_Pnt> m_points;
};