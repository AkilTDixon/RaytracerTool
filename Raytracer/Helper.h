#pragma once
#include <Eigen/Core>
#include <Eigen/Dense>
#include <random>
#include "Ray.h"
#include "Grid.h"

class Ray;
class Grid;

namespace helper {

	const double h = 6.626176*(pow(10,-34));
	const double c = 299792458;
	const double pi = 3.14159265358979323846;
	
	extern bool globalillum;
	extern double max_bounce;
	extern Eigen::Vector2d raysperpixel;
	extern double probterminate;
	extern bool antialiasing;
	extern Eigen::Vector2f** grid;
	extern int speedup;

}

bool unitCircleCheck(double x, double y, double z);
Eigen::Vector3f rotate(double angle, Eigen::Vector3f translate);
Eigen::VectorXd makeXVector(int x);
double getCosineAngle(Eigen::Vector3f v1, Eigen::Vector3f v2);

Eigen::Matrix4d arr16toMat4(double arr[]);
Eigen::Matrix4d makeTranslation(Eigen::Vector3f point);
Eigen::Vector3f randomHemispherePoint();


Eigen::Vector3f colorOverflow(Eigen::Vector3f col);
Eigen::Vector3f vecMult(Eigen::Vector3f v1, Eigen::Vector3f v2);
Eigen::Vector3f vecDiv(Eigen::Vector3f v1, Eigen::Vector3f v2);
Eigen::Vector3f vecMod(Eigen::Vector3f v1, Eigen::Vector3f v2);
Eigen::Vector3f negateV(Eigen::Vector3f v);

double power(int n, double v, double t);
double dist(Eigen::Vector3f);
double distd(Eigen::Vector3d v);
double dist2(Eigen::Vector4d);
double uniRand(int lower, int upper);
double realUniRand(float lower, float upper);
double integrate(int N, int a, int b);
Eigen::Vector3f getPerpendicular(Eigen::Vector3f n);

bool inters(Ray r, Eigen::Vector2d xInterval, Eigen::Vector2d yInterval, Eigen::Vector2d zInterval);
bool inters1D(Ray r,Eigen::Vector2f interval, int index);
bool inters2D(Ray r, Eigen::Vector2f xInterval, Eigen::Vector2f yInterval);

Eigen::Matrix4f rotMat(int axis, float angle);

