#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "point.h"

#define RANDOM_MAX 10

Point *createPoint(double x, double y)
{
    Point *p = (Point *)malloc(sizeof(Point));
    p->x = x;
    p->y = y;
    return p;
}

Point **createPoints(double coordinates[][2], int count)
{
    Point **points = malloc(count * sizeof(Point *));

    for (int i = 0; i < count; i++)
        points[i] = createPoint(coordinates[i][0], coordinates[i][1]);

    return points;
}

Point *copyPoint(Point *origin)
{
    Point *p = (Point *)malloc(sizeof(Point));
    p->x = origin->x;
    p->y = origin->y;
    return p;
}

double randfrom(double min, double max)
{
    double range = (max - min);
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

Point **createRandomPoints(int count, double a, double b)
{
    Point **points = malloc(count * sizeof(Point *));

    for (int i = 0; i < count; i++)
    {
        double x = randfrom(0, RANDOM_MAX);
        double y = a * x + b + randfrom(-5, 5);
        points[i] = createPoint(x, y);
    }

    return points;
}

Point **createPointsOnLine(int count, double a, double b)
{
    Point **points = malloc(count * sizeof(Point *));

    for (int i = 0; i < count; i++)
    {
        double x = randfrom(0, RANDOM_MAX);
        double y = a * x + b;
        points[i] = createPoint(x, y);
    }

    return points;
}

Point **mergePoints(Point **G1, Point **G2, int sizeOfG1G2)
{
    Point **result = malloc(sizeOfG1G2 * 2 * sizeof(Point *));

    for (int i = 0; i < sizeOfG1G2; i++)
    {
        result[i] = copyPoint(G1[i]);
        result[sizeOfG1G2 + i] = copyPoint(G2[i]);
    }

    return result;
}

void free_point(Point *point)
{
    free(point);
}

void free_points(Point **points, int count)
{
    for (int i = 0; i < count; i++)
        free_point(points[i]);
}

void export_points(const char *filename, Point **points, int count, double a, double b)
{
    FILE *f = fopen(filename, "w+");
    fprintf(f, "%f;%f", a, b);

    for (int i = 0; i < count; i++)
        fprintf(f, "\n%f;%f", points[i]->x, points[i]->y);

    fclose(f);
}