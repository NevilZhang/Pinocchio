#ifndef DEFMESH_H_INCLUDED
#define DEFMESH_H_INCLUDED

#include "../Pinocchio/attachment.h"
#include "filter.h"
#include "DisplayMesh.h"

class Motion;

class DefMesh : public DisplayMesh
{
public:
	//定义网格
    DefMesh(const Mesh inMesh, const Skeleton &inOrigSkel, const vector<Vector3> &inMatch,
            const Attachment &inAttachment, Motion *inMotion = NULL) 
      : origSkel(inOrigSkel), match(inMatch), attachment(inAttachment),
        origMesh(inMesh), motion(inMotion), filter(match, origSkel.fPrev())
    {
        transforms.resize(origSkel.fPrev().size() - 1);
    }
	//设置运动
    void setMotion(Motion *inMotion) { motion = inMotion; }
	//获取运动
    Motion *getMotion() const { return motion; }
	//如果有运动就更新
    void updateIfHasMotion() const { if(motion) updateMesh(); }

	//获取骨架
    vector<Vector3> getSkel() const;
	//获取最初的骨架
    const Skeleton &getOrigSkel() const { return origSkel; }
	//获取附件
    const Attachment &getAttachment() const { return attachment; }
	//获取网格
    const Mesh &getMesh() { updateMesh(); return curMesh; }

private:
    double getLegRatio() const;
    vector<Transform<> > computeTransforms() const;
    void updateMesh() const;

    Skeleton origSkel;
    vector<Vector3> match;
    Attachment attachment;
    Mesh origMesh;
    mutable Mesh curMesh;
    vector<Quaternion<> > transforms;
    Motion *motion;

    vector<double> footOffsets;
    mutable MotionFilter filter;
};

#endif //DEFMESH_H_INCLUDED
