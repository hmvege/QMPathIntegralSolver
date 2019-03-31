#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <vector>

class Observable
{
public:
    Observable();
    virtual ~Observable() {}

    virtual double calculate(std::vector<double> &x, int n);
};

#endif // OBSERVABLE_H
