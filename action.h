#ifndef ACTION_H
#define ACTION_H

class Action
{
private:
    double (*potential)(double x);
    double a;
    int N;
public:
    Action(int NPathPoints, double new_a);
    virtual ~Action() {}
    virtual double getAction(double * x, int i);
    // Sets the potential
    void setPotential(double (*pot)(double x)) { potential = pot; }
};

#endif // ACTION_H
