#include "Helper.h"
#include <iostream>

using namespace std;

random_device SEEDER;
mt19937 engine(SEEDER());

Eigen::Matrix4d arr16toMat4(double arr[])
{
    Eigen::Matrix4d temp;

    temp(0, 0) = arr[0];
    temp(0, 1) = arr[1];
    temp(0, 2) = arr[2];
    temp(0, 3) = arr[3];

    temp(1, 0) = arr[4];
    temp(1, 1) = arr[5];
    temp(1, 2) = arr[6];
    temp(1, 3) = arr[7];

    temp(2, 0) = arr[8];
    temp(2, 1) = arr[9];
    temp(2, 2) = arr[10];
    temp(2, 3) = arr[11];

    temp(3, 0) = arr[12];
    temp(3, 1) = arr[13];
    temp(3, 2) = arr[14];
    temp(3, 3) = arr[15];

    return temp;

}


double power(int n, double v, double t)
{
    if (n == 1)
        return t;

    return power(n - 1, v, t * v);

}

double dist(Eigen::Vector3f v)
{
    return sqrt(v.x() * v.x() + v.y() * v.y() + v.z() * v.z());
}

double distd(Eigen::Vector3d v)
{
    return sqrt(v.x() * v.x() + v.y() * v.y() + v.z() * v.z());
}
double dist2(Eigen::Vector4d v)
{
    return sqrt(v.x() * v.x() + v.y() * v.y() + v.z() * v.z());
}

Eigen::Vector3f vecMult(Eigen::Vector3f v1, Eigen::Vector3f v2)
{
    return Eigen::Vector3f(v1.x() * v2.x(), v1.y() * v2.y(), v1.z() * v2.z());
}

Eigen::Vector3f vecDiv(Eigen::Vector3f v1, Eigen::Vector3f v2)
{
    return Eigen::Vector3f(v1.x()/v2.x(), v1.y()/v2.y(), v1.z()/v2.z());
}
Eigen::Vector3f vecMod(Eigen::Vector3f v1, Eigen::Vector3f v2)
{

    return Eigen::Vector3f(v1[1]*v2[2] - v1[2]*v2[1],v1[2]*v2[0] - v1[0]*v2[2],v1[0]*v2[1]-v1[1]*v2[0]);
}

Eigen::Vector3f negateV(Eigen::Vector3f v)
{
    return Eigen::Vector3f(-v[0],-v[1],-v[2]);
}

Eigen::Vector3f colorOverflow(Eigen::Vector3f col)
{
    for (int i = 0; i < 3; i++)
    {
        if (col[i] < 0)
            col[i] = 0;
        else if (col[i] > 1)
            col[i] = 1;
    }

    return col;
}

Eigen::VectorXd makeXVector(int x)
{
    Eigen::VectorXd ret(x);
    ret[0] = 0;
    ret[1] = 0;
    ret[2] = 0;
    ret[3] = 0;
    ret[4] = -1;


    return ret;
}

Eigen::Matrix4d makeTranslation(Eigen::Vector3f point)
{
    Eigen::Matrix4d translation;

    translation(0, 0) = 1;
    translation(1, 0) = 0;
    translation(2, 0) = 0;
    translation(3, 0) = -point.x();

    translation(0, 1) = 0;
    translation(1, 1) = 1;
    translation(2, 1) = 0;
    translation(3, 1) = -point.y();

    translation(0, 2) = 0;
    translation(1, 2) = 0;
    translation(2, 2) = 1;
    translation(3, 2) = -point.z();

    translation(0, 3) = 0;
    translation(1, 3) = 0;
    translation(2, 3) = 0;
    translation(3, 3) = 1;

    return translation;
}
double getCosineAngle(Eigen::Vector3f v1, Eigen::Vector3f v2)
{
    return (v1.dot(v2)) / ((dist(v1))*(dist(v2)));
}
double realUniRand(float lower, float upper)
{

    uniform_real_distribution<float> num(lower, upper);
    

    return num(engine);
}
double uniRand(int lower, int upper)
{

    uniform_int_distribution<int> num(lower, upper);


    return (double)num(engine);
}

Eigen::Vector3f rotate(double angle, Eigen::Vector3f translate)
{
    Eigen::Matrix3f rotationMat;

    rotationMat(0, 0) = cos(angle);
    rotationMat(0, 1) = sin(angle);
    rotationMat(0, 2) = 0;
 
    rotationMat(1, 0) = -sin(angle);
    rotationMat(1, 1) = cos(angle);
    rotationMat(1, 2) = 0;

    rotationMat(2, 0) = 0;
    rotationMat(2, 1) = 0;
    rotationMat(2, 2) = 1;

    return rotationMat * translate;

}
double integrate(int N, int a, int b)
{
    double sum = 0.0;
    double x;
    float xi = 0;

    for (int i = 0; i < N; ++i)
    {
        xi = uniRand(1,10);
        float k;

        k = ((xi*xi) / (0.2 * (xi)));
        sum += k;
    }

    return (10- 1)*(1/ (double)N) * sum;
}


bool unitCircleCheck(double x, double y, double z)
{
    return (( sqrt((x * x) + (y * y) + (z * z)) < 1.0));
}

Eigen::Vector3f randomHemispherePoint()
{
    
    double r1;
    double r2;
    double r3;
    double phi;
    float x, y, z;
    do
    {
        r1 = uniRand(0, 1000)/(double)1000.0;
        r2 = uniRand(0, 1000)/(double)1000.0;
        r3 = 1.0 - (sqrt(r1 * r1));
        phi = 2.0 * helper::pi * r2;
        x = r3 * cosf(phi);
        y = r1;
        z = r3 * sinf(phi);

    } while (!unitCircleCheck(x, y, z));


    Eigen::Vector3f unitP(x, y, z);


    return unitP;



}

Eigen::Vector3f getPerpendicular(Eigen::Vector3f n)
{

    if (n[1] != 0 || n[2] != 0)
        return n.cross(Eigen::Vector3f(1, 0, 0));
    else
        return n.cross(Eigen::Vector3f(0, 1, 0));
    
}

bool inters(Ray r, Eigen::Vector2d xInterval, Eigen::Vector2d yInterval, Eigen::Vector2d zInterval)
{
    //(x[0],y[0],z[0])
    //(x[1],y[1],z[1])
    //x = r.originP.x() + t*r.directionV.x()
    //x - r.originP.x() = t * r.directionV.x()
    //txStart = (xInterval[0] - r.originP.x())/r.directionV.x()
    //txEnd = (xInterval[1] - r.originP.x())/r.directionV.x()
    //L = r.originP + t*r.directionV
    //t = (x - r.originP.x())/ r.directionV.x()

    double temp;
    double tStart = 0;

    /*
    Equation of a Line:
    Origin + (t)Direction = L
    x = o.x() + t*D.x()
    y = o.y() + t*D.y()
    z = o.z() + t*D.z()
    */


    double txStart = (xInterval[0] - (r).originP.x()) / ((r).directionV.x());
    double txEnd = (xInterval[1] - (r).originP.x()) / ((r).directionV.x());
    if (txStart > txEnd)
        swap(txStart, txEnd);


    double tyStart = (yInterval[0] - (r).originP.y()) / ((r).directionV.y());
    double tyEnd = (yInterval[1] - (r).originP.y()) / ((r).directionV.y());
    if (tyStart > tyEnd)
        swap(tyStart, tyEnd);


    double tzStart = (zInterval[0] - (r).originP.z()) / ((r).directionV.z());
    double tzEnd = (zInterval[1] - (r).originP.z()) / ((r).directionV.z());
    if (tzStart > tzEnd)
        swap(tzStart, tzEnd);


    Eigen::Vector3d starts(txStart, tyStart, tzStart);


    tStart = txStart;
    int index = 0;
    for (int i = 0; i < 3; i++)
    {
        if (starts[i] > tStart)
        {
            tStart = starts[i];
            //index = i;
        }
    }

    //if (tStart < 0)
       // return false;
    Eigen::Vector2d interval;
    Eigen::Vector3d xyz = r.originP + tStart * r.directionV;

    if (xyz[0] < xInterval[0]-1 || xyz[0] > xInterval[1]+1)
        return false;
    else if (xyz[1] < yInterval[0]-1 || xyz[1] > yInterval[1]+1)
        return false;
    else if (xyz[2] < zInterval[0]-1 || xyz[2] > zInterval[1]+1)
        return false;

    /*for (int i = 0; i < 3; i++)
    {
        if (index != i)
        {
            switch (i)
            {
            case 0:
                interval = xInterval;
                break;
            case 1:
                interval = yInterval;
                break;
            case 2:
                interval = zInterval;
                break;
            }
            xyz = r.originP[i] + tStart * r.directionV[i];
            Eigen::Vector3f test = r.originP + tStart * r.directionV;
            if (xyz < interval[0] || xyz > interval[1])
                return false;
        }


    }*/
    return true;

}

Eigen::Matrix4f rotMat(int axis, float angle)
{
    Eigen::Matrix4f rot;
    angle = (angle * helper::pi) / 180.0;
    if (axis == 2)
    {
        rot(0, 0) = cos(angle);
        rot(1, 0) = sin(angle);
        rot(2, 0) = 0;
        rot(3, 0) = 0;

        rot(0, 1) = -sin(angle);
        rot(1, 1) = cos(angle);
        rot(2, 1) = 0;
        rot(3, 1) = 0;

        rot(0, 2) = 0;
        rot(1, 2) = 0;
        rot(2, 2) = 1;
        rot(3, 2) = 0;

        rot(0, 3) = 0;
        rot(1, 3) = 0;
        rot(2, 3) = 0;
        rot(3, 3) = 1;

    }
    else if (axis == 1)
    {
        rot(0, 0) = cos(angle);
        rot(1, 0) = 0;
        rot(2, 0) = -sin(angle);
        rot(3, 0) = 0;

        rot(0, 1) = 0;
        rot(1, 1) = 1;
        rot(2, 1) = 0;
        rot(3, 1) = 0;

        rot(0, 2) = sin(angle);
        rot(1, 2) = 0;
        rot(2, 2) = cos(angle);
        rot(3, 2) = 0;

        rot(0, 3) = 0;
        rot(1, 3) = 0;
        rot(2, 3) = 0;
        rot(3, 3) = 1;

    }
    else if (axis == 0)
    {
        rot(0, 0) = 1;
        rot(1, 0) = 0;
        rot(2, 0) = 0;
        rot(3, 0) = 0;

        rot(0, 1) = 0;
        rot(1, 1) = cos(angle);
        rot(2, 1) = sin(angle);
        rot(3, 1) = 0;

        rot(0, 2) = 0;
        rot(1, 2) = -sin(angle);
        rot(2, 2) = cos(angle);
        rot(3, 2) = 0;


        rot(0, 3) = 0;
        rot(1, 3) = 0;
        rot(2, 3) = 0;
        rot(3, 3) = 1;

    }
    return rot;
}
