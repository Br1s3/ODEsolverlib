#ifndef SIMULTAION_NUMERIQUE_H_INCLUED
#define SIMULTAION_NUMERIQUE_H_INCLUED

#define ABS(x) (((x) < 0) ? -(x) : (x))

int ExplicitEuler(const double dt, double t, double *x, double *v, double (*f)(double, double, double));
int SymplecticEuler(const double dt, double t, double *x, double *v, double (*f)(double, double, double));

int RK4(const double h, double t, double *x, double *v, double (*f)(double, double, double));
int RK(const double h, double t, double *x, double *v, double (*f)(double, double, double));
int Verlet(const double h, double t, double *x, double *v, double (*f)(double, double, double));


// STRUCTURE & FONCTION MUETTE UNIQUEMENT UTILISABLE POUR DOPRI45
// typedef struct
// {
//     double tn;
//     double xn;
//     double vn;
//     double an;
// } Derive_temp;

// int methode_RK_row(const int q, Derive_temp P[q], const double A[][q], const double *B, const double *C, const double h, double t, double *x, double *v, double (*f)(double, double, double));
int DOPRI45(double stepSize, double Time, double err, double *x, double *v, double (*f)(double, double, double));


#endif //SIMULTAION_NUMERIQUE_H_INCLUED
