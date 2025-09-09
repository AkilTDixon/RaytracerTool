#include "Ray.h"
using namespace std;


Ray::Ray()
{

}

Ray::Ray(Eigen::Vector3d o, Eigen::Vector3d d)
{
    originP = o;
    directionV = d;
}
Ray::~Ray()
{

}

Eigen::Vector3d Ray::getCoord(double t)
{
    /*

    Equation of a Line:
    Origin + (t)Direction = L

    */

    Eigen::Vector3d point = originP + (t * directionV);

    return point;

}