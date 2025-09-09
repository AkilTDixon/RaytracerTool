#include "Camera.h"
using namespace std;

Camera::Camera()
{
	position = Eigen::Vector3f(0, 0, 0);
	lookat = Eigen::Vector3f(0, 0, -1);
	fov = 90;
	aspectRatio = 1.0;
	up = Eigen::Vector3f(0, 1, 0);

	lookat = lookat.normalized();

	Eigen::Vector3f right(lookat.cross(up));
	right = right.normalized();

	Eigen::Vector3f upV = right.cross(lookat);


	viewMatrix(0, 0) = right.x();
	viewMatrix(1, 0) = upV.x();
	viewMatrix(2, 0) = -lookat.x();
	viewMatrix(3, 0) = 0;

	viewMatrix(0, 1) = right.y();
	viewMatrix(1, 1) = upV.y();
	viewMatrix(2, 1) = -lookat.y();
	viewMatrix(3, 1) = 0;

	viewMatrix(0, 2) = right.z();
	viewMatrix(1, 2) = upV.z();
	viewMatrix(2, 2) = -lookat.z();
	viewMatrix(3, 2) = 0;

	viewMatrix(0, 3) = 0;
	viewMatrix(1, 3) = 0;
	viewMatrix(2, 3) = 0;
	viewMatrix(3, 3) = 1;

	Eigen::Matrix4d translation = makeTranslation(position);



	Eigen::Matrix4d n = viewMatrix * translation;
	inverseViewMatrix = n.inverse();

}
Camera::Camera(Eigen::Vector3f pos, Eigen::Vector3f dir, double f, double ar, Eigen::Vector3f u)
{
	position = pos;
	fov = f;
	aspectRatio = ar;
	up = u;

	lookat = dir;
	lookat = lookat.normalized();

	Eigen::Vector3f right(lookat.cross(up));
	right = right.normalized();

	Eigen::Vector3f upV = right.cross(lookat);


	viewMatrix(0, 0) = right.x();
	viewMatrix(1, 0) = upV.x();
	viewMatrix(2, 0) = -lookat.x();
	viewMatrix(3, 0) = 0;

	viewMatrix(0, 1) = right.y();
	viewMatrix(1, 1) = upV.y();
	viewMatrix(2, 1) = -lookat.y();
	viewMatrix(3, 1) = 0;

	viewMatrix(0, 2) = right.z();
	viewMatrix(1, 2) = upV.z();
	viewMatrix(2, 2) = -lookat.z();
	viewMatrix(3, 2) = 0;

	viewMatrix(0, 3) = 0;
	viewMatrix(1, 3) = 0;
	viewMatrix(2, 3) = 0;
	viewMatrix(3, 3) = 1;
	
	Eigen::Matrix4d translation = makeTranslation(position);



	Eigen::Matrix4d n = viewMatrix * translation;
	inverseViewMatrix = n.inverse();

}

Camera::~Camera()
{

}