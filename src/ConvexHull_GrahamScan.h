#pragma once
#include "pch.h"

class ConvexHull_GrahamScan
{	
public:
	ConvexHull_GrahamScan(std::vector<gp_Pnt> points):m_points(points){}
	~ConvexHull_GrahamScan() {}
    bool compute() {
        if (m_points.size() < 3) return false;
        int minY = m_points[0].Y(), minIndex = 0;
        for (int i = 1; i < m_points.size(); i++) {
            int y = m_points[i].Y();
            if ((y < minY) || (minY == y && m_points[i].X() < m_points[minIndex].X())) {
                minY = m_points[i].Y();
                minIndex = i;
            }
        }
        std::swap(m_points[0], m_points[minIndex]);
       
        std::sort(m_points.begin() + 1, m_points.end(), [&](gp_Pnt a, gp_Pnt b) {
            int dy1 = a.Y() - m_points[0].Y();
            int dx1 = a.X() - m_points[0].X();
            int dy2 = b.Y() - m_points[0].Y();
            int dx2 = b.X() - m_points[0].X();
            return atan2(dy1, dx1) < atan2(dy2, dx2);
            });

       
        m_convexHull.push_back(m_points[0]);
        m_convexHull.push_back(m_points[1]);
        m_convexHull.push_back(m_points[2]);

        for (int i = 3; i < m_points.size(); i++) {
            while (toLeft(m_convexHull[m_convexHull.size() - 2], m_convexHull.back(), m_points[i]) <= 0) {
                m_convexHull.pop_back();
            }
            m_convexHull.push_back(m_points[i]);
        }
        return true;
    }
    std::vector<gp_Pnt> getResult() { return m_convexHull; }
    double toLeft(gp_Pnt prev, gp_Pnt current, gp_Pnt next) {
        return (gp_Vec(prev, current) ^ gp_Vec(prev, next)).Z();
    }
private:
    std::vector<gp_Pnt> m_points;
    std::vector<gp_Pnt> m_convexHull;
};
