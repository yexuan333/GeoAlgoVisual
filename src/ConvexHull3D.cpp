#include "ConvexHull3D.h"

bool ConvexHull::initTetrahedon()
{
	int i = 0;

	auto p1 = m_points[i];
	do
	{
		i++;
		if (i > m_points.size()) {
			return false;
		}
	} while (isDuplicate(p1, m_points[i]));

	auto p2 = m_points[i];
	do
	{
		i++;
		if (i > m_points.size()) {
			return false;
		}
	} while (isCollinear(p1, p2, m_points[i]));

	auto p3 = m_points[i];
	do
	{
		i++;
		if (i > m_points.size()) {
			return false;
		}
	} while (isCoplanar(p1, p2, p3, m_points[i]));
	auto p4 = m_points[i];

	auto vec1 = gp_Vec(p1, p2);
	auto vec2 = gp_Vec(p1, p3);
	auto vec3 = gp_Vec(p1, p4);

	bool isVisible = vec1.Crossed(vec2).Dot(vec3) > 0;
	std::vector<HalfVertex*> vs;
	if (isVisible) {
		vs.push_back(new HalfVertex(p1));
		vs.push_back(new HalfVertex(p3));
		vs.push_back(new HalfVertex(p2));
		vs.push_back(new HalfVertex(p4));
	}
	else {
		vs.push_back(new HalfVertex(p1));
		vs.push_back(new HalfVertex(p2));
		vs.push_back(new HalfVertex(p3));
		vs.push_back(new HalfVertex(p4));
	}
	auto e1 = new HalfEdge(vs[0]);
	auto e2 = new HalfEdge(vs[1]);
	auto e3 = new HalfEdge(vs[2]);

	e1->setNext(e2);
	e2->setNext(e3);
	e3->setNext(e1);
	auto face = new HalfFace(e1);
	e1->setFace(face);
	e2->setFace(face);
	e3->setFace(face);

	addNewFace(e1, vs[3], nullptr);
	addNewFace(e2, vs[3], e1);
	addNewFace(e3, vs[3], e2);
}

void ConvexHull::addNewFace(HalfEdge* share, HalfVertex* newVertex, HalfEdge* sharePrev) {

	auto fe1 = new HalfEdge(share->getNext()->getOrigin());
	auto fe2 = new HalfEdge(newVertex);
	auto fe3 = new HalfEdge(share->getOrigin());

	share->setTwin(fe1);
	fe1->setNext(fe2);
	fe2->setNext(fe3);
	fe3->setNext(fe1);
}

bool ConvexHull::isDuplicate(gp_Pnt p1, gp_Pnt p2)
{
	return p1.SquareDistance(p2) == 0;
}

bool ConvexHull::isCollinear(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3)
{
	gp_Vec v1(p2, p1);
	gp_Vec v2(p3, p1);
	return v1.Crossed(v2).SquareMagnitude() == 0;
}

bool ConvexHull::isCoplanar(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3, gp_Pnt p4)
{
	gp_Vec v1(p2, p1);
	gp_Vec v2(p3, p1);
	gp_Vec v3(p4, p1);

	return v1.Crossed(v2).Dot(v3) == 0;
}
