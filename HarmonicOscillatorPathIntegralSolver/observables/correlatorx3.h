#ifndef CORRELATORX3_H
#define CORRELATORX3_H

#include "observable.h"

class CorrelatorX3 : public Observable
{
private:
    int m_N;
public:
    CorrelatorX3(int N);
    double calculate(std::vector<double> &x, int n);
};

#endif // CORRELATORX3_H
