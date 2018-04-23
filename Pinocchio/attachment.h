#ifndef ATTACHMENT_H
#define ATTACHMENT_H

#include "mesh.h"
#include "skeleton.h"
#include "transform.h"

class VisibilityTester
{
public:
    virtual ~VisibilityTester() {}
    virtual bool canSee(const Vector3 &v1, const Vector3 &v2) const = 0;
};

template<class T> class VisTester : public VisibilityTester
{
public:
    VisTester(const T *t) : tree(t) {}

    virtual bool canSee(const Vector3 &v1, const Vector3 &v2) const //faster when v2 is farther inside than v1
    {
        const double maxVal = 0.002;
        double atV2 = tree->locate(v2)->evaluate(v2);
        double left = (v2 - v1).length();
        double leftInc = left / 100.;
        Vector3 diff = (v2 - v1) / 100.;
        Vector3 cur = v1 + diff;
        while(left >= 0.) {
            double curDist = tree->locate(cur)->evaluate(cur);
            if(curDist > maxVal)
                return false;
            //if curDist and atV2 are so negative that distance won't reach above maxVal, return true
            if(curDist + atV2 + left <= maxVal)
                return true;
            cur += diff;
            left -= leftInc;
        }
        return true;
    }

private:
    const T *tree;
};

template<class T> VisibilityTester *makeVisibilityTester(const T *tree) { return new VisTester<T>(tree); } //be sure to delete afterwards

class AttachmentPrivate;

class PINOCCHIO_API Attachment
{
public:
    Attachment() : a(NULL) {}
    Attachment(const Attachment &);
    Attachment(const Mesh &mesh, const Skeleton &skeleton, const vector<Vector3> &match, const VisibilityTester *tester);
    virtual ~Attachment();

    Mesh deform(const Mesh &mesh, const vector<Transform<> > &transforms) const;
    Vector<double, -1> getWeights(int i) const;
private:
    AttachmentPrivate *a;
};

#endif
