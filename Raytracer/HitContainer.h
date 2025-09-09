#pragma once
#include "Shapes.h"
#include <Eigen/Core>
#include <Eigen/Dense>


struct HitContainer
{
	Geometry* g;
	Eigen::Vector3f hitPixel;
};