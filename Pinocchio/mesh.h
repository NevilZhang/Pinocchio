//网格结构（基于有向边 - Campagna等）
#ifndef MESH_H
#define MESH_H

#include <vector>

#include "vector.h"
#include "rect.h"

struct MeshVertex
{
    MeshVertex() : edge(-1) {}
    
    Vector3 pos;
    Vector3 normal;
    int edge; //an edge such that edge->prev->vertex is this
};

struct MeshEdge
{
    MeshEdge() : vertex(-1), prev(-1), twin(-1) {}
    
    int vertex; //边指向的顶点 - 起始顶点是prev->顶点
    int prev; //ccw, next is prev->prev
    int twin;
};

class PINOCCHIO_API Mesh {
public:
    Mesh() : scale(1.) {}
    Mesh(const string &file);

    bool integrityCheck() const;
    bool isConnected() const; //如果网格由单个连接的组件组成，则返回true
    void computeVertexNormals();
    void normalizeBoundingBox();
    void computeTopology();
    void writeObj(const string &filename) const;
    
private:
    void readObj(istream &strm);
    void readOff(istream &strm);
    void readPly(istream &strm);
    void readGts(istream &strm);
    void readStl(istream &strm);
    void fixDupFaces();
    void sortEdges(); //sort edges so that triplets forming faces are adjacent

public: //data
	//Mesh 顶点集合
    vector<MeshVertex> vertices;
	//Mesh 边集合
    vector<MeshEdge> edges; //halfEdges, really

    Vector3 toAdd;
    double scale;
};

#endif
