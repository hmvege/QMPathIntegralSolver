#include "impaction.h"

ImpAction::ImpAction(int N, double a, double mass) : m_mass(mass), Action(N, a)
{
}

double ImpAction::getAction(std::vector<double> &x, int i)
{
    /*
     * The action only returns the parts that contain x[i], otherwise it should have been a sum from i=0 to i=N-1.
     */

    // Sets correct index values with modulo operations for periodic boundary conditions.
    int i_next = (i + 1) % m_N;
    int i_prev = (m_N + i - 1) % m_N;
    int i_2next = (i + 2) % m_N;
    int i_2prev = (m_N + i - 2) % m_N;

    // Improved action
    double kineticEnergy = m_mass/(12*m_a) * x[i] * (15*x[i] - 16*x[i_prev] - 16*x[i_next] + x[i_2prev] + x[i_2next]);

    return kineticEnergy + m_a*m_V->calculate(x,i);
}
