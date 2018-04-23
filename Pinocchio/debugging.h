#ifndef DEBUGGING_H
#define DEBUGGING_H

#include "mathutils.h"

class Debugging
{
public:
    static ostream &out() { return *outStream; }
    static void PINOCCHIO_API setOutStream(ostream &os) { outStream = &os; }

private:
    static ostream *outStream;
};

#endif //DEBUGGING_H
