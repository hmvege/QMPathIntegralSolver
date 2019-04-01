#include <cmath>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random> // For Mersenne-Twister19937
#include <ctime> // For random seed

#include "system.h"
#include "actions/action.h"
#include "actions/impaction.h"
#include "observables/correlatorx1.h"
#include "observables/correlatorx3.h"
#include "potentials/harmonicoscillator.h"
//#include <mpi.h> // For parallelization later

using namespace std;

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

    // Sets the potential
    HarmonicOscillator HOPotential;

    // Sets the action with potential
    Action S(N,a);
    S.setPotential(&HOPotential);

    // Initialise the observable/correlator
    CorrelatorX1 x1Correlator(N);
    CorrelatorX3 x3Correlator(N);

    System system(N, NCf, NCor, NTherm, epsilon, a);
    system.setAction(&S);

    // Using gammaFunctional
    system.setGammaFunctional(&x1Correlator);
    system.run();
    system.getStatistics();
    system.writeDataToFile("output/gammaFunctional_gamma.txt");
    system.writeStatisticsToFile("output/gammaFunctional_stats.txt");
    system.printEnergies();
    system.printAcceptanceRate();

    // Using gammaFunctional2
    system.setGammaFunctional(&x3Correlator);
    system.run();
    system.getStatistics();
    system.writeDataToFile("output/gammaFunctional2_gamma.txt");
    system.writeStatisticsToFile("output/gammaFunctional2_stats.txt");
    system.printEnergies();
    system.printAcceptanceRate();

    // Using improved action, <x(t)x(0)>
    double m = 1.0;         // Mass
    ImpAction SImp(N,a,m);
    SImp.setPotential(&HOPotential);
    system.setAction(&SImp);
    system.setGammaFunctional(&x1Correlator);
    system.run();
    system.getStatistics();
    system.writeDataToFile("output/improvedAction_gamma.txt");
    system.writeStatisticsToFile("output/improvedAction_stats.txt");
    system.printEnergies();
    system.printAcceptanceRate();

    // <x^3(t)x^3(0)>
    system.setGammaFunctional(&x3Correlator);
    system.run();
    system.getStatistics();
    system.writeDataToFile("output/improvedAction2_gamma.txt");
    system.writeStatisticsToFile("output/improvedAction2_stats.txt");
    system.printEnergies();
    system.printAcceptanceRate();

    programEnd = clock();
    cout << "Program complete. Time used: " << ((programEnd - programStart)/((double)CLOCKS_PER_SEC)) << endl;
    return 0;
}
