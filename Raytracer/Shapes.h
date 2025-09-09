#pragma once
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "Camera.h"
#include "Ray.h"
#include "Sorting.h"
using namespace std;



class KNode;



class Geometry
{
public:
	string type;
	string comment;
	Eigen::Vector3f center;
	Eigen::Vector3f color;
	Eigen::Vector4d currentPixel;
	Eigen::Vector3f normal;

	Eigen::Vector3f ac;
	Eigen::Vector3f dc;
	Eigen::Vector3f sc;

	vector<KNode*> leafs;

	bool tested = false;

	double ka;
	double kd;
	double ks;

	double pc;



	Eigen::Vector2d xInterval;
	Eigen::Vector2d yInterval;
	Eigen::Vector2d zInterval;


	Geometry(); 
	Geometry(string comment,string type, Eigen::Vector3f center, double ka, double kd, double ks, Eigen::Vector3f ac, Eigen::Vector3f dc, Eigen::Vector3f sc, double pc);
	virtual ~Geometry();
	string getType() { return type; };
	virtual Eigen::VectorXd intersect(Ray r);
	virtual Eigen::Vector3f getNormal(Eigen::Vector3f p);


	Eigen::Vector2d getYBounds();
	Eigen::Vector2d getXBounds();
	Eigen::Vector2d getZBounds();


	virtual bool checkPixel(Eigen::Vector4d coord);

private:
};


Eigen::Vector2d getTVals(Ray r, Geometry* g);
class Rectangle : public Geometry
{
public:
	Eigen::Vector3f p1;
	Eigen::Vector3f p2;
	Eigen::Vector3f p3;
	Eigen::Vector3f p4;

	int axis;

	double length;
	double width;
	

	Rectangle();
	Rectangle(string comment, Eigen::Vector3f p1, Eigen::Vector3f p2, Eigen::Vector3f p3, Eigen::Vector3f p4, double ka, double kd, double ks, Eigen::Vector3f ac, Eigen::Vector3f dc, Eigen::Vector3f sc, double pc);

	void setBounds();

	virtual ~Rectangle();

	virtual Eigen::VectorXd intersect(Ray r);
	virtual Eigen::Vector3f getNormal(Eigen::Vector3f p);


	virtual bool checkPixel(Eigen::Vector4d coord);
private:
};

class Sphere : public Geometry
{
public:
	double radius;

	Sphere();
	Sphere(string comment, Eigen::Vector3f center, double radius, double ka, double kd, double ks, Eigen::Vector3f ac, Eigen::Vector3f dc, Eigen::Vector3f sc, double pc);
	virtual ~Sphere();



	virtual Eigen::VectorXd intersect(Ray r);
	virtual Eigen::Vector3f getNormal(Eigen::Vector3f p);

	virtual bool checkPixel(Eigen::Vector4d coord);

private:
};

struct HitContainer
{
	Geometry* g;
	Eigen::Vector4d lightSource;
	Eigen::Vector4d hitPixel;
	double nearestT;
	bool shadow = false;
	bool hitExists = false;
};

HitContainer loopForHits(Ray r, vector<Geometry*> g, bool shadow, HitContainer* origin);
vector<Geometry*> zSort(vector<Geometry*> g);

vector<Geometry*> selectionSortGeometry(vector<Geometry*> g, int size);