#include "ConvexHull3D_V.h"


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