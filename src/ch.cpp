#include "ch.h"
#include <cstdlib>
#include <algorithm>
#include <set>

#define x coord[0]
#define y coord[1]
#define z coord[2]

#define a coefficient[0]
#define b coefficient[1]
#define c coefficient[2]
#define d coefficient[3]

void doublyConnectedEdgeList::clean_up()
{
  int i;
  edge *current_edge;
  for(i=0; i != (int)this->f.size(); ++i)
  {
    current_edge=this->f[i]->e;
    do
    {
      current_edge=current_edge->next;
      delete current_edge->prev;
    } while (current_edge != this->f[i]->e);
  }
  for (i=0; i != (int)this->f.size(); ++i)
    delete this->f[i];
  for (i=0; i != (int)this->v.size(); ++i)
    delete this->v[i];
  this->f.resize(0);
  this->v.resize(0);
}

void ConvexHull::clean_up()
{
    if (this->p == NULL) return;
    delete [] this->p;
    this->p=NULL;
    this->viz.resize(0);
    this->conflictP.resize(0);
    this->ch.clean_up();
}

doublyConnectedEdgeList::~doublyConnectedEdgeList()
{
  clean_up();
}

ConvexHull::~ConvexHull()
{
  clean_up();
}

void ConvexHull::setPoints(int n, point* p)
{
  this->p=p;
  this->nrPoints=n;
  this->viz.resize(n+1, 0);
  this->conflictP.resize(n+1);
}

ConvexHull::ConvexHull(int n, point* p)
{
  setPoints(n, p);
}

ConvexHull::ConvexHull()
{
  this->p=NULL;
}

bool ConvexHull::collinear(point p1, point p2, point p3)
{
  double eq, t;
  int i;
  
  for (i=0; (i != 3) && (p2.coord[i] == p1.coord[i]); ++i) ;
  if (i == 3)//duplicates!
  {
    fprintf(stderr, "ERROR: Duplicates!");
    exit(1);
  }
  t=(p3.coord[i]-p1.coord[i])/(p2.coord[i]-p1.coord[i]);

  for (i=0; i != 3; ++i)  
  {
    eq=p1.coord[i]+(p2.coord[i]-p1.coord[i])*t;
    eq -= p3.coord[i];
    if (!(eq >= -eps && eq <= eps)) return false;
  }
  return true;
}

bool ConvexHull::coplanar(point p1, point p2, point p3, point p4)
{
  equation coef(p1, p2, p3);
  double eq=coef.a*p4.x+coef.b*p4.y+coef.c*p4.z+coef.d;
  if (eq >= -eps && eq <= eps) return true;
  return false;
}

void ConvexHull::addFace(edge *e, vertex *v, edge *eprev)
{
  e->twin=new edge;
  e->twin->twin=e;
  e->twin->origin=e->next->origin;

  face *newface=new face;
  newface->equ=new equation(*(e->next->origin->p), *(e->origin->p), *(v->p));
  newface->e=e->twin;
  e->twin->f=newface;
  this->ch.f.push_back(newface);
  newface->nord=this->ch.f.size()-1;
 
  edge *e1=new edge, *e2=new edge;
  e1->prev=e->twin;
  e2->prev=e1;
  e->twin->prev=e2;

  e1->next=e2;
  e2->next=e->twin;
  e->twin->next=e1;

  e1->f=e2->f=newface;

  e1->origin=e->origin;
  e2->origin=v;

  ++(e->twin->origin->num);
  ++e1->origin->num;
  ++e2->origin->num;

  if(eprev != NULL)
  {
    e1->twin=eprev->twin->prev;
    eprev->twin->prev->twin=e1;
  }
}

void ConvexHull::computeTetrahedon()
{
  vertex *v[5];
  int i;
  for (i=1; i <= 4; ++i)
    v[i]=new vertex;
  
  v[1]->p=&this->p[1];
  v[2]->p=&this->p[2];
  this->viz[1]=viz[2]=true;

  i=3;
  while (collinear(this->p[1], this->p[2], this->p[i])) ++i;
  v[3]->p=&this->p[i];
  this->viz[i]=true;

  int j=i+1;
  while (coplanar(this->p[1], this->p[2], this->p[i], this->p[j])) ++j;
  v[4]->p=&this->p[j];
  this->viz[j]=true;

  edge *e[4];
  for (i=1; i <= 3; i++)
    e[i]=new edge;
  face *newface=new face;

  e[1]->next=e[2];
  e[2]->next=e[3];
  e[3]->next=e[1];

  e[1]->prev=e[3];
  e[2]->prev=e[1];
  e[3]->prev=e[2];

  for (i=1; i <= 3; ++i)
  {
    e[i]->origin=v[i];
    e[i]->f=newface;
  }

  newface->equ=new equation(*v[1]->p, *v[2]->p, *v[3]->p);
  newface->e=e[1];
  this->ch.f.push_back(newface);
  newface->nord=this->ch.f.size()-1;

  for (i=1; i <= 4; ++i)
  {
    this->ch.v.push_back(v[i]);
    v[i]->nord=this->ch.v.size()-1;
  }

  v[1]->num=v[2]->num=v[3]->num=1;

  addFace(e[1], v[4], NULL);
  addFace(e[2], v[4], e[1]);
  addFace(e[3], v[4], e[2]);

  e[1]->twin->next->twin=e[3]->twin->prev;
  e[3]->twin->prev->twin=e[1]->twin->next;
}

inline int ConvexHull::sgn(double v)
{
  if (v >= -eps && v <= eps)
  {
    if (this->exteriorSgn == -1) return 1;
    return -1;
  }
  if (v < 0) return -1;
  return 1;
}

int ConvexHull::computeInteriorSgn()
{
  long double s[3];
  point m;
  int i, j;
  
  s[0]=s[1]=s[2]=0;
  
  for (i=1; i <= this->nrPoints; ++i)
    if (this->viz[i] == true)
      for (j=0; j != 3; ++j)
        s[j]+=this->p[i].coord[j];

  for (i=0; i != 3; ++i)
    m.coord[i]=s[i]/this->nrPoints;

  equation coef=*this->ch.f[0]->equ;
  return sgn(coef.a*m.x+coef.b*m.y+coef.c*m.z+coef.d);
}

void ConvexHull::conflictTetrahedon()
{
  this->exteriorSgn = computeInteriorSgn()*(-1);
  
  int i, j;
  equation e;

  for (i=1; i <= this->nrPoints; ++i)
  {
    if (this->viz[i] == true) continue;
    for (j=0; j != (int)this->ch.f.size(); ++j)
    {
       e=*this->ch.f[j]->equ;
       if (sgn (e.a*this->p[i].x + e.b*this->p[i].y + e.c*this->p[i].z + e.d) == this->exteriorSgn) 
       {
         //face j conflicts with point i
         this->ch.f[j]->conflict.push_back(i); 
         this->conflictP[i].push_back(j);
       }
    }
  }
}

typedef std::pair <vertex*, std::pair<edge*, face*> > horizon;
//origin of the (external)edge, (external)horizon edge 

int find(std::vector <horizon> H, vertex* value)
{
  int i, s;
  for (s=1; s < (int)H.size(); s <<= 1);
  for (i=0; s; s >>= 1)
    if (i+s < (int)H.size() && H[i+s].first <= value)
      i+=s;
  return i;
}

void ConvexHull::eraseEdge(edge* E)
{
  --E->origin->num;
  if (E->origin->num == 0)
    this->ch.v[E->origin->nord]=NULL;
  delete E;
}

void ConvexHull::eraseFace(face* F)
{
  this->ch.f[F->nord]=NULL;
  delete F;
}

void ConvexHull::addPoint(int ordP)
{
/*DEBUG!  
  printf("for point %d\n", ordP);
  edge *e;
  for (int i=0; i != (int)this->ch.f.size(); ++i)
  {
    printf("face %d:\n", i);
    if (this->ch.f[i] == NULL)
    {
      printf("inexistent!\n");
      continue;
    }
    e=this->ch.f[i]->e;
    do
    {
      printf ("%d ", e->origin->nord);
      e=e->next;
    } while (e != this->ch.f[i]->e);
    printf ("\n");
  }
  printf ("\n\n");
*/

  //1.find ordered horizon ----------------------------------------------
 
  if (this->conflictP[ordP].empty()) return; 
 
  std::vector<horizon> H, Ho;
  int i, pos;
  double r;
  face* conflictFace;
  edge* currentEdge;
  equation* equ;

  for (i=0; i != (int)this->conflictP[ordP].size(); ++i)
  {
    conflictFace=this->ch.f[this->conflictP[ordP][i]];
    if (conflictFace == NULL) continue;
    currentEdge=conflictFace->e;
    do
    {
      equ=currentEdge->twin->f->equ; 
      if (sgn(equ->a*this->p[ordP].x + equ->b*this->p[ordP].y + equ->c*this->p[ordP].z + equ->d) != this->exteriorSgn)
        //currentEdge->twin->f is not visible from point this->p[ordP]
        H.push_back(std::make_pair(currentEdge->twin->origin, std::make_pair(currentEdge->twin, currentEdge->f)));
      currentEdge=currentEdge->next;
    } while (currentEdge != conflictFace->e);
  }
 
  if (H.size ())
  {
    std::sort(H.begin(), H.end());
    pos=0;
    do
    {
      Ho.push_back(H[pos]);
      pos=find(H, Ho[Ho.size()-1].second.first->next->origin);   
    } while (pos != 0);
  }
  else 
    return;
  
/* DEBUG! 
   printf("conflicts:\n");
  for (int i=0; i != (int)this->conflictP[ordP].size(); ++i)
    printf ("%d ", this->conflictP[ordP][i]);
  printf("\n");
  printf("horzion:");
  for (int i=0; i != Ho.size(); ++i)
    printf("(%d %d) ", Ho[i].first->nord, Ho[i].second.first->next->origin->nord);
  printf("\n");
*/

  //2.add faces ---------------------------------------------------------

  vertex* newvertex=new vertex;
  newvertex->p=&this->p[ordP];
  this->ch.v.push_back(newvertex);  
  newvertex->nord=this->ch.v.size()-1;

  edge* Ei;
  
  for (i=0; i != (int)Ho.size(); ++i)
  {
    Ei=Ho[i].second.first;

    // --- add new face
    if (i)
      addFace(Ei, newvertex, Ho[i-1].second.first);
    else
      addFace(Ei, newvertex, NULL);
  }
   
  Ho[0].second.first->twin->next->twin=Ho[Ho.size()-1].second.first->twin->prev;
  Ho[Ho.size()-1].second.first->twin->prev->twin=Ho[0].second.first->twin->next;

 
  //3.create conflicts---------------------------------------------------

  edge* it;
  std::set<int> conf;
  std::vector<int>::iterator c;
  std::set<int>::iterator setit;

  for (i=0; i != (int)Ho.size(); ++i)
  {
    Ei=Ho[i].second.first;

    // --- test coplanarity (this->p[ordP] in Ei->f)
    equ=Ei->f->equ;
    r=equ->a*this->p[ordP].x + equ->b*this->p[ordP].y + equ->c*this->p[ordP].z + equ->d;
    if (r >= -eps && r <= eps)
    {
      //delete new face
      eraseFace(Ei->twin->f);
   
      //new links
      Ei->next->prev=Ei->twin->prev;
      Ei->twin->prev->next=Ei->next;
      Ei->prev->next=Ei->twin->next;
      Ei->twin->next->prev=Ei->prev;
      
      //reset face links
      for (it=Ei->twin->next; it != Ei->next; it=it->next)
        it->f=Ei->f;
    
      Ei->f->e=Ei->twin->next;

      //delete Ei and Ei->twin
      eraseEdge(Ei->twin);
      eraseEdge(Ei);
    }
    else
    {
      //find new face's conflicts
      equ=Ei->twin->f->equ;
      for (c=Ei->f->conflict.begin(); c != Ei->f->conflict.end(); ++c)
        conf.insert(*c);
      for (c=Ho[i].second.second->conflict.begin(); c != Ho[i].second.second->conflict.end(); ++c)
        conf.insert(*c);
      for (setit=conf.begin(); setit != conf.end(); ++setit)
      {
        if (*setit <= ordP) continue;

        if (sgn(equ->a*this->p[*setit].x + equ->b*this->p[*setit].y + equ->c*this->p[*setit].z + equ->d) != this->exteriorSgn) 
          continue;
        
        Ei->twin->f->conflict.push_back(*setit);
        this->conflictP[*setit].push_back(Ei->twin->f->nord);
      }
      conf.clear();
    }
  }

  //4.clean up ----------------------------------------------------------
  
  face* currentFace;
  for (i=0; i != (int)this->conflictP[ordP].size(); ++i)
  {
    currentFace=this->ch.f[this->conflictP[ordP][i]];
    if (currentFace == NULL) continue;
    currentEdge=currentFace->e;
    do
    {
      currentEdge=currentEdge->next;
      eraseEdge(currentEdge->prev);
    } while (currentEdge != currentFace->e);
    eraseFace(currentFace);
  }
  this->conflictP[ordP].resize(0);  
}

void ConvexHull::computeConvexHull()
{
  computeTetrahedon();
  conflictTetrahedon();
  int i;
  for(i=1; i <= this->nrPoints; ++i)
  {
    if(this->viz[i] == true) continue;
    addPoint(i);
  }
}

doublyConnectedEdgeList* ConvexHull::getConvexHull(int n, point* p)
{
  setPoints(n, p);
  computeConvexHull();

  //remove NULL
  int i;
  std::vector <face*> faux;
  std::vector <vertex*> vaux;
  for (i=0; i != (int)this->ch.f.size(); ++i)  
    if (this->ch.f[i] != NULL)
      faux.push_back(this->ch.f[i]);    
  for (i=0; i != (int)this->ch.v.size(); ++i)
    if (this->ch.v[i] != NULL)
      vaux.push_back(this->ch.v[i]);

  //reset nord
  for (i=0; i != (int)faux.size(); ++i) 
    faux[i]->nord=i;
  for (i=0; i != (int)vaux.size(); ++i)
    vaux[i]->nord=i;

  this->ch.f=faux;
  this->ch.v=vaux;
  
  return &this->ch;
}

/*int main()
{
  
  return 0;
}*/

#undef x
#undef y
#undef z

#undef a
#undef b
#undef c
#undef d


