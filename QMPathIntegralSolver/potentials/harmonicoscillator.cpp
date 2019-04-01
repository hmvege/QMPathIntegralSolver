#include "harmonicoscillator.h"

HarmonicOscillator::HarmonicOscillator()
{

}
double HarmonicOscillator::calculate(std::vector<double> &x, int i)
{
    /*
     * HO potential
     */
    return x[i]*x[i]/2.0;
}
