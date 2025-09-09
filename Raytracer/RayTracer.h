#pragma once
#include "Shapes.h"

#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>

#include "json.hpp"


#include <sstream>
#include <fstream>
#include <cstdio>
#include <math.h>
#include <chrono>

#include <string>
#include "jsonClass.h"
#include "Camera.h"
#include "Helper.h"
#include <thread>
#include "Light.h"
#include "Ray.h"
#include "Camera.h"
#include "Helper.h"


using namespace std;
using namespace nlohmann;

template<typename T>
void vPrint(string, T);


int saveppm(string, vector<double>&, double, double);
string uppercase(string s);
string lowercase(string s);




class RayTracer
{
public:
	JsonParse myJ;

	RayTracer();
	RayTracer(nlohmann::json&);
	~RayTracer();

	void run();

	bool parseGeo(nlohmann::json&);
	bool parseLight(nlohmann::json&);
	bool parseOutput(nlohmann::json&);

	template<typename T>
	bool getElements(nlohmann::detail::iter_impl<nlohmann::json>, std::string, int, T&);
	void makeThreads(int threadCount, int size, vector<double>* buffer, vector<Geometry*>* objects, vector<Light*>* lights, Camera& cam);


private:

};

/*
Recall MAST218
ray as a function of a line (L)
L(t) = P + tV

where P is your starting point on the line (x, y, z)
where V is the direction vector of the line (x, y, z)
where t is any integer
where L is the resulting point at value t
*/



//Helper to print all the arrays
template<typename T>
void vPrint(string theStr, T struc)
{
	auto p = *struc->FloatMemberMap[theStr](*struc);

	cout << "[ ";
	for (int i = 0; i < 3; i++)
		cout << p[i] << "  ";
	cout << "]\n";
}



void startTrace(int id,  int totalWidth, int totalHeight, vector<double>* arr2, vector<Geometry*>* objects, vector<Light*>* lights, Camera& cam, int baseWidth, int baseHeight, int imgWidth, int imgHeight);
