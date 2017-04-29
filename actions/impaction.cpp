#include "impaction.h"

ImpAction::ImpAction(int NPathPoints, double latticeSpacing, double mass)
{
    a = latticeSpacing;
    m = mass;
    N = NPathPoints;
}

double ImpAction::getAction(double * x, int i)
{
    /*
     * The action only returns the parts that contain x[i], otherwise it should have been a sum from i=0 to i=N-1.
     */
    int i_next = (i + 1) % N; // mod boundary conditions
    int i_prev = (N + i - 1) % N; // mod boundary conditions
    int i_2next = (i + 2) % N; // mod boundary conditions
    int i_2prev = (N + i - 2) % N; // mod boundary conditions
    // Improved
    double kineticEnergy = m/(12*a) * x[i] * (15*x[i] - 16*x[i_prev] - 16*x[i_next] + x[i_2prev] + x[i_2next]); // THIS WORKS
    return kineticEnergy + a*potential(x[i]);
}
