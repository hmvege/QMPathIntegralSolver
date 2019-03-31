#ifndef IMPACTION_H
#define IMPACTION_H

#include "action.h"

class ImpAction : public Action
{
private:
    double m_mass;
public:
    ImpAction(int N, double a, double mass);
    virtual double getAction(std::vector<double> &x, int i);
};

#endif // IMPACTION_H
