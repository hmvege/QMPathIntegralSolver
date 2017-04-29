#ifndef METROPOLIS_H
#define METROPOLIS_H

#include <random>
#include "actions/action.h"

class Action;

class Metropolis
{
private:
    int N;
    int NCf;
    int NCor;
    int NTherm;
    double epsilon;
    double a;
    double ** Gamma;
    int acceptanceCounter;

    // Variables used to perform statistics
    double * varianceGamma;
    double * stdGamma;
    double * deltaE_std;
    double * averagedGamma;
    double * averagedGammaSquared;
    double * deltaE;

    // Storing the action as a pointer
    Action *S = nullptr;

    // Storing the Gamma functional
    double (*gammaFunctional)(double * x, int n, int _N);

    // Function for updating our system using the Metropolis algorithm
    void update(double *x, std::mt19937_64 &gen, std::uniform_real_distribution<double> &epsilon_distribution, std::uniform_real_distribution<double> &uniform_distribution);
public:
    Metropolis(int new_N, int new_NCf, int new_NCor, int NTherm, double new_epsilon, double new_a);
    ~Metropolis();
    void runMetropolis();
    void getStatistics();
    void writeStatisticsToFile(const char *filename);
    void writeDataToFile(const char *filename);

    // Setters
    void setAction(Action *newS) { S = newS; }
    void setGammaFunctional(double (*newGammaFunctional)(double * x, int n, int _N)) { gammaFunctional = newGammaFunctional; }
    void setN(int new_N) { N = new_N; }
    void setNCf(int new_NCf) { NCf = new_NCf; }
    void setEpsilon(double new_epsilon) { epsilon = new_epsilon; }

    // Getters
    int getN() { return N; }
    int getNCf() { return NCf; }
    int getEpsilon() { return epsilon; }

    // Printers
    void printEnergies();
    void printAcceptanceRate();
};

#endif // METROPOLIS_H
