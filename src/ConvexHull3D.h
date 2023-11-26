#pragma once
#include "pch.h"

class HalfFace;
class HalfEdge;

class HalfVertex
{
public:
    HalfVertex(){}
    HalfVertex(gp_Pnt p):point(p){}
    gp_Pnt point;
};

class HalfEdge
{
public:
    HalfEdge(HalfVertex* v){
        origin = v;
    }
    HalfVertex* getOrigin() { return origin; }

    void setNext(HalfEdge* e) { next = e; e->setPrev(this); }
    HalfEdge* getNext() {return next; }
    void setPrev(HalfEdge* e) { prev = e; e->setNext(this); }
    HalfEdge* getPrev() { return prev; }
    void setTwin(HalfEdge* e) { twin = e; e->setTwin(this); }
    HalfEdge* getTwin() { return twin; }
    void setFace(HalfFace* f) { face = f; }
    HalfFace* getFace() { return face;}
private:
    HalfVertex* origin;
    HalfEdge* next;
    HalfEdge* prev;
    HalfEdge* twin;
    HalfFace* face;
};

class HalfFace
{
public:
    HalfFace(HalfEdge* e){
        edge = e;
    }
    HalfEdge* edge;
    std::vector<int> conflict;
};

/// <summary>
/// doubly connected edge list
/// </summary>
class DCEL
{
public:
    ~DCEL();
    void cleanUp();
    std::vector<HalfFace*> faces;
    std::vector<HalfVertex*> vertexes;
};

class ConvexHull
{
public:
    ConvexHull();
    ConvexHull(std::vector<gp_Pnt> points);
    ~ConvexHull();
    bool compute();
    DCEL getResult();
    bool initTetrahedon();
    bool isDuplicate(gp_Pnt p1, gp_Pnt p2);
    bool isCollinear(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3);
    bool isCoplanar(gp_Pnt p1, gp_Pnt p2, gp_Pnt p3, gp_Pnt p4);
    void addNewFace(HalfEdge* share, HalfVertex* newVertex, HalfEdge* sharePrev);
protected:
    std::vector<gp_Pnt> m_points;
};
