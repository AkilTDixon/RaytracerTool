#pragma once
#include <functional>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>
#include "LinkedList.h"



struct OutputJ
{
	map<string, function<Eigen::Vector3f*(OutputJ&)>> FloatMemberMap;
	string comment;
	string filename;
	Eigen::Vector2d size;
	Eigen::Vector3f lookat;	//lookat vector
	Eigen::Vector3f up;		//the up vector of the camera
	double fov;			//field of view
	Eigen::Vector3f centre;	//center coordinate
	Eigen::Vector3f ai;	//ambient intensity
	Eigen::Vector3f bkc;	//background color


	bool globalillum;
	bool antialiasing;
	int speedup;
	bool twosiderender;
	
	

	Eigen::Vector2d raysperpixel;
	int maxbounces;
	double probterminate;


	Eigen::Vector3f* getlookat() { return &lookat; };
	Eigen::Vector3f* getup() { return &up; };
	Eigen::Vector3f* getcentre() { return &centre; };
	Eigen::Vector3f* getai() { return &ai; };
	Eigen::Vector3f* getbkc() { return &bkc; };

	template<typename T>
	friend void vPrint(string, T);

	OutputJ();
};



struct LightJ
{
	map<string, function<Eigen::Vector3f*(LightJ&)>> FloatMemberMap;

	string comment;
	string type;

	//Area
	Eigen::Vector3f P1;	//Point 1 (x, y, z)
	Eigen::Vector3f P2;	//Point 2
	Eigen::Vector3f P3;	//Point 3
	Eigen::Vector3f P4;	//Point 4
	vector<int> order;


	//Point
	Eigen::Vector3f centre;
	bool usecenter;
	Eigen::Vector3f id;	//diffuse light intensity
	Eigen::Vector3f is;	//specular light intensity

	Eigen::Matrix4f transform;

	Eigen::Vector3f* getP1() { return &P1; }
	Eigen::Vector3f* getP2() { return &P2; }
	Eigen::Vector3f* getP3() { return &P3; }
	Eigen::Vector3f* getP4() { return &P4; }
	Eigen::Vector3f* getCentre() { return &centre; }
	Eigen::Vector3f* getid() { return &id; }
	Eigen::Vector3f* getis() { return &is; }

	template<typename T>
	friend void vPrint(string, T);


	LightJ();
};



struct GeometryJ
{
	map<string, function<Eigen::Vector3f*(GeometryJ&)>> FloatMemberMap;
	string comment;
	string type;

	//Rectangle
	Eigen::Vector3f P1;	//Point 1 (x, y, z)
	Eigen::Vector3f P2;	//Point 2
	Eigen::Vector3f P3;	//Point 3
	Eigen::Vector3f P4;	//Point 4
	vector<int> order;

	//Sphere
	Eigen::Vector3f centre;
	double radius;

	Eigen::Matrix4f transform;

	Eigen::Vector3f ac;	//Ambient reflection color of a surface
	Eigen::Vector3f dc;	//Diffuse reflection color of a surface
	Eigen::Vector3f sc;	//Specular reflection color of a surface

	double ka;	//Ambient reflection coefficient
	double kd;	//Diffuse reflection coefficient
	double ks;	//Specular reflection coefficient

	double pc;	//phong coefficient


	Eigen::Vector3f* getP1() { return &P1; }
	Eigen::Vector3f* getP2() { return &P2; }
	Eigen::Vector3f* getP3() { return &P3; }
	Eigen::Vector3f* getP4() { return &P4; }
	Eigen::Vector3f* getCentre() { return &centre; }
	Eigen::Vector3f* getac() { return &ac; }
	Eigen::Vector3f* getdc() { return &dc; }
	Eigen::Vector3f* getsc() { return &sc; }


	template<typename T>
	friend void vPrint(string, T);

	GeometryJ();

};





class JsonParse
{
public:
	MyList<shared_ptr<GeometryJ>> geo;
	MyList<shared_ptr<LightJ>> light;
	MyList<shared_ptr<OutputJ>> outP;

	JsonParse();
	~JsonParse();

	void cleanThis();

private:
};


