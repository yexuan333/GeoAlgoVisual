#include "ConvexHull3D_Incremental.h"

ConvexHull3D_Incremental::ConvexHull3D_Incremental(std::vector<gp_Pnt> points) : m_points(points){
	m_Pconflict.resize(points.size());
}

bool ConvexHull3D_Incremental::initConflict() {
	int i = m_index;
	for (; i < m_points.size(); ++i)
	{
		for (int j = 0; j < m_dcel.faces.size(); ++j)
		{
			if (m_dcel.faces[j]->viuslPoint(m_points[i]) > 0)
			{
				//face j conflicts with point i
				m_dcel.faces[j]->conflict.push_back(i);
				m_Pconflict[i].push_back(j);
			}
		}
	}
	return true;
}

bool ConvexHull3D_Incremental::initTetrahedon()
{
	m_index = 0;

	auto p1 = m_points[m_index];
	auto v1 = std::make_shared<HalfVertex>(p1);
	v1->index = 0;
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
	auto vec2 = gp_Vec(p2, p3);
	auto vec3 = gp_Vec(p1, p4);

	bool isVisible = vec1.Crossed(vec2).Dot(vec3) > 0;
	std::vector<HalfVertex_ptr> vs;
	if (isVisible) {
		auto v2 = std::make_shared<HalfVertex>(p3); v2->index = 2;
		auto v3 = std::make_shared<HalfVertex>(p2); v3->index = 1;
		auto v4 = std::make_shared<HalfVertex>(p4); v4->index = 3;
		vs.push_back(v1);
		vs.push_back(v2);
		vs.push_back(v3);
		vs.push_back(v4);
	}
	else {
		auto v2 = std::make_shared<HalfVertex>(p2); v2->index = 1;
		auto v3 = std::make_shared<HalfVertex>(p3); v3->index = 2;
		auto v4 = std::make_shared<HalfVertex>(p4); v4->index = 3;
		vs.push_back(v1);
		vs.push_back(v2);
		vs.push_back(v3);
		vs.push_back(v4);
	}
	m_dcel.vertexes.insert(m_dcel.vertexes.end(), vs.begin(), vs.end());
	
	auto e1 = std::make_shared<HalfEdge>(vs[0]);
	auto e2 = std::make_shared<HalfEdge>(vs[1]);
	auto e3 = std::make_shared<HalfEdge>(vs[2]);

	e1->next = e2; e2->prev = e1;
	e2->next = e3; e3->prev = e2;
	e3->next = e1; e1->prev = e3;
	auto face = std::make_shared<HalfFace>(e1);
	e1->face = face;
	e2->face = face;
	e3->face = face;
	face->index = 0;
	addFace(face);
	addNewFace(e1, vs[3],nullptr, nullptr);
	addNewFace(e2, vs[3],e1, nullptr);
	addNewFace(e3, vs[3],e2,e1);
}
void ConvexHull3D_Incremental::addFace(HalfFace_ptr face) {
	m_dcel.faces.push_back(face);
}
HalfFace_ptr ConvexHull3D_Incremental::addNewFace(HalfEdge_ptr current, HalfVertex_ptr newVertex, HalfEdge_ptr prev, HalfEdge_ptr next) {
	auto fe1 = std::make_shared<HalfEdge>(current->next->origin);
	auto fe2 = std::make_shared<HalfEdge>(current->origin);
	auto fe3 = std::make_shared<HalfEdge>(newVertex);

	auto face = std::make_shared<HalfFace>(fe1);
	fe1->face = face;
	fe2->face = face;
	fe3->face = face;
	current->twin = fe1; fe1->twin = current;
	fe1->next = fe2; fe2->prev = fe1;
	fe2->next = fe3; fe3->prev = fe2;
	fe3->next = fe1; fe1->prev = fe3;

	if(prev != nullptr){
		fe2->twin = prev->twin->prev;
		prev->twin->prev->twin = fe2;
	}

	if (next != nullptr) {
		fe3->twin = next->twin->next;
		next->twin->next->twin = fe3;
	}
	
	face->index = m_dcel.faces.size();
	addFace(face);
	return face;
}

bool ConvexHull3D_Incremental::isDuplicate(gp_Pnt p1, gp_Pnt p2)
{
	return p1.SquareDistance(p2) == 0;
}

bool ConvexHull3D_Incremental::isCollinear(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3)
{
	gp_Vec v1(p1, p2);
	gp_Vec v2(p1, p3);
	return v1.Crossed(v2).SquareMagnitude() == 0;
}

bool ConvexHull3D_Incremental::isCoplanar(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3, gp_Pnt p4)
{
	gp_Vec v1(p1, p2);
	gp_Vec v2(p1, p3);
	gp_Vec v3(p1, p4);

	return v1.Crossed(v2).Dot(v3) == 0;
}

bool ConvexHull3D_Incremental::compute() {
	if (!initTetrahedon()) return false;
	initConflict();
	for (; m_index < m_points.size(); ++m_index)
	{
		addPoint(m_index);
	}
}
DCEL ConvexHull3D_Incremental::getResult() {
	return m_dcel;
}

int ConvexHull3D_Incremental::find(std::vector<HalfEdge_ptr> H, int index)
{
	int i, s;
	for (s = 1; s < (int)H.size(); s <<= 1);
	for (i = 0; s; s >>= 1)
		if (i + s < (int)H.size() && H[i + s]->origin->index <= index)
			i += s;
	return i;
}

void ConvexHull3D_Incremental::findHorizon(int index, std::vector<HalfEdge_ptr>& horizon) {
	for (int i = 0; i < m_Pconflict[index].size(); ++i)
	{
		auto conflictFace = m_dcel.faces[m_Pconflict[index][i]];
		if (!conflictFace->vaild) continue;

		conflictFace->vaild = false;
		auto currentEdge = conflictFace->edge;
		do
		{
			// if twin face don't visual point , the edge is horizon
			if (currentEdge->twin->face->viuslPoint(m_points[index]) <= 0) {
				horizon.push_back(currentEdge->twin);
			}
			currentEdge = currentEdge->next;
		} while (currentEdge != conflictFace->edge);
	}
}

void ConvexHull3D_Incremental::handleAddCoplanarFace(HalfEdge_ptr horizon, HalfFace_ptr newFace, HalfFace_ptr sideFace) {
	// coplanar
	horizon->twin->next->prev = horizon->prev;
	horizon->prev->next = horizon->twin->next;

	horizon->twin->prev->next = horizon->next;
	horizon->next->prev = horizon->twin->prev;

	//reset face links
	for (auto iter = horizon->twin->next; iter != horizon->next; iter = iter->next)
		iter->face = sideFace;

	newFace->vaild = false;
	//newFace->edge = horizonSorted[i]->next;
}

void ConvexHull3D_Incremental::addPoint(int index) {
	if (m_Pconflict[index].empty()) return;
	std::vector<HalfEdge_ptr> horizon;
	findHorizon(index, horizon);

	std::vector<HalfEdge_ptr> horizonSorted;
	if (horizon.size()) {
		std::sort(horizon.begin(), horizon.end(), [](HalfEdge_ptr a, HalfEdge_ptr b) {
			return a->origin->index < b->origin->index;
			});
		int pos = 0;
		do
		{
			horizonSorted.push_back(horizon[pos]);
			pos = find(horizon, horizonSorted[horizonSorted.size() - 1]->next->origin->index);
		} while (pos != 0);
	}
	else return;

	auto newV = std::make_shared<HalfVertex>(m_points[index]);
	newV->index = index;
	for (int i = 0; i < horizonSorted.size(); i++) {
		auto sideFace = horizonSorted[i]->face;
		auto removeFace = horizonSorted[i]->twin->face;
		HalfFace_ptr newFace;
		if (i == 0) {
			newFace = addNewFace(horizonSorted[i], newV,nullptr, nullptr);
		}
		else if (i == horizonSorted.size() - 1) {
			newFace = addNewFace(horizonSorted[i], newV, horizonSorted[i-1], horizonSorted[0]);
		}
		else {
			newFace = addNewFace(horizonSorted[i], newV, horizonSorted[i - 1], nullptr);
		}

		if (sideFace->viuslPoint(m_points[index]) == 0) {
			handleAddCoplanarFace(horizonSorted[i], newFace, sideFace);
		}
		else {
			std::set<int> newConflict;
			for (auto iter = sideFace->conflict.begin(); iter != sideFace->conflict.end(); iter++)
				newConflict.insert(*iter);
			for (auto iter = removeFace->conflict.begin(); iter != removeFace->conflict.end(); iter++)
				newConflict.insert(*iter);

			for (auto iter = newConflict.begin(); iter != newConflict.end(); ++iter)
			{
				if (*iter <= index) continue;

				if (newFace->viuslPoint(m_points[*iter]) > 0) {
					newFace->conflict.push_back(*iter);
					m_Pconflict[*iter].push_back(m_dcel.faces.size() - 1);
				}

			}
		}
	}
}
