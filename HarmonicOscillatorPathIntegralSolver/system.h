#ifndef SYSTEM_H
#define SYSTEM_H

#include <random>
#include <vector>
#include <string>
#include "actions/action.h"
#include "observables/observable.h"

class Action;

class System
{
private:
    unsigned int m_N;
    unsigned int m_NCf;
    unsigned int m_NCor;
    unsigned int m_NTherm;
    double m_epsilon;
    double m_a;
    unsigned long long m_acceptanceCounter;

    // Our space
    std::vector<double> m_x;

    // Observable storage unit
    std::vector<double> m_obsData;

    // Variables used to perform statistics
    std::vector<double> m_varianceGamma;
    std::vector<double> m_stdGamma;
    std::vector<double> m_stdDeltaE;
    std::vector<double> m_averagedGamma;
    std::vector<double> m_averagedGammaSquared;
    std::vector<double> m_deltaE;

    // Storing the action as a pointer
    Action *m_S = nullptr;

    // Storing the Gamma functional
    Observable *m_Obs = nullptr;


    // RNGs
    std::mt19937_64 m_generator;
    std::uniform_real_distribution<double> m_epsilonDistribution;
    std::uniform_real_distribution<double> m_uniformDistribution;

    // Function for updating our system using the Metropolis algorithm
    void update();
public:
    System(unsigned int N, unsigned int NCf, unsigned int NCor, unsigned int NTherm, double epsilon, double a);
    ~System();
    void run();
    void getStatistics();
    void writeStatisticsToFile(const std::string filename);
    void writeDataToFile(const std::string filename);

    // Setters
    void setAction(Action *S) { m_S = S; }
    void setGammaFunctional(Observable *Obs) { m_Obs = Obs; }
    void setN(unsigned int N) { m_N = N; }
    void setNCf(unsigned int NCf) { m_NCf = NCf; }
    void setEpsilon(double epsilon) { m_epsilon = epsilon; }

    // Getters
    unsigned int getN() const { return m_N; }
    unsigned int getNCf() const { return m_NCf; }
    double getEpsilon() { return m_epsilon; }

    // Printers
    void printEnergies();
    void printAcceptanceRate();
};

#endif // SYSTEM_H
