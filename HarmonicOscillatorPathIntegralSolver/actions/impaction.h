#ifndef IMPACTION_H
#define IMPACTION_H

#include "action.h"

class ImpAction : public Action
{
private:
    double m;
public:
    ImpAction(int NPathPoints, double latticeSpacing, double mass);
    virtual double getAction(double * x, int i);
};

#endif // IMPACTION_H
