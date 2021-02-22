struct T_point
{
    double x;
    double y;
};

typedef struct T_point Point;

Point *createPoint(double, double);
Point *copyPoint(Point *);
Point **createPoints(double[][2], int);
Point **createRandomPoints(int, double, double);
Point **createPointsOnLine(int, double, double);
Point **mergePoints(Point **, Point **, int);
double randfrom(double, double);
void free_point(Point *);
void free_points(Point **, int);
void export_points(const char *, Point **, int, double, double);