#ifndef HARMONICOSCILLATOR_H
#define HARMONICOSCILLATOR_H

#include <vector>
#include "potential.h"

class HarmonicOscillator : public Potential
{
public:
    HarmonicOscillator();

    double calculate(std::vector<double> &x, int i);
};

#endif // HARMONICOSCILLATOR_H
