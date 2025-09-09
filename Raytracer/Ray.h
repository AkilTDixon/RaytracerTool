#pragma once
#include <Eigen/Core>
#include <Eigen/Dense>

class Ray
{
public:
	Eigen::Vector3d originP;
	Eigen::Vector3d directionV;

	double maxDist;

	Ray();
	Ray(Eigen::Vector3d, Eigen::Vector3d);
	~Ray();


	Eigen::Vector3d getCoord(double);

private:
};

