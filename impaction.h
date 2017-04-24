#ifndef IMPACTION_H
#define IMPACTION_H


class ImpAction
{
private:
    double a;
    double m;
    int N;

    double (*potential)(double x);
public:
    ImpAction(int NPathPoints, double latticeSpacing, double mass);
    double getAction(double * x, int i);
    // Sets the action
    double (*S)(double * x, int i);
    // Sets the potential
    void setPotential(double (*pot)(double x)) { potential = pot; }
};

#endif // IMPACTION_H
