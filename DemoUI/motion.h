#ifndef MOTION_H
#define MOTION_H

#include "../Pinocchio/transform.h"

class Motion
{
public:
    Motion(const string &filename);

    bool empty() const { return data.empty(); }
    vector<Transform<> > get() const;
	//��ȡ��̬
    vector<Vector3> getPose() const;
    vector<Vector3> getRefPose() const { return refPose; }
    double getLegLength() const { return legLength; }
    double getLegWidth() const { return legWidth; }

    const vector<vector<Transform<> > > &getData() const { return data; }
    void setFixedFrame(int inFrame) { fixedFrame = inFrame < 0 ? -1 : (int)(inFrame % data.size()); }
private:
    int getFrameIdx() const;
	//���ݶ�ȡ
    void readH(istream &strm);
    vector<vector<Transform<> > > data;
    vector<vector<Vector3> > poses;
    vector<Vector3> refPose;
    double legLength;
    double legWidth;
    int fixedFrame;
};

#endif
