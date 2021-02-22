#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>
#include "point.h"

#define LAMBDA 0.00005
#define THRESHOLD 0.00000001
#define SEPARATOR "******** %s ********\n"

void getGradient(Point **, int, double, double, double *, double *);
void simpleLinarRegression(Point **, int, double *);
void verycomplicatedLinearRegression(Point **, int, double *, double *);
void mostestComplicatedLinearRegression(Point **, int, double, double, double *, double *);
void validateRegressionModel(int);

/*
 * Function: simpleLinarRegression 
 * --------------------
 * Analytical linar regression. Compute the slope "a" assuming it's passing through coordinate (0,0),
 * given an array of points
 *
 * points: an array containing all the points
 * count: how many points are in the points array
 * a: pointer to a double that will be the result slope
 */
void simpleLinarRegression(Point **points, int count, double *a)
{
    // E(a) = SUM(X^2)⋅a^2 − SUM(XY)⋅a + SUM(Y^2)
    // We have three coeficients based on sum of our points, that will allow us to find our error :
    double sumX = 0;
    double sumXY = 0;
    double sumY = 0;

    for (int i = 0; i < count; i++)
    {
        sumX += points[i]->x * points[i]->x;
        sumXY += points[i]->x * points[i]->y;
        sumY += points[i]->y * points[i]->y;
    }

    // if function E(a)  = sumX * a^2 − 2 * sumXY * a^1 + sumY
    // which means E'(a) = 2 * sumX * a - 1 * 2 * sumXY
    double sumXp = 2 * sumX;
    double sumXYp = -(2 * sumXY);

    // now solve E'(a) = 0
    *a = -(sumXYp) / sumXp;
}

/*
 * Function: verycomplicatedLinearRegression 
 * --------------------
 * Analytical linar regression. Compute the slope "a" and the intercept "b" of a line
 * given an array of points
 *
 * points: an array containing all the points
 * count: how many points are in the points array
 * a: pointer to a double that will be the result slope
 * b: pointer to a double that will be the result intercept
 */
void verycomplicatedLinearRegression(Point **points, int count, double *a, double *b)
{
    double sum_xi_squared = 0;
    double sum_xi = 0;
    double sum_xi_yi = 0;
    double sum_yi = 0;
    double xi = 0;
    double yi = 0;

    for (int i = 0; i < count; i++)
    {
        xi = points[i]->x;
        yi = points[i]->y;

        sum_xi_squared += (xi * xi);
        sum_xi += xi;
        sum_xi_yi += (xi * yi);
        sum_yi += yi;
    }

    *a = (count * sum_xi_yi - sum_xi * sum_yi) / (count * sum_xi_squared - sum_xi * sum_xi);
    *b = (sum_yi - *a * sum_xi) / count;
}

/*
 * Function: getGradient 
 * --------------------
 * Compute the gradient for a given array of points and a line (ax + b)
 *
 * points: an array containing all the points
 * count: how many points are in the points array
 * a: the slope of the line
 * b: the intercept of the line
 * x: pointer to a double that will be the resulting X factor for the gradient vector
 * y: pointer to a double that will be the resulting Y factor for the gradient vector
 */
void getGradient(Point **points, int count, double a, double b, double *x, double *y)
{
    double sum_xi_squared = 0;
    double sum_xi = 0;
    double sum_xi_yi = 0;
    double sum_yi = 0;

    for (int i = 0; i < count; i++)
    {
        double xi = points[i]->x;
        double yi = points[i]->y;

        sum_xi_squared += (xi * xi);
        sum_xi += xi;
        sum_xi_yi += (xi * yi);
        sum_yi += yi;
    }

    // A
    // 2*a * SumXi_Squared      +    2b * SumXi +   - 2 * SumXiYi
    *x = (2 * a * sum_xi_squared) + (2 * b * sum_xi) - (2 * sum_xi_yi);
    // B
    // 2*a * SumXi      +       2*b * N     -       2 * SumYi
    *y = (2 * a * sum_xi) + (2 * b * count) - (2 * sum_yi);
}

/*
 * Function: mostestComplicatedLinearRegression 
 * --------------------
 * Numerical linar regression. Compute the slope "a" and the intercept "b" of a line
 * given an array of points
 *
 * points: an array containing all the points
 * count: how many points are in the points array
 * threshold: the limit at which the estimated line is good enough to be returned
 * lambda: the rate at which the estimation will converge to the result
 * a: pointer to a double that will be the result slope
 * b: pointer to a double that will be the result intercept
 */
void mostestComplicatedLinearRegression(Point **points, int count, double threshold, double lambda, double *a, double *b)
{
    *a = 0;
    *b = 0;
    double x;
    double y;
    double newA;
    double newB;

    // keep looping until the result is precise enough
    while (true)
    {
        getGradient(points, count, *a, *b, &x, &y);

        newA = *a - (lambda * x);
        newB = *b - (lambda * y);

        if (isnan(newA) || isnan(newB))
        {
            fprintf(stderr, "Calculation overflowed\n");
            exit(EXIT_FAILURE);
        }

        if ((fabs(newA - *a) < threshold) && (fabs(newB - *b) < threshold))
        {
            // found a solution that is within the precision of the threshold
            return;
        }
        else
        {
            *a = newA;
            *b = newB;
        }
    }
}

void validateRegressionModel(int nbOfPointsPerGroup)
{
    // Points are randomly generated around a slope (ax+b), we use the same for all points
    double slope = 1;
    double intercept = 10;

    Point **G1 = createRandomPoints(nbOfPointsPerGroup, slope, intercept);
    Point **G2 = createRandomPoints(nbOfPointsPerGroup, slope, intercept);
    Point **G3 = createRandomPoints(nbOfPointsPerGroup, slope, intercept);

    Point **G1G2 = mergePoints(G1, G2, nbOfPointsPerGroup);
    Point **G1G3 = mergePoints(G1, G3, nbOfPointsPerGroup);
    Point **G2G3 = mergePoints(G2, G3, nbOfPointsPerGroup);

    double aG1G2 = 0;
    double bG1G2 = 0;
    mostestComplicatedLinearRegression(G1G2, nbOfPointsPerGroup * 2, THRESHOLD, LAMBDA, &aG1G2, &bG1G2);
    export_points("./bin/G1_U_G2.csv", G1G2, nbOfPointsPerGroup * 2, aG1G2, bG1G2);

    double aG3 = 0;
    double bG3 = 0;
    mostestComplicatedLinearRegression(G3, nbOfPointsPerGroup, THRESHOLD, LAMBDA, &aG3, &bG3);
    export_points("./bin/G3.csv", G3, nbOfPointsPerGroup, aG3, bG3);

    double aG1G3 = 0;
    double bG1G3 = 0;
    mostestComplicatedLinearRegression(G1G3, nbOfPointsPerGroup * 2, THRESHOLD, LAMBDA, &aG1G3, &bG1G3);
    export_points("./bin/G1_U_G3.csv", G1G3, nbOfPointsPerGroup * 2, aG1G3, bG1G3);

    double aG2 = 0;
    double bG2 = 0;
    mostestComplicatedLinearRegression(G2, nbOfPointsPerGroup, THRESHOLD, LAMBDA, &aG2, &bG2);
    export_points("./bin/G2.csv", G2, nbOfPointsPerGroup, aG2, bG2);

    double aG2G3 = 0;
    double bG2G3 = 0;
    mostestComplicatedLinearRegression(G2G3, nbOfPointsPerGroup * 2, THRESHOLD, LAMBDA, &aG2G3, &bG2G3);
    export_points("./bin/G2_U_G3.csv", G2G3, nbOfPointsPerGroup * 2, aG2G3, bG2G3);

    double aG1 = 0;
    double bG1 = 0;
    mostestComplicatedLinearRegression(G1, nbOfPointsPerGroup, THRESHOLD, LAMBDA, &aG1, &bG1);
    export_points("./bin/G1.csv", G1, nbOfPointsPerGroup, aG1, bG1);

    printf("slope of G1 U G2                                   : %.3f (expected %.3f)\n", aG1G2, slope);
    printf("intercept of G1 U G2                               : %.3f (expected %.3f)\n", bG1G2, intercept);
    printf("slope of G3                                        : %.3f (expected %.3f)\n", aG3, slope);
    printf("intercept of G3                                    : %.3f (expected %.3f)\n", bG3, intercept);
    printf("error between G1 U G2 and G3                       : %.3f, %.3f\n", fabs(aG1G2 - aG3), fabs(bG1G2 - bG3));
    printf("error between G1 U G2 and original slope/intercept : %.3f, %.3f\n", fabs(aG1G2 - slope), fabs(bG1G2 - intercept));
    printf("error between G3 and original slope/intercept      : %.3f, %.3f\n\n", fabs(aG3 - slope), fabs(bG3 - intercept));

    printf("slope of G1 U G3                                   : %.3f (expected %.3f)\n", aG1G3, slope);
    printf("intercept of G1 U G3                               : %.3f (expected %.3f)\n", bG1G3, intercept);
    printf("slope of G2                                        : %.3f (expected %.3f)\n", aG2, slope);
    printf("intercept of G2                                    : %.3f (expected %.3f)\n", bG2, intercept);
    printf("error between G1 U G3 and G2                       : %.3f, %.3f\n", fabs(aG1G3 - aG2), fabs(bG1G3 - bG2));
    printf("error between G1 U G3 and original slope/intercept : %.3f, %.3f\n", fabs(aG1G3 - slope), fabs(bG1G3 - intercept));
    printf("error between G2 and original slope/intercept      : %.3f, %.3f\n\n", fabs(aG2 - slope), fabs(bG2 - intercept));

    printf("slope of G2 U G3                                   : %.3f (expected %.3f)\n", aG2G3, slope);
    printf("intercept of G2 U G3                               : %.3f (expected %.3f)\n", bG2G3, intercept);
    printf("slope of G1                                        : %.3f (expected %.3f)\n", aG1, slope);
    printf("intercept of G1                                    : %.3f (expected %.3f)\n", bG1, intercept);
    printf("error between G2 U G3 and G1                       : %.3f, %.3f\n", fabs(aG2G3 - aG1), fabs(bG2G3 - bG1));
    printf("error between G2 U G3 and original slope/intercept : %.3f, %.3f\n", fabs(aG2G3 - slope), fabs(bG2G3 - intercept));
    printf("error between G1 and original slope/intercept      : %.3f, %.3f\n\n", fabs(aG1 - slope), fabs(bG1 - intercept));

    free_points(G1, nbOfPointsPerGroup);
    free_points(G2, nbOfPointsPerGroup);
    free_points(G3, nbOfPointsPerGroup);
    free_points(G1G2, nbOfPointsPerGroup * 2);
    free_points(G1G3, nbOfPointsPerGroup * 2);
    free_points(G2G3, nbOfPointsPerGroup * 2);
    free(G1);
    free(G2);
    free(G3);
    free(G1G2);
    free(G1G3);
    free(G2G3);
}

int main()
{
    // Initialize a seed for rng
    srand(time(NULL));

    int coordinatesCount = 200;
    Point **points = createPointsOnLine(coordinatesCount, -1, 2);

    // Simple linear regression going through 0,0 for simple points
    {
        printf(SEPARATOR, "simple linear regression for given points");
        double slope = 0;
        simpleLinarRegression(points, coordinatesCount, &slope);
        printf("Slope (A) = %f, Intercept (B) = 0 (hard coded)\n\n", slope);
        export_points("./bin/simple_linear_regression.csv", points, coordinatesCount, slope, 0.0);
    }

    // Analytic linear regression for simple points
    {
        printf(SEPARATOR, "Full linear regression for given points");
        double slope = 0;
        double intercept = 0;
        verycomplicatedLinearRegression(points, coordinatesCount, &slope, &intercept);
        printf("Slope (A) = %f, Intercept (B) = %f\n\n", slope, intercept);
        export_points("./bin/full_linear_regression.csv", points, coordinatesCount, slope, intercept);
    }

    // Numeric linear regression for simple points
    {
        printf(SEPARATOR, "Numeric linear regression for given points");
        double a = 0;
        double b = 0;
        mostestComplicatedLinearRegression(points, coordinatesCount, THRESHOLD, LAMBDA, &a, &b);
        printf("Slope (A) = %f, Intercept (B) = %f\n\n", a, b);
        export_points("./bin/numeric_linear_regression.csv", points, coordinatesCount, a, b);
    }

    // now we compare analytic vs numerical with random points
    {
        printf(SEPARATOR, "Analytic vs Numeric for random points");
        double a = 0;
        double b = 0;
        int pCount = 200;
        Point **points = createRandomPoints(pCount, 1, 1);
        verycomplicatedLinearRegression(points, pCount, &a, &b);
        printf("With analytic we find: a: %f b: %f\n", a, b);
        a = 0;
        b = 0;
        mostestComplicatedLinearRegression(points, pCount, THRESHOLD, LAMBDA, &a, &b);
        printf("With numerical we find: a: %f b: %f\n", a, b);
        //export_points("./bin/test.csv", points, pCount, a, b);
        free_points(points, pCount);
        free(points);
    }

    printf("\n");
    printf(SEPARATOR, "Validate regression model using cross validation");
    validateRegressionModel(250);

    // free memory
    free_points(points, coordinatesCount);
    free(points);

    return 0;
}