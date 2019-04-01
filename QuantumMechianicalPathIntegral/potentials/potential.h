#ifndef POTENTIAL_H
#define POTENTIAL_H

#include <vector>

class Potential
{
public:
    Potential();
    virtual ~Potential() {}

    virtual double calculate(std::vector<double> &x, int i);
};

#endif // POTENTIAL_H
