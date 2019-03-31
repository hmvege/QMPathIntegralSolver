#ifndef CORRELATORX1_H
#define CORRELATORX1_H

#include "observable.h"

class CorrelatorX1 : public Observable
{
private:
    int m_N;
public:
    CorrelatorX1(int N);
    double calculate(std::vector<double> &x, int n);
};

#endif // CORRELATORX1_H
