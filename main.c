#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#if defined(PROG1)
#    define ODESOLVERLIB_IMPLEMENTATION
#    include "ODEsolverlib.h"
#elif defined(PROG2)
#    include "ODEsolverlib.h"
#else
#    include "ODEsolver.h"
#endif

/* Stress test of EDO solver with stiff equation
A stiff equation is an equation that resemble to:

    y' = a*y, a |R < 0

For exemple we take a = -15:

    y' = -15*y, y(0) = 1

the exact solution is:

    y(t) = exp(-15*t)

*/

#define UNUSED(x) (void)x


double solution_stiff_equation(double t)
{
    return exp(-15.f*t);
}

int sol_stiff_equ_format(const double dt, double t, double *x, double *v, double (*f)(double, double, double))
{
    UNUSED(dt);
    UNUSED(x);
    UNUSED(f);
    (*v) = exp(-15.f*t);
    return 0;
}

double stiff_equation(double t, double y, double dy)
{
    UNUSED(t);
    UNUSED(y);
    return -15.f*dy;
}


int main(int argc, char *argv[])
{
    argc--; argv++;
    int save_file = 0;
    int res = -1;
    FILE *streamout = stdout;

    if (argc == 1) res = atoi(argv[0]);
    switch (res)
    {
	case 0:
	    printf("WARNING: Wrong argument. Default out stdout\n");
	    break;
	case 1:
	    printf("INFO: Output results chosen files\n");
	    save_file = 1;
	    break;
	case 2:
	    printf("INFO: Output results chosen stdout\n");
	    break;
	default:
	    printf("INFO: No output results chosen. Default out stdout\n");
	    break;

    }

    const double dt = 1.f/10.f;

    int (*method[7])(const double dt, double t, double *x, double *v, double (*f)(double, double, double)) = {
	sol_stiff_equ_format,
	ExplicitEuler,
	SymplecticEuler,
	RK4,
	RK,
	Verlet
    };

    char all_names_methods[7][31] = {
	"Exact solution",
	"Explicit Euler method",
	"Semi-implicite Euler method",
	"Runge-Kutta 4 method",
	"Runge-Kutta (free ceof) method",
	"Verlet method",
	"DOPRI45 method"
    };

    double tmp;

    double y_buff[7][(int)(1.f/dt)+1];
    double t_buff[7][(int)(1.f/dt)+1];

    // Exact solution don't need y variable
    for (double step = 0, t = 0; step < (int)1.f/dt; step++, t += dt) {
	t_buff[0][(int)step] = t;
	if (method[0](dt, t, &tmp, &y_buff[0][(int)step], stiff_equation) < 0)
	    fprintf(stderr, "ERROR: Computes exact solution\n");
    }

    // All other method except DOPRI45
    for (int i = 1; i < 6; i++) {
	for (double step = 0, t = 0, y = 1; step < (int)1.f/dt; step++, t += dt) {
	    t_buff[i][(int)step] = t;
	    y_buff[i][(int)step] = y;
	    // if (method[i](dt, t, &tmp, &y, stiff_equation) < 0)
	    if (method[i](dt, t, &tmp, &y, stiff_equation) < 0)
	        fprintf(stderr, "ERROR: Computes %s\n", all_names_methods[i]);
	}
    }

    // DOPRI45 modify the time itself to obtain a better accuracy
    for (double step = 0, t = 0, y = 1; step < (int)1.f/dt; step++, t += dt) {
	t_buff[6][(int)step] = t;
	y_buff[6][(int)step] = y;
	if (DOPRI45(dt, t, 0.001, &tmp, &y, stiff_equation) < 0)
	    fprintf(stderr, "ERROR: Computes methode_DOPRI45\n");
    }


    // Show or save the work
    for (int i = 0; i < 7; i++) {
	if (save_file == 1) {
	    char file_name[50];
	    sprintf(file_name, "results/%s.csv", all_names_methods[i]);
	    streamout = fopen(file_name, "w");
	    if (streamout == NULL) {
    		fprintf(stderr, "ERROR: Failed to write file: %s\n", file_name);
		exit(1);
	    }
	}

        printf("\n%s:\n", all_names_methods[i]);

	for (int step = 0; step < (int)1.f/dt; step++)
	    fprintf(streamout, "%lf,%lf\n", t_buff[i][step], y_buff[i][step]);
	if (save_file == 1) fclose(streamout);
    }

    return 0;
}
