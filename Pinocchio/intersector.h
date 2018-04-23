//与网格相交线的例程
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1
#ifndef INTERSECTOR_H
#define INTERSECTOR_H

#include "mesh.h"
#include "vecutils.h"

class PINOCCHIO_API Intersector {
public:
    Intersector() : mesh(NULL) {}
    Intersector(const Mesh &m, const Vector3 &inDir) : mesh(&m), dir(inDir) { init(); }
    
    vector<Vector3> intersect(const Vector3 &pt, vector<int> *outIndices = NULL) const;    
    const Vector3 &getDir() const { return dir; }
private:
    void init();
    void getIndex(const Vector2 &pt, int &x, int &y) const;
    
    const Mesh *mesh;
    Vector3 dir;
    Vector3 v1, v2; //basis
    Rect2 bounds; //within the basis
    
    vector<Vector2> points;
    vector<Vector3> sNormals; //they are scaled for intersection
    vector<vector<int> > triangles;
};

#endif //INTERSECTOR_H
