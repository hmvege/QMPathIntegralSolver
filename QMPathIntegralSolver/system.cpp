#include <random>   // For Mersenne-Twister19937
#include <ctime>    // For random seed
#include <cmath>    // For exp()
#include <fstream>
#include <iostream>
#include "system.h"
#include "actions/action.h"

using std::cout;
using std::endl;

System::System(unsigned int N, unsigned int NCf, unsigned int NCor, unsigned int NTherm, double epsilon, double a) :
    m_N(N), m_NCf(NCf), m_NCor(NCor), m_NTherm(NTherm), m_epsilon(epsilon), m_a(a)
{
    /*
     * Class for calculating correlators using the Metropolis algorithm.
     * Takes an action object as well as a Gamma functional to be used in the action.
     */

    // Initialise the RNGs
    m_generator = std::mt19937_64(1234);
    m_epsilonDistribution = std::uniform_real_distribution<double>(-m_epsilon, m_epsilon);
    m_uniformDistribution = std::uniform_real_distribution<double>(0,1);

    // Setting up array
    m_x = std::vector<double>(m_N, 0);
}

System::~System()
{
}

void System::update()
{
    /*
     * Private function used for updating our system. Performs the Metropolis algorithm
     */
    double xPrev = 0;
    double SPrev = 0;
    double deltaS = 0;

    for (unsigned int i = 0; i < m_N; i++)
    {
        xPrev = m_x[i];
        SPrev = m_S->getAction(m_x, i); // This only needs to be calculated once per site update, and then store the previous one.
        m_x[i] += m_epsilonDistribution(m_generator); // setting a new possible x-position to test for
//        deltaS = m_S->getAction(m_x, i) - SPrev;
        if (exp(-(m_S->getAction(m_x, i) - SPrev)) > m_uniformDistribution(m_generator))
        {
            m_acceptanceCounter++;
        } else {
            m_x[i] = xPrev;
        }
    }
}

void System::run()
{

    // Setting up array for observable values
    m_obsData = std::vector<double>(m_NCf * m_N, 0);

    // Running thermalization
    for (unsigned int i = 0; i < m_NTherm * m_NCor; i++)
    {
        update();
    }

    // Setting the Metropolis acceptance counter to 0 in order not to count the thermalization
    m_acceptanceCounter = 0;

    // Main part of algorithm
    for (unsigned int alpha = 0; alpha < m_NCf; alpha++)
    {
        for (unsigned int i = 0; i < m_NCor; i++) // Updating NCor times before updating the Gamma function
        {
            update();
        }
        for (unsigned int n = 0; n < m_N; n++)
        {
            m_obsData[alpha*m_N + n] = m_Obs->calculate(m_x,n);
        }
    }
}

void System::getStatistics()
{
    /*
     * Class instance for sampling statistics from our system.
     */
    m_averagedGamma           = std::vector<double>(m_N, 0);
    m_averagedGammaSquared    = std::vector<double>(m_N, 0);
    m_varianceGamma           = std::vector<double>(m_N, 0);
    m_stdGamma                = std::vector<double>(m_N, 0);
    m_deltaE                  = std::vector<double>(m_N, 0);
    m_stdDeltaE               = std::vector<double>(m_N, 0);

    // Performing an average over the Monte Carlo obtained values
    for (unsigned int n = 0; n < m_N; n++)
    {
        for (unsigned int alpha = 0; alpha < m_NCf; alpha++)
        {
            m_averagedGamma[n] += m_obsData[alpha*m_N + n];
            m_averagedGammaSquared[n] += m_obsData[alpha*m_N + n]*m_obsData[alpha*m_N + n];
        }
        m_averagedGamma[n] /= double(m_NCf);
        m_averagedGammaSquared[n] /= double(m_NCf);
    }

    // Getting change in energy & calculating variance & standard deviation of G
    for (unsigned int n = 0; n < m_N; n++)
    {
        m_varianceGamma[n] = (m_averagedGammaSquared[n] - m_averagedGamma[n]*m_averagedGamma[n])/m_NCf;
        m_stdGamma[n] = sqrt(m_varianceGamma[n]);
        m_deltaE[n] = log(m_averagedGamma[n]/m_averagedGamma[(n+1) % m_N])/m_a;
    }

    // Calculating the uncertainty in dE(hand calculation for analytic expression done beforehand)
    for (unsigned int n = 0; n < m_N; n++)
    {
        m_stdDeltaE[n] = sqrt(pow(m_stdGamma[n]/m_averagedGamma[n],2) + pow(m_stdGamma[(n+1)%m_N]/m_averagedGamma[(n+1)%m_N],2))/m_a;
    }
}

void System::writeDataToFile(const std::string filename)
{
    /*
     * For writing the raw Gamma data to file.
     */
    std::ofstream file;
    file.open(filename);
    file << "acceptanceCounter " << double(m_acceptanceCounter)/double(m_N*m_NCf*m_NCor) << endl;
    file << "NCor " << m_NCor << endl;
    file << "NCf " << m_NCf << endl;
    file << "NTherm " << m_NTherm << endl;
    for (unsigned int alpha = 0; alpha < m_NCf; alpha++)
    {
        for (unsigned int n = 0; n < m_N; n++)
        {
            file << m_obsData[alpha*m_N + n] << " ";
        }
        file << endl;
    }
    file.close();
    cout << filename << " written" << endl;
}

void System::writeStatisticsToFile(const std::string filename)//, double * dE, double * averagedGamma, double * averagedGammaSquared, int acceptanceCounter)
{
    /*
     * Writes statistics to file about:
     * acceptanceCounter:   the number of accepted configurations
     * NCor:                number of times between each sampling of the functional
     * NCf:                 number of paths we are looking at
     * t=n*a:               points on lattice
     * dE:                  energy for a given point on lattice
     * dE error:            the error in the dE measurment
     * variance:            Var(G)
     * standardDeviation:   std(G)
     */
    std::ofstream file;
    file.open(filename);
    file << "acceptanceCounter " << double(m_acceptanceCounter)/double(m_N*m_NCf*m_NCor) << endl;
    file << "NCor " << m_NCor << endl;
    file << "NCf " << m_NCf << endl;
    file << "NTherm " << m_NTherm << endl;
    for (unsigned int n = 0; n < m_N; n++)
    {
        file << n*m_a << " "
             << m_deltaE[n] << " "
             << m_stdDeltaE[n] << " "
             << m_varianceGamma[n] << " "
             << m_stdGamma[n] << endl;
    }
    file.close();
    cout << filename << " written" << endl;
}

void System::printEnergies()
{
    /*
     * Printing the energies in the calculation
     */
    for (unsigned int n = 0; n < m_N; n++)
    {
        cout << m_deltaE[n] << endl;
    }
}

void System::printAcceptanceRate()
{
    printf("Acceptancerate: %f \n", double(m_acceptanceCounter)/double(m_NCf*m_NCor*m_N));
}
