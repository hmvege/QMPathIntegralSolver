#ifndef RANDOM_H
#define RANDOM_H

#include <ctime>

class random
{
private:
    double ran0(long *idum);
public:
    long seed;
    random(long newSeed = std::time(nullptr));
    double uniform(double min, double max);
};

#endif // RANDOM_H
