#ifndef FILTER_H_INCLUDED
#define FILTER_H_INCLUDED

#include "../Pinocchio/matrix.h"
#include "../Pinocchio/vector.h"
#include "../Pinocchio/transform.h"

class MotionFilter
{
public:
    MotionFilter(const vector<Vector3> &inJoints, const vector<int> inPrev)
        : joints(inJoints), prev(inPrev) {}

    void step(const vector<Transform<> > &transforms, vector<Vector3> feet);

    const vector<Transform<> > &getTransforms() const { return curTransforms; }

private:
    Matrixn<double> getJac(const vector<Transform<> > &transforms) const;
    void addTranslation();

    vector<Vector3> joints;
    vector<int> prev;

    Vector3 prevTrans;
    Vectorn<double> prevFeet;
    vector<Transform<> > curTransforms;
};

#endif //FILTER_H_INCLUDED
