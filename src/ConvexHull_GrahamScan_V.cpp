#include "ConvexHull_GrahamScan_V.h"

ConvexHull_GrahamScan_V::ConvexHull_GrahamScan_V(const Handle(AIS_InteractiveContext)& contex) : GeoAlgo_V(contex) {
	std::vector<gp_Pnt> points;
	points.reserve(50);
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0, 100);

	for (int i = 0; i < 50; ++i) {
		double x = dis(gen);
		double y = dis(gen);
		points.emplace_back(x, y, 0);
	}
	setPoints(points);
}

void ConvexHull_GrahamScan_V::displayPoints() {
	for (std::size_t i = 0; i < m_points.size(); ++i) {
		auto ais = new AIS_Point(new Geom_CartesianPoint(m_points[i]));
		m_contex->Display(ais, false);
		AIS_TextLabel* label = new AIS_TextLabel();
		label->SetText(std::to_string(i).c_str());
		label->SetPosition(m_points[i]);
		m_contex->Display(label, false);
	}
}
void ConvexHull_GrahamScan_V::removeLastLine()
{
	auto back = m_aisConvex.back();
	m_contex->Remove(back, false);
	m_aisConvex.pop_back();
}

void ConvexHull_GrahamScan_V::displayline(gp_Pnt s, gp_Pnt e)
{
	BRepBuilderAPI_MakeEdge edge(s,e);
	auto ais = new AIS_Shape(edge.Shape());
	ais->SetColor(Quantity_NOC_RED);
	m_aisConvex.push_back(ais);
	m_contex->Display(ais, false);
}

void ConvexHull_GrahamScan_V::update(int time) {
	Q_EMIT updateView();
	Sleep(time);
}

void ConvexHull_GrahamScan_V::process() {
	std::cout << "Start ConvexHull_GrahamScan...." << std::endl;

	if (!check()) {
		std::cout << "Chech data occur error" << std::endl;
	}
	displayPoints();
	update(1000);

	findLowest();
	auto lowest = new AIS_Point(new Geom_CartesianPoint(m_points[0]));
	Handle(Prs3d_PointAspect) aPointAspect = new Prs3d_PointAspect(Aspect_TOM_O_POINT, Quantity_NOC_GREEN, 3.0);
	lowest->SetMarker(Aspect_TOM_O);
	lowest->SetColor(Quantity_Color(1, 0, 0, Quantity_TOC_RGB));
	m_contex->Display(lowest, false);
	update(1000);

	sortByAngle();
	m_contex->RemoveAll(false);
	displayPoints();
	update(100);

	displayline(m_convexHull[0], m_convexHull[1]);
	displayline(m_convexHull[1], m_convexHull[2]);
	update(100);

	for (int i = 3; i < m_points.size(); i++) {
		while (toLeft(m_convexHull[m_convexHull.size() - 2], m_convexHull.back(), m_points[i]) <= 0) {
			removeLastLine();
			update(50);
			m_convexHull.pop_back();
		}
		m_convexHull.push_back(m_points[i]);
		displayline(m_convexHull[m_convexHull.size() - 2], m_convexHull.back());
		update(50);
	}
	displayline(m_convexHull[m_convexHull.size() - 2], m_convexHull.back());
	update(50);

	Q_EMIT finished();
}