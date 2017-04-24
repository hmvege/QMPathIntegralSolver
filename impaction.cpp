#include "impaction.h"

ImpAction::ImpAction(int NPathPoints, double latticeSpacing, double mass)
{
    a = latticeSpacing;
    m = mass;
    N = NPathPoints;
}

double ImpAction::getAction(double * x, int i)
{
    int i_next = (i + 1) % N; // mod boundary conditions
    int i_prev = (N + i - 1) % N; // mod boundary conditions
    double delta = x[i_next] - 2 * x[i] + x[i_prev]/(a*a);
    return a * ( - 0.5*m*x[i] * (delta - a*a*delta*delta/12.0) * x[i] + a * potential(x[i]) );
}
