#pragma once
#include "pch.h"
#include "GeoAlgo_V.h"
#include "ConvexHull3D_Incremental.h"

class ConvexHull3D_Incremental_V : public GeoAlgo_V, public ConvexHull3D_Incremental
{
	Q_OBJECT
public:
	ConvexHull3D_Incremental_V(const Handle(AIS_InteractiveContext)& contex);
	~ConvexHull3D_Incremental_V() {}

	void process() override;
private:
	void addFace(HalfFace_ptr face) override;
	void handleAddCoplanarFace(HalfEdge_ptr horizon, HalfFace_ptr newFace, HalfFace_ptr sideFace) override;
	void findHorizon(int index, std::vector<HalfEdge_ptr>& horizon) override;
	void displayPoints();
	void makeDisplayEdgePoint(HalfEdge_ptr edge, gp_Pnt center, gp_Pnt& first, gp_Pnt& last);
	void drawFace(HalfFace_ptr halfFace);
	void eraseFace(HalfFace_ptr halfFace);
	void displayDCEL(DCEL dcel);
	//const Handle(AIS_InteractiveContext)& m_contex;
	std::vector<AIS_Shape*> m_aisFace;
	std::vector<AIS_Shape*> m_aisHorizon;
};