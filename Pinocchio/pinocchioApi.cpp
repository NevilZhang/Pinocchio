
#include "pinocchioApi.h"
#include "debugging.h"
#include <fstream>

ostream *Debugging::outStream = new ofstream();

PinocchioOutput autorig(const Skeleton &given, const Mesh &m)
{
    int i;
    PinocchioOutput out;//准备返回的数据

    Mesh newMesh = prepareMesh(m);
    if(newMesh.vertices.size() == 0)
        return out;
    TreeType *distanceField = constructDistanceField(newMesh);
	cout << "uui1" << endl;
    //离散
    vector<Sphere> medialSurface = sampleMedialSurface(distanceField);
	cout << "1646" << endl;
    vector<Sphere> spheres = packSpheres(medialSurface);
	cout << "8888" << endl;
    PtGraph graph = connectSamples(distanceField, spheres);
	cout << "三国杀" << endl;
    //离散嵌入
    vector<vector<int> > possibilities = computePossibilities(graph, spheres, given);
	//可以通过重新指定骨架关节的可能性来设置约束：
	//限制关节i到球体j，使用：possiblities [i] = vector <int>（1，j）;
    //constraints can be set by respecifying possibilities for skeleton joints:
    //to constrain joint i to sphere j, use: possiblities[i] = vector<int>(1, j);
	cout << "好听是好听" << endl;
    vector<int> embeddingIndices = discreteEmbed(graph, spheres, given, possibilities);

    if(embeddingIndices.size() == 0) { //failure
        delete distanceField;
        return out;
    }

    vector<Vector3> discreteEmbedding = splitPaths(embeddingIndices, graph, given);

    //continuous refinement
    vector<Vector3> medialCenters(medialSurface.size());
    for(i = 0; i < (int)medialSurface.size(); ++i)
        medialCenters[i] = medialSurface[i].center;

    out.embedding = refineEmbedding(distanceField, medialCenters, discreteEmbedding, given);

    //attachment
    VisTester<TreeType> *tester = new VisTester<TreeType>(distanceField);
    out.attachment = new Attachment(newMesh, given, out.embedding, tester);

    //cleanup
    delete tester;
    delete distanceField;

    return out;
}

