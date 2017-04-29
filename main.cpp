#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random> // For Mersenne-Twister19937
#include <ctime> // For random seed
#include "metropolis.h"
#include "actions/action.h"
#include "actions/impaction.h"
//#include <mpi.h> // For parallelization later

using namespace std;

double potential(double x);
double gammaFunctional(double * x, int n, int N);
double gammaFunctional2(double *x, int n, int N);

int main()
{
    clock_t programStart, programEnd; // Clocking for fun
    programStart = clock();

    double epsilon = 1.4;   // Random interval for picking a new path position to try
    double a = 0.5;         // Lattice spacing
    int NTherm  = 10;       // Number of times we are to thermalize, that is NTherm * NCor
    int N       = 20;       // Points in path at lattice, looking at a 1D lattice, but modelling the possible paths as columns of a matrix
    int NCor    = 20;       // Only keeping every 20th path
    int NCf     = 1e5;      // Number of random path or path configurations

    Action S(N,a);
    S.setPotential(potential);

    Metropolis metropolis(N, NCf, NCor, NTherm, epsilon, a);
    metropolis.setAction(&S);

//    // Using gammaFunctional
//    metropolis.setGammaFunctional(&gammaFunctional);
//    metropolis.runMetropolis();
//    metropolis.getStatistics();
//    metropolis.writeDataToFile("output/gammaFunctional_gamma.txt");
//    metropolis.writeStatisticsToFile("output/gammaFunctional_stats.txt");
//    metropolis.printEnergies();
//    metropolis.printAcceptanceRate();

//    // Using gammaFunctional2
//    metropolis.setGammaFunctional(&gammaFunctional2);
//    metropolis.runMetropolis();
//    metropolis.getStatistics();
//    metropolis.writeDataToFile("output/gammaFunctional2_gamma.txt");
//    metropolis.writeStatisticsToFile("output/gammaFunctional2_stats.txt");
//    metropolis.printEnergies();
//    metropolis.printAcceptanceRate();

    // Using improved action
    double m = 1.0;         // Mass
    ImpAction SImp(N,a,m);
    SImp.setPotential(potential);
    metropolis.setAction(&SImp);
    metropolis.setGammaFunctional(&gammaFunctional);
    metropolis.runMetropolis();
    metropolis.getStatistics();
    metropolis.writeDataToFile("output/improvedAction_gamma.txt");
    metropolis.writeStatisticsToFile("output/improvedAction_stats.txt");
    metropolis.printEnergies();
    metropolis.printAcceptanceRate();

    programEnd = clock();
    cout << "Program complete. Time used: " << ((programEnd - programStart)/((double)CLOCKS_PER_SEC)) << endl;
    return 0;
}

double potential(double x)
{
    /*
     * HO potential
     */
    return x*x/2.0;
}

double gammaFunctional(double * x, int n, int N)
{
    /*
     * For exercise with x(i+1)*x(i)
     */
    double G = 0;
    for (int i = 0; i < N; i++)
    {
        G += x[(i + n) % N] * x[i];
    }
    return G/((double) N);
}

double gammaFunctional2(double *x, int n, int N)
{
    /*
     * For exercise with x(i+1)^3*x(i)^3
     */
    double G = 0;
    for (int i = 0; i < N; i++)
    {
        G += x[(i + n) % N]*x[(i + n) % N]*x[(i + n) % N] * x[i]*x[i]*x[i];;
    }
    return G/((double) N);
}
