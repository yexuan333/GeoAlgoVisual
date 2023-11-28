#pragma once
#include "pch.h"
#include "GeoAlgo_V.h"
#include "ConvexHull_GrahamScan.h"

class ConvexHull_GrahamScan_V : public GeoAlgo_V,  public ConvexHull_GrahamScan
{
public:
	ConvexHull_GrahamScan_V(const Handle(AIS_InteractiveContext)& contex);
	~ConvexHull_GrahamScan_V() {}
	void process() override;
private:
	void displayPoints();
	void displayline(gp_Pnt s, gp_Pnt e);
	void removeLastLine();
	void update(int time);
	std::vector<AIS_Shape*> m_aisConvex;
};