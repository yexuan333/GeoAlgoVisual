#include "ConvexHull3D_V.h"

ConvexHull3D_V::ConvexHull3D_V(const Handle(AIS_InteractiveContext)& contex) :GeoAlgo_V(contex) 
{
	//std::vector<gp_Pnt> points;
	//points.reserve(50);
	//std::random_device rd;
	//std::mt19937 gen(rd());
	//std::uniform_real_distribution<> dis(0, 100);

	//for (int i = 0; i < 50; ++i) {
	//	double x = dis(gen);
	//	double y = dis(gen);
	//	points.emplace_back(x, y, 0);
	//}
	m_points.push_back(gp_Pnt(0, 0, 0));
	m_points.push_back(gp_Pnt(10, 10, 0));
	m_points.push_back(gp_Pnt(20, 0, 0));
	m_points.push_back(gp_Pnt(10, 5, 0));
}

point topoint(gp_Pnt p) {
	point p1;
	p1.coord[0] = p.X();
	p1.coord[1] = p.Y();
	p1.coord[2] = p.Z();
	return p1;
}

void ConvexHull3D_V::process() {
	auto p = new point[m_points.size()];
	for (size_t i = 0; i < m_points.size(); i++)
	{
		p[i] = topoint(m_points[i]);
	}
	ConvexHull op;
	auto dcel = op.getConvexHull(m_points.size() - 1, p);
}