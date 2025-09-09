#include "Light.h"
using namespace std;


Light::Light()
{
	type = "point";
	center = Eigen::Vector3f(0, 0, 0);
	usecenter = true;
	is = Eigen::Vector3f(0, 0, 0);
	id = Eigen::Vector3f(0, 0, 0);
}
Light::Light(string t, Eigen::Vector3f c, Eigen::Vector3f d, Eigen::Vector3f s, Eigen::Vector3f a, bool uc)
{
	type = t;
	center = c;
	usecenter = uc;
	is = s;
	id = d;
	ai = a;
}
Light::Light(string t, Eigen::Vector3f point1, Eigen::Vector3f point2, Eigen::Vector3f point3, Eigen::Vector3f point4, Eigen::Vector3f d, Eigen::Vector3f s, Eigen::Vector3f a, bool uc)
{
	type = t;
	p1 = point1;
	p2 = point2;
	p3 = point3;
	p4 = point4;

	center = ((p3 - p1) / 2) + p1;
	usecenter = uc;
	xInterval = Eigen::Vector2f(min(min(p1.x(), p2.x()), min(p3.x(), p4.x())), max(max(p1.x(), p2.x()), max(p3.x(), p4.x())));
	yInterval = Eigen::Vector2f(min(min(p1.y(), p2.y()), min(p3.y(), p4.y())), max(max(p1.y(), p2.y()), max(p3.y(), p4.y())));
	zInterval = Eigen::Vector2f(min(min(p1.z(), p2.z()), min(p3.z(), p4.z())), max(max(p1.z(), p2.z()), max(p3.z(), p4.z())));


	is = s;
	id = d;
	ai = a;
}
Light::~Light()
{

}



Eigen::Vector3f Light::getColor(Geometry* g, Camera cam, vector<Geometry*>& objects, Eigen::Vector4d currentPixel)
{
	Eigen::Vector3f point(currentPixel[0], currentPixel[1], currentPixel[2]);

	//Fetch the normal at the current pixel
	Eigen::Vector3f normal = g->getNormal(point);

	Eigen::Vector3f indirect(0,0,0);



	if (helper::globalillum)
	{
		

		Eigen::Vector3f A = randomHemispherePoint();

		Eigen::Vector3f ranDir;
		float d;
		do
		{
			ranDir = (Eigen::Vector3f(point[0] + uniRand(1, 10), point[1] + uniRand(1, 10), point[2] + uniRand(1, 10)) - point).normalized();
			d = normal.dot(ranDir);
		} while (d <= 1.0 && d > 0.85);
		Eigen::Vector3f x = ranDir.cross(normal).normalized();
		Eigen::Vector3f z = x.cross(normal).normalized();

		Eigen::Matrix3f r;

		r(0, 0) = x[0];
		r(0, 1) = x[1];
		r(0, 2) = x[2];

		r(1, 0) = normal[0];
		r(1, 1) = normal[1];
		r(1, 2) = normal[2];

		r(2, 0) = z[0];
		r(2, 1) = z[1];
		r(2, 2) = z[2];

		Eigen::Vector3f B = (r.inverse() * A) + point;

		Eigen::Vector3f sampleDir = (B - point).normalized();


	

		if (type == "area" && !usecenter)
		{
			Eigen::Vector3f lightContrib(0, 0, 0);
			int shadowCount = 0;
			bool stuckInShadow = false;
			for (int s = 0; s < 5; s++)
			{
				for (int ss = 0; ss < 5; ss++)
				{
					float a = helper::grid[s][ss][0];
					float b = helper::grid[s][ss][1];
					Eigen::Vector3f randomLightPoint = p1 + (a * (p4 - p1)) + (b * (p2 - p1));
					generatedP = randomLightPoint;

					Eigen::Vector3f l = (generatedP - point).normalized();


					//float p = 1.0 / (2.0 * helper::pi);
					Eigen::Vector3f emittance = g->dc * g->kd;

					Eigen::Vector3f diffuse = vecMult(calcDiffuse(normal, l, g), g->dc);


					Eigen::Vector3f bounceCol = recursiveBounce(sampleDir, point, g, point, 0, objects, g, normal);

					if (shadowTest(point, normal, objects, g))
					{
						diffuse = Eigen::Vector3f(0, 0, 0);
						shadowCount++;
					}
					lightContrib += diffuse + vecMult(bounceCol, emittance);
					if (shadowCount >= 10)
					{
						stuckInShadow = true;
						break;
					}
					
				}
				if (stuckInShadow)
					break;
			}
			lightContrib /= (5 * 5);
			return colorOverflow(lightContrib);
		}
		else
		{
			generatedP = center;
			

			Eigen::Vector3f l = (generatedP - point).normalized();
			Eigen::Vector3f v = (cam.position - point).normalized(); //wo
			
			Eigen::Vector3f specular = vecMult(calcSpecular(normal, l, v, g), g->sc);
			Eigen::Vector3f diffuse = vecMult(calcDiffuse(normal, l, g),g->dc);
			//float p = 1.0 / (2.0 * helper::pi);
			if (shadowTest(point, normal, objects, g))
			{
				specular = Eigen::Vector3f(0, 0, 0);
				diffuse = Eigen::Vector3f(0, 0, 0);
			}

			Eigen::Vector3f bounceCol = recursiveBounce(sampleDir, point, g, point, 0, objects, g, normal);
			
		


			


			return colorOverflow((diffuse+specular)+vecMult(bounceCol,g->dc*g->kd));
		}
	}
	


	if (type == "point" || usecenter)
	{
		generatedP = center;
		if (shadowTest(point, normal, objects, g))
			return Eigen::Vector3f(0, 0, 0);



		//If the pixel shouldn't be shadowed, give it the appropriate color


		//the direction of the light source is (theta(i), phi(i)) : point - center
		//the direction that the light is being reflected is (theta(r), phi(r)) : cam.position - point

		//the Irradiance is E(theta(i), phi(i))
		//the Radiance is L(theta(r), phi(r))

		//BRDF is therefore the ratio
		/*
			f(theta(i), phi(i), theta(r), phi(r)) = L(theta(r), phi(r)) / (theta(i), phi(i))
		*/

		//Vector from the point to the light source
		Eigen::Vector3f l = (center - point).normalized(); //wi

		//Vector from the point to the camera
		Eigen::Vector3f v = (cam.position - point).normalized(); //wo

		//BRDF(L,V)
		//Albedo is between 0 and 1
		/*

		L = (Albedo / pi) * E
		E = (J / (dist(point-center) * dist(center - point)) ) * (normal.dot(center - point)))

		L = (Albedo / pi) * ( (J / (dist(point-center) * dist(center - point)) ) * (normal.dot(center - point))) )

		J is a constant

		*/


		//BRDF
		Eigen::Vector3f diffuse = vecMult(calcDiffuse(normal, l, g), g->dc);
		Eigen::Vector3f specular = vecMult(calcSpecular(normal, l, v, g), g->sc);

	


		//R, G, and B need to be between 0 and 1
		return colorOverflow(diffuse+specular);
	}
	else
	{
		Eigen::Vector3f lightContrib(0, 0, 0);
		Eigen::Vector3f v = (cam.position - point).normalized(); //wo
		Eigen::Vector3f l;
		Eigen::Vector3f randomLightPoint;
		//Vector from the point to the camera


		Eigen::Vector3f diffuse;
		Eigen::Vector3f specular;
		for (int s = 0; s < helper::raysperpixel[0]; s++)
		{
			for (int ss = 0; ss < helper::raysperpixel[0]; ss++)
			{
				float a = helper::grid[s][ss][0];
				float b = helper::grid[s][ss][1];
				randomLightPoint = p1 + (a * (p4 - p1)) + (b * (p2 - p1));
				generatedP = randomLightPoint;


				/*Shadows*/
				if (shadowTest(point, normal, objects, g))
					continue;



				l = (generatedP - point).normalized(); 




				diffuse = vecMult(calcDiffuse(normal, l, g),g->dc);
				specular = vecMult(calcSpecular(normal, l, v, g),g->sc);

				lightContrib += (diffuse + specular);
			}
			
		}

		lightContrib /= (helper::raysperpixel[0] * helper::raysperpixel[0]);


		return colorOverflow(lightContrib);

	}
	 


}
Eigen::Vector3f Light::calcDiffuse(Eigen::Vector3f normal, Eigen::Vector3f l,Geometry* g)
{


	/*

	L = k * intensity * max(0, n.dot(l))

	where L is the color, k is the diffuse coefficient (kd), I is the intensity of the light
	n is the normal vector, l is the vector from the point to the light source

	diffuse = g->kd * id * max(0, normal.dot(v));

	*/

	//Diffusion intensity - cos(theta)
	double f = normal.dot(l);


	//Lambertian Shading for diffused light
	return g->kd * id * max(0.0, f);
}
Eigen::Vector3f Light::calcSpecular(Eigen::Vector3f normal, Eigen::Vector3f l, Eigen::Vector3f v, Geometry* g)
{
	//Half-vector between the light source vector and the camera vector
	//Its dot product with the Normal will yield the intensity of the specular lighting
	Eigen::Vector3f h = (v + l).normalized();


	float halfDot = normal.dot(h);


	//Intensity to the power of the Phong
	float m = pow(halfDot, g->pc);


	/*

		L = ks * I * max(0, n.dot(h)^p)

		where L is the color, ks is the specular coefficient, I is the intensity of the light
		n is the normal vector, h is the half vector and p is the phong

		specular = g->ks * is * m

	*/

	return g->ks * is * m;
}



bool Light::shadowTest(Eigen::Vector3f point, Eigen::Vector3f normal, vector<Geometry*> g, Geometry* originalObj)
{
	Eigen::Vector3f direction = (generatedP - point);
	if (normal.dot(direction.normalized()) < 0)
		return true;


	Ray lightRay(Eigen::Vector3d(point[0],point[1],point[2]), Eigen::Vector3d(direction[0], direction[1], direction[2]).normalized());
	HitContainer* originObj = new HitContainer;
	originObj->g = originalObj;
	originObj->hitPixel = Eigen::Vector4d(point[0], point[1], point[2], 1);
	originObj->lightSource = Eigen::Vector4d(generatedP[0], generatedP[1], generatedP[2], 1);


	float lRayLength = dist(generatedP - point);
	HitContainer h = loopForHits(lightRay,g,true,originObj);
	delete originObj;

	if (h.g == NULL)
		return false;
	else
	{
		Eigen::Vector3f thePixel = Eigen::Vector3f(h.hitPixel[0], h.hitPixel[1], h.hitPixel[2]);

		if (dist(generatedP - thePixel) < lRayLength &&
			dist(point - thePixel) < lRayLength
			)
			return true;
	}
	return false;

	
}
Eigen::Vector3f Light::recursiveBounce(Eigen::Vector3f sampleDir, Eigen::Vector3f point, Geometry* origObject, Eigen::Vector3f origin, int count, vector<Geometry*> g, Geometry* hitObject, Eigen::Vector3f objectNormal)
{


	if ((realUniRand(0.0,1.0)) <= helper::probterminate || count == helper::max_bounce)
	{
		Eigen::Vector3f diffuse(0, 0, 0);
		if (shadowTest(origin, objectNormal, g, hitObject))
			return diffuse;
		
		if (count > 0)
		{
			Eigen::Vector3f l = (generatedP - origin).normalized();

			diffuse = id * max(0.0f, objectNormal.dot(l));
		}
		return diffuse;
	}
	
		


	Ray bounceRay(Eigen::Vector3d(origin[0],origin[1],origin[2]), Eigen::Vector3d(sampleDir[0], sampleDir[1], sampleDir[2]));


	HitContainer* originObj = new HitContainer;
	originObj->g = hitObject;
	originObj->hitPixel = Eigen::Vector4d(origin[0], origin[1], origin[2], 1);
	
	HitContainer hit = loopForHits(bounceRay,g,false,originObj);
	delete originObj;
	if (hit.g != NULL)
	{
		int closest = 0;

		Geometry* obj = hit.g;
		Eigen::Vector3f thePixel = Eigen::Vector3f(hit.hitPixel[0], hit.hitPixel[1], hit.hitPixel[2]);
		Eigen::Vector3f normal = obj->getNormal(thePixel);

		Eigen::Vector3f A = randomHemispherePoint();

		Eigen::Vector3f ranDir;
		float d;
		do
		{
			ranDir = (Eigen::Vector3f(thePixel[0] + uniRand(1, 10), thePixel[1] + uniRand(1, 10), thePixel[2] + uniRand(1, 10)) - thePixel).normalized();
			d = normal.dot(ranDir);
		} while (d <= 1 && d > 0.85);
		Eigen::Vector3f x = ranDir.cross(normal).normalized();
		Eigen::Vector3f z = x.cross(normal).normalized();

		Eigen::Matrix3f r;


		r(0, 0) = x[0];
		r(0, 1) = x[1];
		r(0, 2) = x[2];

		r(1, 0) = normal[0];
		r(1, 1) = normal[1];
		r(1, 2) = normal[2];

		r(2, 0) = z[0];
		r(2, 1) = z[1];
		r(2, 2) = z[2];

		Eigen::Vector3f B = (r.inverse() * A) + thePixel;

		Eigen::Vector3f nextDir = (B - thePixel).normalized();

		float cosTheta = nextDir.dot(normal);
		Eigen::Vector3f emittance = obj->dc * obj->kd;


		
		Eigen::Vector3f bounceCol = recursiveBounce(nextDir, point, origObject, thePixel, count + 1, g, obj, normal);


		return vecMult(emittance,bounceCol);
	}
	else
	{
		Eigen::Vector3f diffuse(0, 0, 0);
		if (shadowTest(origin, objectNormal, g, hitObject))
			return diffuse;

		if (count > 0)
		{
			Eigen::Vector3f l = (generatedP - origin).normalized();

			diffuse = id * max(0.0f, objectNormal.dot(l));
		}
		return diffuse;
	}

	

	
}

