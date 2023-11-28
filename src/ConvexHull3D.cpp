#include "ConvexHull3D.h"

ConvexHull::ConvexHull(std::vector<gp_Pnt> points) : m_points(points){
	m_Pconflict.reserve(points.size());
}

bool ConvexHull::initConflict() {
	int i = m_index;
	for (; i < m_points.size(); ++i)
	{
		for (int j = 0; j < m_dcel.faces.size(); ++j)
		{
			if (m_dcel.faces[j]->viuslPoint(m_points[i]))
			{
				//face j conflicts with point i
				m_dcel.faces[j]->conflict.push_back(i);
				m_Pconflict[i].push_back(j);
			}
		}
	}
	return true;
}

bool ConvexHull::initTetrahedon()
{
	m_index = 0;

	auto p1 = m_points[m_index];
	do
	{
		m_index++;
		if (m_index > m_points.size()) {
			return false;
		}
	} while (isDuplicate(p1, m_points[m_index]));

	auto p2 = m_points[m_index];
	do
	{
		m_index++;
		if (m_index > m_points.size()) {
			return false;
		}
	} while (isCollinear(p1, p2, m_points[m_index]));

	auto p3 = m_points[m_index];
	do
	{
		m_index++;
		if (m_index > m_points.size()) {
			return false;
		}
	} while (isCoplanar(p1, p2, p3, m_points[m_index]));
	auto p4 = m_points[m_index];
	m_index++;

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
	m_dcel.vertexes.insert(m_dcel.vertexes.end(), vs.begin(), vs.end());

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

	m_dcel.faces.push_back(face);
	addNewFace(e1, vs[3], nullptr, nullptr);
	addNewFace(e2, vs[3], e1, nullptr);
	addNewFace(e3, vs[3], e2, e1);
}

void ConvexHull::addNewFace(HalfEdge* current, HalfVertex* newVertex, HalfEdge* prev, HalfEdge* next) {

	auto fe1 = new HalfEdge(current->getNext()->getOrigin());
	auto fe2 = new HalfEdge(newVertex);
	auto fe3 = new HalfEdge(current->getOrigin());
	auto face = new HalfFace(fe1);
	fe1->setFace(face);
	fe2->setFace(face);
	fe3->setFace(face);
	current->setTwin(fe1);
	fe1->setNext(fe2);
	fe2->setNext(fe3);
	fe3->setNext(fe1);

	if (prev != nullptr) {
		prev->getTwin()->getPrev()->setTwin(fe2);
	}
	if (next != nullptr) {
		next->getTwin()->getNext()->setTwin(fe3);
	}

	m_dcel.faces.push_back(face);
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

bool ConvexHull::compute() {
	if (!initTetrahedon()) return false;
	initConflict();
	for (; m_index < m_points.size(); ++m_index)
	{
		addPoint(m_points[m_index]);
	}
}

void ConvexHull::addPoint(gp_Pnt point) {

}