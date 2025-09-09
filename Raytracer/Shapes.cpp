#include "Shapes.h"
#include "Helper.h"
#include <chrono>
using namespace std;


Geometry::Geometry()
{

}
bool Geometry::checkPixel(Eigen::Vector4d coord)
{
    return true;
}
Geometry::Geometry(string com, string t, Eigen::Vector3f c, double a, double d, double s, Eigen::Vector3f av, Eigen::Vector3f dv, Eigen::Vector3f sv, double p)
{
    type = t;
    center = c;
    comment = com;
    ka = a;
    kd = d;
    ks = s;

    ac = av;
    dc = dv;
    sc = sv;

    pc = p;


}
Geometry::~Geometry()
{

}
Eigen::Vector2d Geometry::getZBounds()
{
    return zInterval;
}

Eigen::Vector2d Geometry::getYBounds()
{
    return yInterval;
}

Eigen::Vector2d Geometry::getXBounds()
{
    return xInterval;
}
Eigen::VectorXd Geometry::intersect(Ray r)
{
    Eigen::VectorXd ret = makeXVector(5);
    return ret;
}
Eigen::Vector3f Geometry::getNormal(Eigen::Vector3f point)
{
    return Eigen::Vector3f(0,0,0);
}
Sphere::Sphere() : Geometry("","Sphere", Eigen::Vector3f(0, 0, 0), 0.0, 1.0, 1.0, Eigen::Vector3f(1, 0, 0), Eigen::Vector3f(1, 0, 0), Eigen::Vector3f(1, 1, 1), 10.0)
{
    center = Eigen::Vector3f(0,0,0);
    radius = 1.0;
    comment = "";

    xInterval = Eigen::Vector2d((center.x() - radius), (center.x() + radius));
    xInterval[0] = floorl(xInterval[0] * 1000) / 1000;
    xInterval[1] = floorl(xInterval[1] * 1000) / 1000;
    yInterval = Eigen::Vector2d((center.y() - radius), (center.y() + radius));
    yInterval[0] = floorl(yInterval[0] * 1000) / 1000;
    yInterval[1] = floorl(yInterval[1] * 1000) / 1000;
    zInterval = Eigen::Vector2d((center.z() - radius), (center.z() + radius));
    zInterval[0] = floorl(zInterval[0] * 1000) / 1000;
    zInterval[1] = floorl(zInterval[1] * 1000) / 1000;

}
Sphere::Sphere(string com, Eigen::Vector3f c, double r, double a, double d, double s,Eigen::Vector3f av, Eigen::Vector3f dv, Eigen::Vector3f sv, double p) : Geometry(com,"Sphere",c, a, d, s, av, dv, sv, p)
{
	center = c;
	radius = r;
    comment = com;


    xInterval = Eigen::Vector2d((center.x() - radius), (center.x() + radius));
    xInterval[0] = floorl(xInterval[0] * 1000) / 1000;
    xInterval[1] = floorl(xInterval[1] * 1000) / 1000;
    yInterval = Eigen::Vector2d((center.y() - radius), (center.y() + radius));
    yInterval[0] = floorl(yInterval[0] * 1000) / 1000;
    yInterval[1] = floorl(yInterval[1] * 1000) / 1000;
    zInterval = Eigen::Vector2d((center.z() - radius), (center.z() + radius));
    zInterval[0] = floorl(zInterval[0] * 1000) / 1000;
    zInterval[1] = floorl(zInterval[1] * 1000) / 1000;

}

Sphere::~Sphere()
{

}
bool Sphere::checkPixel(Eigen::Vector4d coord)
{
    double x = (center.x() - coord.x()), y = (center.y() - coord.y()), z = (center.z() - coord.z());

    return ((sqrtf((x*x)+(y*y)+(z*z))) <= radius);
}

Rectangle::Rectangle() : 
    Geometry("","Rectangle", Eigen::Vector3f( (((1,-1,0)-(-1,1,0))/2)+(1,-1,0) ),  0.0, 1.0, 1.0, Eigen::Vector3f(1, 0, 0), Eigen::Vector3f(1, 0, 0), Eigen::Vector3f(1, 1, 1), 10.0)
{
    p1 = Eigen::Vector3f(-1, 1, 0);
    p2 = Eigen::Vector3f(-1, -1, 0);
    p3 = Eigen::Vector3f(1, -1, 0);
    p4 = Eigen::Vector3f(1, 1, 0);

    Eigen::Vector3f v1 = (p4 - p1);
    Eigen::Vector3f v2 = (p2 - p1);

    length = dist(v1); // length
    width = dist(v2);
    comment = "";
    setBounds();

    center = ((p3 - p1) / 2.0) + p1;

    normal = (p2 - p1).cross(p4 - p1);

 
}
Rectangle::Rectangle(string com, Eigen::Vector3f _p1, Eigen::Vector3f _p2, Eigen::Vector3f _p3, Eigen::Vector3f _p4, double a, double d, double s, Eigen::Vector3f av, Eigen::Vector3f dv, Eigen::Vector3f sv, double p) :
    Geometry(com,"Rectangle",Eigen::Vector3f(((_p3 - _p1) / 2) + _p1), a, d, s, av, dv, sv, p)
{
    p1 = _p1;
    p2 = _p2;
    p3 = _p3;
    p4 = _p4;
    comment = com;
    Eigen::Vector3f v1 = (p2 - p1);
    Eigen::Vector3f v2 = (p4 - p1);

    length = dist(v1); // length
    width = dist(v2);


    setBounds();

    center = ((p3 - p1)/2.0)+p1;

    normal = (p2 - p1).cross(p4 - p1);

}
Rectangle::~Rectangle()
{

}
bool Rectangle::checkPixel(Eigen::Vector4d coord)
{
    if (coord.x() > xInterval[1] || coord.y() > yInterval[1] || coord.z() > zInterval[1])
        return false;
    if (coord.x() < xInterval[0] || coord.y() < yInterval[0] || coord.z() < zInterval[0])
        return false;

    return true;
}
void Rectangle::setBounds()
{
    xInterval = Eigen::Vector2d(min(min(p1.x(), p2.x()), min(p3.x(), p4.x())), max(max(p1.x(), p2.x()), max(p3.x(), p4.x())));
    yInterval = Eigen::Vector2d(min(min(p1.y(), p2.y()), min(p3.y(), p4.y())), max(max(p1.y(), p2.y()), max(p3.y(), p4.y())));
    zInterval = Eigen::Vector2d(min(min(p1.z(), p2.z()), min(p3.z(), p4.z())), max(max(p1.z(), p2.z()), max(p3.z(), p4.z())));

    if (xInterval[0] == xInterval[1])
        axis = 0;
    else if (yInterval[0] == yInterval[1])
        axis = 1;
    else if (zInterval[0] == zInterval[1])
        axis = 2;

    xInterval[0] = floorf(xInterval[0] * 1000) / 1000;
    xInterval[1] = floorf(xInterval[1] * 1000) / 1000;

    yInterval[0] = floorf(yInterval[0] * 1000) / 1000;
    yInterval[1] = floorf(yInterval[1] * 1000) / 1000;

    zInterval[0] = floorf(zInterval[0] * 1000) / 1000;
    zInterval[1] = floorf(zInterval[1] * 1000) / 1000;

}



Eigen::VectorXd Rectangle::intersect(Ray r)
{
    //Eigen::Vector4f ret(0, 0, 0, -1);
    Eigen::VectorXd ret = makeXVector(5);

    /*
        P = 2*(L + W)
        where L = (p1.x - p3.x)
        and W = (p1.y - p2.y)
    */


    /*
    Equation of a Plane:
    P2 - P1 = Vector1
    P4 - P1 = Vector2
    Vector1.cross(Vector2) = Normal

    Normal.dot(Vector(x-x0,y-y0,z-z0)) = 0

    where x0,y0,z0 is any point known to be on the plane

    Equation of a Line:
    Origin + (t)Direction = L
    x = o.x() + t*D.x()
    y = o.y() + t*D.y()
    z = o.z() + t*D.z()

    Intersection:
    Normal.dot(Vector((o.x() + t*D.x())-x0,(o.y() + t*D.y())-y0,(o.z() + t*D.z())-z0)) = 0


    I'll use p1 as the known point on the plane


    normal.x() * (x - x0) + normal.y() * (y-y0) + normal.z() * (z-z0) = 0
    ->
    normal.x() * ( x - p1.x() ) + normal.y() * ( y-p1.y() ) + normal.z() * ( z-p1.z() ) = 0
    ->
    normal.x() * ((o.x() + t*D.x()) - p1.x()) + normal.y() * ((o.y() + t*D.y())-p1.y()) + normal.z() * ((o.z() + t*D.z())-p1.z()) = 0
    ->
    normal.x()*o.x() + normal.x()*t*D.x() - normal.x()*p1.x() + normal.y()*o.y() + normal.y()*t*D.y()-normal.y()*p1.y() + normal.z()*o.z() + normal.z()*t*D.z()-normal.z()*p1.z() = 0
    ->
    normal.x()*t*D.x() + normal.y()*t*D.y() + normal.z()*t*D.z() = normal.x()*p1.x() - normal.x()*o.x() + normal.y()*p1.y() - normal.y()*o.y() + normal.z()*p1.z() - normal.z()*o.z()
    ->
    t * (normal.x()*D.x() + normal.y()*D.y() + normal.z()*D.z()) = normal.x()*p1.x() - normal.x()*o.x() + normal.y()*p1.y() - normal.y()*o.y() + normal.z()*p1.z() - normal.z()*o.z()
    ->
    t = ( (p1.x()*normal.x() - r.originP.x() * normal.x()) +
     (p1.x()*normal.y() - r.originP.y() * normal.y()) +
     (p1.x()*normal.z() - r.originP.z() * normal.z()) 
     ) 
     /
    ( (r.directionV.x()*normal.x() + r.directionV.y()*normal.y() + r.directionV.z()*normal.z() ) )
    
    */

    double t = ((center.x() * normal.x() - r.originP.x() * normal.x()) +
        (center.y() * normal.y() - r.originP.y() * normal.y()) +
        (center.z() * normal.z() - r.originP.z() * normal.z())
          
        )/((r.directionV.x() * normal.x() + r.directionV.y() * normal.y() + r.directionV.z() * normal.z()));
    if (t < 0)
        return ret;
    //With the t value, I can find the corresponding point on the line where the ray should intersect the plane
    Eigen::Vector3d holder = r.getCoord(t);
    Eigen::Vector3f rayPoint = Eigen::Vector3f(holder[0],holder[1],holder[2]);
    

    /* This point on the line is not necessarily within the rectangle, this needs to be checked
    by getting the vectors between the points */
    
    /* Sufficient for border checking */
    double v1 = (rayPoint - p1).dot(p4 - p1);
    double v2 = (p4 - p1).dot((p4 - p1));
    double v3 = (rayPoint - p1).dot(p2 - p1);
    double v4 = (p2 - p1).dot(p2 - p1);

    
    if (((0 <= v1) && (v1 <= v2)) &&
        ((0 <= v3) && (v3 <= v4)))   
    {
        ret[0] = rayPoint[0];
        ret[1] = rayPoint[1];
        ret[2] = rayPoint[2];
        ret[3] = t;
        ret[4] = 1;

        return ret;
    }
  
    return ret;

}
Eigen::Vector3f Rectangle::getNormal(Eigen::Vector3f point)
{
    return normal.normalized();
}



Eigen::VectorXd Sphere::intersect(Ray r)
{
 
    /*
    (x-x0)^2 + (y-y0)^2 + (z-z0)^2 = r^2
    (P + tV - C) dot (P + tV - C) = r^2
    (r.originP + (t)r.directionV - object.centre).dot(r.originP + (t)r.directionV - object.centre) = object.radius * object.radius


    This will result in an at^2 + bt + c equation


    solving for t
    Quadratic: (-b + sqrt(b^2 - 4ac))/(2a)

    
    Equation of a Line:
    Origin + (t)Direction = L
    x = o.x() + t*D.x()
    y = o.y() + t*D.y()
    z = o.z() + t*D.z()

    */
    Eigen::VectorXd ret = makeXVector(5);
    Eigen::Vector3d centerD = Eigen::Vector3d(center[0],center[1],center[2]);

    double a = r.directionV.dot(r.directionV);
    double b = (double)2.0 * (r.originP - centerD).dot(r.directionV);
    double c = (r.originP - centerD).dot(r.originP - centerD) - (radius * radius);


    double delta = b * b - 4.0 * a * c;


    //Avoid square rooting a negative
    if (delta >= 0)
    {
        double t1 = 0;
        
        if (delta == 0)
        {
            t1 = (-1.0*b) / ((double)2.0 * a);
            Eigen::Vector3d holder = r.getCoord(t1);
            currentPixel = Eigen::Vector4d(holder[0],holder[1],holder[2],1);
            if (t1 > 0)
            {
                ret[0] = currentPixel[0];
                ret[1] = currentPixel[1];
                ret[2] = currentPixel[2];
                ret[3] = t1;  
                ret[4] = 2;
                return ret;
            }
        }
        else
        {
            
            t1 = (-1*b + sqrt(delta)) / ((double)2.0 * a);
            double t2 = (-1*b - sqrt(delta)) / ((double)2.0 * a);

            Eigen::Vector3d pixel1 = r.getCoord(t1);
            Eigen::Vector3d pixel2 = r.getCoord(t2);
            double d1 = distd(r.originP - pixel1);
            double d2 = distd(r.originP - pixel2);

            //If the 2nd pixel is closer to the camera, then use that pixel
            if (d2 < d1)
            {
                currentPixel = Eigen::Vector4d(pixel2[0], pixel2[1], pixel2[2], 1);
                ret[0] = pixel2[0];
                ret[1] = pixel2[1];
                ret[2] = pixel2[2];
                ret[3] = t2;
                ret[4] = 1;
                return ret;
            }
            else
            {

                currentPixel = Eigen::Vector4d(pixel1[0], pixel1[1], pixel1[2], 1);
                ret[0] = pixel1[0];
                ret[1] = pixel1[1];
                ret[2] = pixel1[2];
                ret[3] = t1;
                ret[4] = 1;
                return ret;
            }

        }
    }

    return ret;

}


Eigen::Vector3f Sphere::getNormal(Eigen::Vector3f p)
{
    return (p - center).normalized();
}





HitContainer loopForHits(Ray r, vector<Geometry*> g, bool shadow, HitContainer* origin)
{
    int closestIndex = 0;
    HitContainer hit;
    hit.g = NULL;
    vector<Geometry*> hits;
    vector<Eigen::Vector4d> currentPixel;
    Eigen::VectorXd p;
    for (int i = 0; i < g.size(); i++)
    {

        if (origin != NULL)
        {
            if (g[i] != origin->g)
            {
                if (helper::speedup)
                {
                    if (inters(r, g[i]->xInterval, g[i]->yInterval, g[i]->zInterval))
                    {
                        p = g[i]->intersect(r);
                        if (p[4] > 0)
                        {
                            hits.push_back(g[i]);
                            currentPixel.push_back(Eigen::Vector4d(p[0], p[1], p[2], p[3]));
                           // break;
                        }
                    }
                }
                else
                {
                    p = g[i]->intersect(r);
                    if (p[4] > 0)
                    {
                        hits.push_back(g[i]);
                        currentPixel.push_back(Eigen::Vector4d(p[0], p[1], p[2], p[3]));
                        //break;
                    }
                }
            }
        }
        else
        {
            if (helper::speedup)
            {
                if (inters(r, g[i]->xInterval, g[i]->yInterval, g[i]->zInterval))
                {
                    p = g[i]->intersect(r);
                    if (p[4] > 0)
                    {
                        hits.push_back(g[i]);
                        currentPixel.push_back(Eigen::Vector4d(p[0], p[1], p[2], p[3]));
                        break;
                    }
                }
            }
            else
            {
                p = g[i]->intersect(r);
                if (p[4] > 0)
                {
                    hits.push_back(g[i]);
                    currentPixel.push_back(Eigen::Vector4d(p[0], p[1], p[2], p[3]));
                    break;
                }
            }
        }
    }

    if (hits.size() >= 1)
    {
        if (!shadow)
        {
            if (origin != NULL)
            {
                for (int k = 0; k < hits.size(); k++)
                {
                    if (closestIndex != k)
                    {
                        float d1 = dist2(currentPixel[closestIndex] - origin->hitPixel);
                        float d2 = dist2(currentPixel[k] - origin->hitPixel);

                        if (d2 < d1)
                            closestIndex = k;
                    }
                }
            }
            else
            {
                for (int k = 0; k < hits.size(); k++)
                {
                    //If the object's pixel has a higher z-value than the current closest object to the camera, then there's a new closest object
                    if (hits[k] != hits[closestIndex])
                    {
                        if (currentPixel[k].z() > currentPixel[closestIndex].z())
                            closestIndex = k;
                    }
                }
            }
        }
        else
        {
            float lRayLength = dist2(origin->lightSource - origin->hitPixel);
            for (int k = 0; k < hits.size(); k++)
            {
                //If the object's pixel has a higher z-value than the current closest object to the camera, then there's a new closest object


                float d1 = dist2(currentPixel[k] - origin->lightSource);
                float d2 = dist2(currentPixel[k] - origin->hitPixel);

                if (d1 < lRayLength && d2 < lRayLength)
                {
                    hit.g = hits[k];
                    hit.hitPixel = currentPixel[k];
                    hit.shadow = true;

                    return hit;
                }

            }

            return hit;

        }

        hit.g = hits[closestIndex];
        hit.hitPixel = currentPixel[closestIndex];


    }



    return hit;
}

vector<Geometry*> zSort(vector<Geometry*> g)
{
    //unique_lock<mutex> lock1(*helper::m);
   
    //double* zVals = new double[g.size()];


    vector<Geometry*> ret;

    //for (int i = 0; i < g.size(); i++)
    //    zVals[i] = g[i]->zInterval[1];

    //auto start = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
    //ret = selectionSortGeometry(g,g.size());
    //auto end = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
    //cout << "Execution Time: " << (end - start) << " nanoseconds" << endl << endl;
  


  
    //auto start = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
    ret = selectionSortGeometry(g, g.size());
    //auto end = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
    //cout << "Execution Time: " << (end - start) << " nanoseconds" << endl << endl;

    


    return ret;
    
}

vector<Geometry*> selectionSortGeometry(vector<Geometry*> g, int size)
{
    int theMin = 0;
    int currentVal = 0;
    for (int j = 0; j < size; j++)
    {
        for (int i = currentVal; i < size; i++)
        {
            if (g[i]->zInterval[0] > g[theMin]->zInterval[0])
                theMin = i;
        }
        swap(g[currentVal], g[theMin]);
        currentVal++;
        theMin = currentVal;
    }
    return g;
}