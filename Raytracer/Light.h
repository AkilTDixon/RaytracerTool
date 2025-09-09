#pragma once
#include "Shapes.h"
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "Camera.h"
using namespace std;






class Light
{
public:
	string type;
	Eigen::Vector3f center;
	Eigen::Vector3f id;
	Eigen::Vector3f is;
	Eigen::Vector3f ai;

	Eigen::Vector3f p1;	
	Eigen::Vector3f p2;	
	Eigen::Vector3f p3;	
	Eigen::Vector3f p4;	
	Eigen::Vector3f generatedP;

	bool usecenter;


	Eigen::Vector2f xInterval;
	Eigen::Vector2f yInterval;
	Eigen::Vector2f zInterval;

	Light();
	Light(string type, Eigen::Vector3f center, Eigen::Vector3f id, Eigen::Vector3f is, Eigen::Vector3f ai, bool usecenter);
	Light(string type, Eigen::Vector3f p1, Eigen::Vector3f p2, Eigen::Vector3f p3, Eigen::Vector3f p4, Eigen::Vector3f id, Eigen::Vector3f is, Eigen::Vector3f ai, bool usecenter);
	~Light();

	Eigen::Vector3f getColor(Geometry* g, Camera cam, vector<Geometry*>& objects, Eigen::Vector4d currentPixel);
	Eigen::Vector3f calcDiffuse(Eigen::Vector3f normal, Eigen::Vector3f l, Geometry* g);
	Eigen::Vector3f calcSpecular(Eigen::Vector3f normal, Eigen::Vector3f l, Eigen::Vector3f v, Geometry* g);
	Eigen::Vector3f recursiveBounce(Eigen::Vector3f sampleDir, Eigen::Vector3f point, Geometry* origObject, Eigen::Vector3f origin, int count, vector<Geometry*> g, Geometry* hitObject, Eigen::Vector3f objectNormal);
	bool shadowTest(Eigen::Vector3f point, Eigen::Vector3f normal, vector<Geometry*> g, Geometry* originalObj);
private:

};
