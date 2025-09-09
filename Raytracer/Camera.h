#pragma once
#include <iostream>
#include <Eigen/Core>
#include <Eigen/Dense>

#include "json.hpp"

#include <sstream>
#include <fstream>
#include <cstdio>

#include "Helper.h"
#include "jsonClass.h"
using namespace std;


class Camera
{
public:
	Eigen::Vector3f position;
	Eigen::Vector3f lookat;
	Eigen::Vector3f up;
	double fov;
	double aspectRatio;
	Eigen::Vector3f bkc;


	Eigen::Matrix4d projectionMatrix;
	Eigen::Matrix4d inverseProjMatrix;
	Eigen::Matrix4d viewMatrix;
	Eigen::Matrix4d inverseViewMatrix;
	Eigen::Matrix4d mvpMatrix;

	int closestIndex;
	double i, j;
	double imgWidth, imgHeight;

	Camera();
	Camera(Eigen::Vector3f position, Eigen::Vector3f lookat, double fov, double aspectRatio, Eigen::Vector3f up);
	~Camera();

private:
};