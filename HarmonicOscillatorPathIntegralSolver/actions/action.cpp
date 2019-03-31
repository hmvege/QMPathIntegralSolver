#include "action.h"

Action::Action(int N, double a) : m_N(N), m_a(a)
{
}

double Action::getAction(std::vector<double> &x, int i)
{
    int i_next = (i + 1) % m_N; // mod boundary conditions
    int i_prev = (m_N + i - 1) % m_N; // mod boundary conditions
    return x[i]*(x[i] - x[i_prev] - x[i_next])/m_a + m_a*m_V->calculate(x,i);
}

