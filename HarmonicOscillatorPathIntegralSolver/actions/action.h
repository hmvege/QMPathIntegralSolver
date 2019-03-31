#ifndef ACTION_H
#define ACTION_H

#include <vector>
#include "potentials/potential.h"

class Action
{
protected:
    Potential *m_V = nullptr;
    double m_a;
    int m_N;
public:
    Action(int N, double a);

    virtual ~Action() {}
    virtual double getAction(std::vector<double> &x, int i);

    // Setters
    void setPotential(Potential *V) { m_V = V; }
    void setLatticeSpacing(double a) { m_a = a; }
    void setNLatticePoints(int N) { m_N = N; }

    // Getters
    double getLatticeSpacing() { return m_a; }
    int getNLatticePoints() { return m_N; }
};

#endif // ACTION_H
