#pragma once
#include <Eigen/Core>
#include <Eigen/Dense>
#include "Shapes.h"
#include <vector>
#include <string>
#include "Ray.h"
using namespace std;


class HitContainer;
class Geometry;

struct Cell
{
	Eigen::Vector2f xInterval;
	Eigen::Vector2f yInterval;
	Eigen::Vector2f zInterval;

	vector<Geometry*> objects;
};


class Grid
{
public:


	Eigen::Vector2f xInterval;
	Eigen::Vector2f yInterval;
	Eigen::Vector2f zInterval;

	double xDist, yDist, zDist;

	int xLength;
	int yLength;
	int zLength;

	double nX;
	double nY;
	double nZ;

	Cell*** theGrid;

	Grid();
	Grid(Eigen::Vector2f x, Eigen::Vector2f y, Eigen::Vector2f z);
	~Grid();

	void operator()(double size);
	void fill(vector<Geometry*> g);
	HitContainer march(Ray r);
private:
};

