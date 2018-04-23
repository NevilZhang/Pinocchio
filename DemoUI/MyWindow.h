#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <FL/Fl_Gl_Window.H>
#include "../Pinocchio/mesh.h"
#include "../Pinocchio/transform.h"
#include "DisplayMesh.h"
//Ïß¶Î
struct LineSegment
{
    LineSegment() : thickness(1.) {}
    LineSegment(const Vector3 &inP1, const Vector3 &inP2,
        const Vector3 &inColor = Vector3(1, 1, 1), double inThickness = 1.)
        : p1(inP1), p2(inP2), color(inColor), thickness(inThickness) {}

    Vector3 p1, p2, color;
    double thickness;
};

class MyWindow : public Fl_Gl_Window
{
public:
    MyWindow();

    virtual ~MyWindow() {}
    virtual void draw();
    virtual int handle(int);

    void addMesh(DisplayMesh *inM) { meshes.push_back(inM); }
    void addLine(const LineSegment &l) { lines.push_back(l); }
    void clearLines() { lines.clear(); }

private:
    bool flatShading, floor, skeleton;
    Transform<> transform;
    vector<DisplayMesh *> meshes;
    vector<LineSegment> lines;

    void resetTransform();
    void initGL();
    void drawMesh(const Mesh &m, bool flatShading, Vector3 trans = Vector3());
    void drawFloor();
};

#endif //MYWINDOW_H
