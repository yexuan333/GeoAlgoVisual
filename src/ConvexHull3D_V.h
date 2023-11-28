#pragma once
#include "pch.h"
#include "GeoAlgo_V.h"
#include "ch.h"

class ConvexHull3D_V : public GeoAlgo_V
{
	Q_OBJECT
public:
	ConvexHull3D_V(const Handle(AIS_InteractiveContext)& contex);
	~ConvexHull3D_V() {}

	void process() override;

private:
	//const Handle(AIS_InteractiveContext)& m_contex;
	std::vector<AIS_Shape*> m_aisConvex;
	std::vector<gp_Pnt> m_points;
};