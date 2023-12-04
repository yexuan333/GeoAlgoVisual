#pragma once
#include "pch.h"
#include "GeoAlgo_V.h"
#include "ConvexHull3D_Incremental.h"

class ConvexHull3D_Incremental_V : public GeoAlgo_V
{
	Q_OBJECT
public:
	ConvexHull3D_Incremental_V(const Handle(AIS_InteractiveContext)& contex);
	~ConvexHull3D_Incremental_V() {}

	void process() override;

private:
	void displayDCEL(DCEL dcel);
	//const Handle(AIS_InteractiveContext)& m_contex;
	std::vector<AIS_Shape*> m_aisConvex;
	std::vector<gp_Pnt> m_points;
};