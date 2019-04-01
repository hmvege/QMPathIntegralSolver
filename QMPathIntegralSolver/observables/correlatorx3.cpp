#include "correlatorx3.h"

CorrelatorX3::CorrelatorX3(int N) : m_N(N)
{

}

double CorrelatorX3::calculate(std::vector<double> &x, int n)
{
    /*
     * For exercise with x(i+1)^3*x(i)^3
     */
    double G = 0;
    int shiftedIndex = 0;
    for (int i = 0; i < m_N; i++)
    {
        shiftedIndex = (i + n) % m_N;
        G += x[shiftedIndex]*x[shiftedIndex]*x[shiftedIndex] * x[i]*x[i]*x[i];;
    }
    return G/((double) m_N);
}
