#pragma once
#include <Eigen/Core>
#include <Eigen/Dense>
#include "Shapes.h"
#include <vector>
#include <string>
#include "Ray.h"
#include "Helper.h"
using namespace std;


class HitContainer;

struct KNode
{
	Eigen::Vector2d xInterval;
	Eigen::Vector2d yInterval;
	Eigen::Vector2d zInterval;
	int axisSplit;
	int intIndex;

	int index;



	bool leaf;
	vector<Geometry*> objects;

	KNode* left;
	KNode* right;
};


void setLeftInterval(KNode* node, KNode* nextNode, int index);
void setRightInterval(KNode* node, KNode* nextNode, int index);

Eigen::Vector2d getInterval(KNode* node, int index);



class KDTree
{
public:


	KNode* root;
	double tRStart;
	double tREnd;

	KDTree();
	KDTree(Eigen::Vector2d xInt, Eigen::Vector2d yInt, Eigen::Vector2d zInt);
	//KDTree(Eigen::Vector2f xInt, Eigen::Vector2f yInt);
	~KDTree();

	void operator()(Eigen::Vector2d xInt, Eigen::Vector2d yInt, Eigen::Vector2d zInt);
	//void operator()(Eigen::Vector2f xInt, Eigen::Vector2f yInt);
	void insertObject(vector<Geometry*> g);
	void build(KNode* node, int depth);
	void clean(KNode* node);
	void traverse(KNode* parent, KNode* node, vector<Geometry*> g);
	HitContainer traverse2(Ray r, KNode* node, double tStart, double tEnd, HitContainer* origin, bool shadow);
	HitContainer findIntersect(Ray r, HitContainer* origin, bool shadow);
	bool cull(KNode* node);

private:
};