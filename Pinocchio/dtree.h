#ifndef DTREE_H
#define DTREE_H

#include "rect.h"
#include "indexer.h"

template<class Data, int Dim>
class DNode : public Data
{
public:
    typedef DNode<Data, Dim> Self;
    typedef Vector<double, Dim> Vec;
    typedef Rect<double, Dim> MyRect;

    int countNodes() const
    {
        int nodes = 1;
        if(children[0] != NULL)
            for(int i = 0; i < numChildren; ++i)
            nodes += children[i]->countNodes();
        return nodes;
    }
    
    int maxLevel() const
    {
        if(children[0] == NULL)
            return 0;
        int ml = 0;
        for(int i = 0; i < numChildren; ++i)
            ml = max(ml, children[i]->maxLevel());
        return 1 + ml;
    }

    Self *getParent() const { return parent; }
    Self *getChild(int idx) const { return children[idx]; }
    const MyRect &getRect() const { return rect; }
    int getChildIndex() const { return childIndex; }

    static const int numChildren = 1 << Dim;

private:
    DNode(MyRect r) : Data(this), parent(NULL), rect(r)
    {
        zeroChildren();
        Data::init();
    }

    DNode(Self *inParent, int inChildIndex) : Data(this), parent(inParent), childIndex(inChildIndex)
    {
        zeroChildren();
        rect = MyRect(inParent->rect.getCorner(childIndex)) | MyRect(inParent->rect.getCenter());
        Data::init();
    }

    ~DNode()
    { 
        for(int i = 0; i < numChildren; ++i)
            if(children[i])
                delete children[i];
    }

    void split()
    {
        for(int i = 0; i < numChildren; ++i)
            children[i] = new Self(this, i);
    }

    template<class D, int DI, template<typename N, int ID> class IDX> friend class DRootNode;

    void zeroChildren() { for(int i = 0; i < numChildren; ++i) children[i] = NULL; }

    //data
    Self *parent;
    Self *children[numChildren];
    int childIndex;

    MyRect rect;
};

template<class Data, int Dim, template<typename Node, int IDim> class Indexer = DumbIndexer>
class DRootNode : public DNode<Data, Dim>, public Indexer<DNode<Data, Dim>, Dim>
{
public:
    typedef DNode<Data, Dim> Node;
    typedef DRootNode<Data, Dim, Indexer> Self;
    typedef Indexer<Node, Dim> MyIndexer;
    typedef Vector<double, Dim> Vec;
    typedef Rect<double, Dim> MyRect;

    DRootNode(MyRect r = MyRect(Vec(), Vec().apply(bind2nd(plus<double>(), 1.)))) : Node(r)
    {
        MyIndexer::setRoot(this);
    }
    
    void split(Node *node)
    {
        node->split();
    }
};

#endif