#include "correlatorx1.h"

CorrelatorX1::CorrelatorX1(int N) : m_N(N)
{

}

double CorrelatorX1::calculate(std::vector<double> &x, int n)
{
    /*
     * For exercise with x(i+1)*x(i)
     */
    double G = 0;
    for (int i = 0; i < m_N; i++)
    {
        G += x[(i + n) % m_N] * x[i];
    }
    return G/double(m_N);
}
