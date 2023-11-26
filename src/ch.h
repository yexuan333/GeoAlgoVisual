#pragma once
#include <vector>
#include <cstdio>

#define eps 0.0000001

#define x coord[0]
#define y coord[1]
#define z coord[2]

#define a coefficient[0]
#define b coefficient[1]
#define c coefficient[2]
#define d coefficient[3]

struct point
{
  double coord[3];
};

struct equation
{
  double coefficient[4];
  
  equation(point p1, point p2, point p3)
  {
    a=p1.y*(p2.z - p3.z) + p2.y*(p3.z - p1.z) + p3.y*(p1.z - p2.z);
    b=p1.z*(p2.x - p3.x) + p2.z*(p3.x - p1.x) + p3.z*(p1.x - p2.x);
    c=p1.x*(p2.y - p3.y) + p2.x*(p3.y - p1.y) + p3.x*(p1.y - p2.y);
    d=p1.x*(p2.y*p3.z - p3.y*p2.z) + p2.x*(p3.y*p1.z - p1.y*p3.z) + p3.x*(p1.y*p2.z - p2.y*p1.z);
    d*=-1;
  }
  
  equation()
  {
  }
};

struct edge;
struct vertex;

struct face
{
  equation *equ;
  edge *e;
  std::vector<int> conflict;
  int nord;
  face ()
  {
    equ=NULL;
    e=NULL;
  }
};

struct edge
{
  vertex *origin;
  edge *next,*prev,*twin;
  face *f;
  edge ()
  {
    origin=NULL;
    next=prev=twin=NULL;
    f=NULL;
  }
};

struct vertex
{
  point *p;
  int num, nord;
  vertex ()
  {
    p=NULL;
    num=0;
  }
};

class doublyConnectedEdgeList
{
  public:
    ~doublyConnectedEdgeList();
    void clean_up();

    std::vector<face*> f;
    std::vector<vertex*> v;  
};

class ConvexHull
{
  public:
    ConvexHull();
    ConvexHull(int, point*);
    ~ConvexHull();
    void setPoints(int, point*);
    doublyConnectedEdgeList* getConvexHull(int, point*);
    void clean_up();

  protected:
    void computeTetrahedon();
    void addFace(edge*, vertex*, edge*);
    bool collinear(point, point, point);
    bool coplanar(point, point, point, point);
    void computeConvexHull();
    int computeInteriorSgn();
    void conflictTetrahedon();
    int sgn(double);
    void addPoint(int);
    void eraseEdge(edge*);
    void eraseFace(face*);

    point* p;    
    int nrPoints;
    
    std::vector<bool> viz;
    
    doublyConnectedEdgeList ch;
    
    int exteriorSgn;

    std::vector< std::vector<int> > conflictP;
};

#undef x
#undef y
#undef z

#undef a
#undef b
#undef c
#undef d


