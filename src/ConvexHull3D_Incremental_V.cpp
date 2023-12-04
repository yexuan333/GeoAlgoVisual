#include "ConvexHull3D_Incremental_V.h"

ConvexHull3D_Incremental_V::ConvexHull3D_Incremental_V(const Handle(AIS_InteractiveContext)& contex) :GeoAlgo_V(contex)
{
	//m_points.reserve(50);
	//std::random_device rd;
	//std::mt19937 gen(rd());
	//std::uniform_real_distribution<> dis(0, 100);

	//for (int i = 0; i < 50; ++i) {
	//	double x = dis(gen);
	//	double y = dis(gen);
	//	double z = dis(gen);
	//	m_points.emplace_back(x, y, z);
	//}
	m_points.push_back(gp_Pnt(0, 0, 0));
	m_points.push_back(gp_Pnt(100, 100, 0));
	m_points.push_back(gp_Pnt(200, 0, 0));
	m_points.push_back(gp_Pnt(100, 50, 50));
	m_points.push_back(gp_Pnt(100, 50, 150));
	m_points.push_back(gp_Pnt(100, 100, 100));
}

//point topoint(gp_Pnt p) {
//	point p1;
//	p1.coord[0] = p.X();
//	p1.coord[1] = p.Y();
//	p1.coord[2] = p.Z();
//	return p1;
//}
void ConvexHull3D_Incremental_V::displayDCEL(DCEL dcel) {
	for (auto face :dcel.faces)
	{
		if (!face->vaild) continue;

		BRepBuilderAPI_MakePolygon polygonBuilder;
		auto iter = face->edge;
		do
		{
			auto current = iter->origin->point;
			polygonBuilder.Add(current);
			iter = iter->next;
		} while (iter != face->edge);
		polygonBuilder.Close();
		auto wire = polygonBuilder.Wire();
		auto face = BRepBuilderAPI_MakeFace(wire, true);
		auto aisFace = new AIS_Shape(face.Shape());

		m_contex->Display(aisFace,AIS_Shaded,0,false);
	}
	updateView();
}

void ConvexHull3D_Incremental_V::process() {
	//auto p = new point[m_points.size()];
	//for (size_t i = 0; i < m_points.size(); i++)
	//{
	//	p[i] = topoint(m_points[i]);
	//}
	ConvexHull3D_Incremental op(m_points);
	op.compute();
	auto dcel = op.getResult();
	displayDCEL(dcel);
	finished();
}