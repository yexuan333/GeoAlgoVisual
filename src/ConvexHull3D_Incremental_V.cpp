#include "ConvexHull3D_Incremental_V.h"
#include <BRepGProp_Face.hxx>

ConvexHull3D_Incremental_V::ConvexHull3D_Incremental_V(const Handle(AIS_InteractiveContext)& contex) :GeoAlgo_V(contex),ConvexHull3D_Incremental()
{
	int num = 50;
	m_points.reserve(num);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 100);

	for (int i = 0; i < num; ++i) {
		double x = dis(gen);
		double y = dis(gen);
		double z = dis(gen);
		m_points.emplace_back(x, y, z);
	}
	m_Pconflict.resize(m_points.size());
	//m_points.push_back(gp_Pnt(0, 0, 0));
	//m_points.push_back(gp_Pnt(100, 100, 0));
	//m_points.push_back(gp_Pnt(200, 0, 0));
	//m_points.push_back(gp_Pnt(100, 150, 50));
	//m_points.push_back(gp_Pnt(400, -20, 20));
	//m_points.push_back(gp_Pnt(-100, -100, 100));
	//m_points.push_back(gp_Pnt(200, -100, 100));
}

void ConvexHull3D_Incremental_V::makeDisplayEdgePoint(HalfEdge_ptr edge, gp_Pnt center, gp_Pnt& first, gp_Pnt& last) {
	first = edge->origin->point;
	last = edge->next->origin->point;
	gp_Vec tan(first, last);
	tan.Normalize();
	tan *= 10;
	gp_Vec curToCenter(first, center);
	curToCenter.Normalize();
	curToCenter *= 5;
	first.Translate(curToCenter);
	first.Translate(tan);
	gp_Vec lastToCenter(last, center);
	lastToCenter.Normalize();
	lastToCenter *= 5;
	last.Translate(-tan);
	last.Translate(lastToCenter);

}


void ConvexHull3D_Incremental_V::drawFace(HalfFace_ptr halfFace) {
	if (!halfFace->vaild) return;

	BRepBuilderAPI_MakePolygon polygonBuilder;
	auto iter = halfFace->edge;
	do
	{
		auto current = iter->origin->point;
		polygonBuilder.Add(current);
		iter = iter->next;
	} while (iter != halfFace->edge);
	polygonBuilder.Close();
	auto wire = polygonBuilder.Wire();
	auto face = BRepBuilderAPI_MakeFace(wire, true);
	auto aisFaceShaded = new AIS_Shape(face.Shape());
	auto aisFaceFrame = new AIS_Shape(face.Shape());
	//aisFaceShaded->SetTransparency(0.2);
	m_contex->Display(aisFaceShaded, AIS_Shaded, 0, false);
	//m_contex->Display(aisFaceFrame, AIS_WireFrame, 0, false);
	m_aisFace.push_back(aisFaceShaded);
	BRepGProp_Face faceProps(face.Face());
	GProp_GProps properties;
	BRepGProp::SurfaceProperties(face, properties);
	auto center = properties.CentreOfMass();

	//auto edge = halfFace->edge;
	//do
	//{
	//	gp_Pnt curF, curL;
	//	makeDisplayEdgePoint(edge, center, curF, curL);
	//	auto curMid = curF.Translated(gp_Vec(curF, curL) * 0.4);
	//	auto aisCurEdge = new AIS_Shape(BRepBuilderAPI_MakeEdge(curF, curL).Shape());
	//	m_contex->Display(aisCurEdge, AIS_WireFrame, 0, false);

	//	if (edge->twin != nullptr) {
	//		gp_Pnt twinL = edge->twin->origin->point;
	//		gp_Pnt twinF = edge->twin->next->origin->point;
	//		auto twinMid = twinF.Translated(gp_Vec(twinF, twinL) * 0.4);
	//		auto aisTwinEdge = new AIS_Shape(BRepBuilderAPI_MakeEdge(curMid, twinMid).Shape());
	//		m_contex->Display(aisTwinEdge, AIS_WireFrame, 0, false);
	//	}


	//	edge = edge->next;
	//} while (edge != halfFace->edge);

	//AIS_TextLabel* label = new AIS_TextLabel();
	//std::stringstream stream;
	//stream << "F:" << halfFace->index;
	//label->SetText(stream.str().c_str());
	//label->SetPosition(center);
	//m_contex->Display(label, false);
	update(200);
}

void ConvexHull3D_Incremental_V::eraseFace(HalfFace_ptr halfFace)
{
	m_contex->Remove(m_aisFace[halfFace->index], false);
	update(200);
}

void ConvexHull3D_Incremental_V::displayDCEL(DCEL dcel) {
	for (auto halfFace :dcel.faces)
	{
		drawFace(halfFace);
	}
	update(30);
}

void ConvexHull3D_Incremental_V::addFace(HalfFace_ptr face)
{
	ConvexHull3D_Incremental::addFace(face);
	drawFace(face);
}

void ConvexHull3D_Incremental_V::handleAddCoplanarFace(HalfEdge_ptr horizon, HalfFace_ptr newFace, HalfFace_ptr sideFace)
{
	ConvexHull3D_Incremental::handleAddCoplanarFace(horizon, newFace, sideFace);
	eraseFace(newFace);
	drawFace(sideFace);
	return;
}

void ConvexHull3D_Incremental_V::findHorizon(int index, std::vector<HalfEdge_ptr>& horizon)
{
	if (!m_aisHorizon.empty()) {
		for (auto iter = m_aisHorizon.begin(); iter != m_aisHorizon.end(); iter++)
		{
			m_contex->Erase(*iter, false);
		}
	}

	for (int i = 0; i < m_Pconflict[index].size(); ++i)
	{
		auto conflictFace = m_dcel.faces[m_Pconflict[index][i]];
		if (conflictFace->vaild) {
			m_aisFace[conflictFace->index]->SetColor(Quantity_Color(1, 0, 0, Quantity_TOC_RGB));
			m_contex->Redisplay(m_aisFace[conflictFace->index], false);
		}
	}
	update(500);
	for (int i = 0; i < m_Pconflict[index].size(); ++i)
	{
		auto conflictFace = m_dcel.faces[m_Pconflict[index][i]];
		if (conflictFace->vaild) {
			m_contex->Erase(m_aisFace[conflictFace->index], false);
		}
	}
	update(200);
	ConvexHull3D_Incremental::findHorizon(index, horizon);


	for (auto iter = horizon.begin(); iter != horizon.end(); iter++)
	{
		auto aisCurEdge = new AIS_Shape(BRepBuilderAPI_MakeEdge((*iter)->origin->point, (*iter)->next->origin->point).Shape());
		m_aisHorizon.push_back(aisCurEdge);
		m_contex->Display(aisCurEdge, AIS_WireFrame, 0, false);
	}
	update(200);
}

void ConvexHull3D_Incremental_V::displayPoints() {
	for (std::size_t i = 0; i < m_points.size(); ++i) {
		auto ais = new AIS_Point(new Geom_CartesianPoint(m_points[i]));
		m_contex->Display(ais, false);
		AIS_TextLabel* label = new AIS_TextLabel();
		label->SetText(std::to_string(i).c_str());
		label->SetPosition(m_points[i]);
		m_contex->Display(label, false);
	}
	update(200);
}

void ConvexHull3D_Incremental_V::process() {
	displayPoints();
	compute();
	//auto dcel = getResult();
	//displayDCEL(dcel);
	finished();
}