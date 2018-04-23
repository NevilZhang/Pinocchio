#include <FL/fl_ask.H>
#include <fstream>

#include "processor.h"
#include "../Pinocchio/skeleton.h"
#include "../Pinocchio/utils.h"
#include "../Pinocchio/debugging.h"
#include "../Pinocchio/attachment.h"
#include "../Pinocchio/pinocchioApi.h"
#include "defmesh.h"
#include "motion.h"

struct ArgData
{
    ArgData() :
        stopAtMesh(false), stopAfterCircles(false), skelScale(1.), noFit(false),
        skeleton(HumanSkeleton())
    {
    }
    
    bool stopAtMesh;
    bool stopAfterCircles;
    string filename;
    string motionname;
    Quaternion<> meshTransform;
    double skelScale;
    bool noFit;
    Skeleton skeleton;
    string skeletonname;
};

void printUsageAndExit()
{
    cout << "Usage: DemoUI filename.{obj | ply | off | gts | stl}" << endl;
    cout << "              [-skel skelname] [-rot x y z deg]* [-scale s]" << endl;
    cout << "              [-meshonly | -mo] [-circlesonly | -co]" << endl;
    cout << "              [-motion motionname] [-nofit]" << endl;

    exit(0);
}

ArgData processArgs(const vector<string> &args)
{
    ArgData out;
    int cur = 2;
    int num = args.size();
    if(num < 2)
        printUsageAndExit();

    out.filename = args[1];
    
    while(cur < num) {
        string curStr = args[cur++];
        if(curStr == string("-skel")) {
            if(cur == num) {
                cout << "No skeleton specified; ignoring." << endl;
                continue;
            }
            curStr = args[cur++];
            if(curStr == string("human"))
                out.skeleton = HumanSkeleton();
            else if(curStr == string("horse"))
                out.skeleton = HorseSkeleton();
            else if(curStr == string("quad"))
                out.skeleton = QuadSkeleton();
            else if(curStr == string("centaur"))
                out.skeleton = CentaurSkeleton();
            else
                out.skeleton = FileSkeleton(curStr);
            out.skeletonname = curStr;
            continue;
        }
        if(curStr == string("-rot")) {
            if(cur + 3 >= num) {
                cout << "Too few rotation arguments; exiting." << endl;
                printUsageAndExit();
            }
            double x, y, z, deg;
            sscanf(args[cur++].c_str(), "%lf", &x);
            sscanf(args[cur++].c_str(), "%lf", &y);
            sscanf(args[cur++].c_str(), "%lf", &z);
            sscanf(args[cur++].c_str(), "%lf", &deg);
            
            out.meshTransform = Quaternion<>(Vector3(x, y, z), deg * M_PI / 180.) * out.meshTransform;
            continue;
        }
        if(curStr == string("-scale")) {
            if(cur >= num) {
                cout << "No scale provided; exiting." << endl;
                printUsageAndExit();
            }
            sscanf(args[cur++].c_str(), "%lf", &out.skelScale);
            continue;
        }
        if(curStr == string("-meshonly") || curStr == string("-mo")) {
            out.stopAtMesh = true;
            continue;
        }
        if(curStr == string("-circlesonly") || curStr == string("-co")) {
            out.stopAfterCircles = true;
            continue;
        }
        if(curStr == string("-nofit")) {
            out.noFit = true;
            continue;
        }
        if(curStr == string("-motion")) {
            if(cur == num) {
                cout << "No motion filename specified; ignoring." << endl;
                continue;
            }
            out.motionname = args[cur++];
            continue;
        }
        cout << "Unrecognized option: " << curStr << endl;
        printUsageAndExit();
    }

    return out;
}

void process(const vector<string> &args, MyWindow *w)
{
    int i;
    ArgData a = processArgs(args);

    Debugging::setOutStream(cout);

    Mesh m(a.filename);
	//����ģ��
    if(m.vertices.size() == 0) {
        cout << "Error reading file.  Aborting." << endl;
        exit(0);
        return;
    }            
	cout << "������" << endl;
	//
    for(i = 0; i < (int)m.vertices.size(); ++i)
        m.vertices[i].pos = a.meshTransform * m.vertices[i].pos;
    m.normalizeBoundingBox();
    m.computeVertexNormals();

    Skeleton given = a.skeleton;
    given.scale(a.skelScale * 0.7);

    if(a.stopAtMesh) { //if early bailout
        w->addMesh(new StaticDisplayMesh(m));
        return;
    }
	cout << "�ٺ�" << endl;
    PinocchioOutput o;
	cout << "�󷢶ԶԶԷ�" << endl;
    if(!a.noFit) { //do everything
		cout << "������" << endl;
        o = autorig(given, m);
		cout << "�󷢷�" << endl;
    }
    else { //skip the fitting step--assume the skeleton is already correct for the mesh
		cout << "�㷢�ĸ�" << endl;
        TreeType *distanceField = constructDistanceField(m);
        VisTester<TreeType> *tester = new VisTester<TreeType>(distanceField);
		cout << "��s�Ƿ���" << endl;
        o.embedding = a.skeleton.fGraph().verts;
        for(i = 0; i < (int)o.embedding.size(); ++i)
            o.embedding[i] = m.toAdd + o.embedding[i] * m.scale;
		cout << "�󷢹㷢��" << endl;
        o.attachment = new Attachment(m, a.skeleton, o.embedding, tester);

        delete tester;
        delete distanceField;
    }
	cout << "�ٺٹ���" << endl;
    if(o.embedding.size() == 0) {
        cout << "Error embedding" << endl;
        exit(0);
    }
	cout << "���" << endl;

    if(a.motionname.size() > 0) {
        w->addMesh(new DefMesh(m, given, o.embedding, *(o.attachment), new Motion(a.motionname)));
    }
    else {
        w->addMesh(new StaticDisplayMesh(m));

        for(i = 1; i < (int)o.embedding.size(); ++i)
        {
            w->addLine(LineSegment(o.embedding[i], o.embedding[given.fPrev()[i]], Vector3(.5, .5, 0), 4.));
        }
    }
	cout << "�Ǻ�" << endl;
    //output skeleton embedding
    for(i = 0; i < (int)o.embedding.size(); ++i)
        o.embedding[i] = (o.embedding[i] - m.toAdd) / m.scale;
    ofstream os("skeleton.out");
    for(i = 0; i < (int)o.embedding.size(); ++i) {
        os << i << " " << o.embedding[i][0] << " " << o.embedding[i][1] <<
                   " " << o.embedding[i][2] << " " << a.skeleton.fPrev()[i] << endl;
    }
	cout << "�ߺ�" << endl;
    //output attachment
    std::ofstream astrm("attachment.out");
    for(i = 0; i < (int)m.vertices.size(); ++i) {
        Vector<double, -1> v = o.attachment->getWeights(i);
        for(int j = 0; j < v.size(); ++j) {
            double d = floor(0.5 + v[j] * 10000.) / 10000.;
            astrm << d << " ";
        }
        astrm << endl;
    }
	cout << "O(��_��)O����~" << endl;
    delete o.attachment;
}
