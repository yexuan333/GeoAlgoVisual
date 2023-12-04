#pragma once
#include "pch.h"
#include <set>

class HalfFace;
class HalfEdge;
class HalfVertex;

using HalfFace_ptr = std::shared_ptr<HalfFace>;
using HalfEdge_ptr = std::shared_ptr<HalfEdge>;
using HalfVertex_ptr = std::shared_ptr<HalfVertex>;

class HalfVertex
{
public:
    HalfVertex(){}
    HalfVertex(gp_Pnt p):point(p){}
    gp_Pnt point;
    std::vector<int> conflict;
};

class HalfEdge
{
public:
    HalfEdge(HalfVertex_ptr v){
        origin = v;
    }
    HalfVertex_ptr origin = nullptr;
    HalfEdge_ptr next = nullptr;
    HalfEdge_ptr prev = nullptr;
    HalfEdge_ptr twin = nullptr;
    HalfFace_ptr face = nullptr;
};

class HalfFace
{
public:
    HalfFace(HalfEdge_ptr e){
        edge = e;
    }
    bool viuslPoint(gp_Pnt p) {
        auto v1 = gp_Vec(edge->prev->origin->point, edge->origin->point);
        auto v2 = gp_Vec(edge->origin->point, edge->next->origin->point);
        auto v3 = gp_Vec(edge->origin->point, p);
        return v1.Crossed(v2).Dot(v3);
    }
    HalfEdge_ptr edge;
    std::vector<int> conflict;
    bool vaild = true;
};

/// <summary>
/// doubly connected edge list
/// </summary>
class DCEL
{
public:
    void cleanUp(){}
    std::vector<HalfFace_ptr> faces;
    std::vector<HalfVertex_ptr> vertexes;
};

class ConvexHull3D_Incremental
{
public:
    ConvexHull3D_Incremental(std::vector<gp_Pnt> points);
    ~ConvexHull3D_Incremental(){}
    bool compute();
    DCEL getResult();
    bool initTetrahedon();
    bool initConflict();
    bool isDuplicate(gp_Pnt p1, gp_Pnt p2);
    bool isCollinear(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3);
    bool isCoplanar(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3, gp_Pnt p4);
    HalfFace_ptr addNewFace(HalfEdge_ptr current, HalfVertex_ptr newVertex);
    void addPoint(int index);
protected:
    std::vector<gp_Pnt> m_points;
    std::vector<std::vector<int>> m_Pconflict;
    DCEL m_dcel;
    int m_index = 0;
};
