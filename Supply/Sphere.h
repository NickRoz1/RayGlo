//---------------------------------------------------------------------------

#ifndef SphereH
#define SphereH
//---------------------------------------------------------------------------

#include "Vector3D.h"
#include "Math.h"

enum MaterialTypeEnum{
	DIFFUSIVE,
	REFLECTIVE,
	GLASS
};

class Sphere{
	public:
		Sphere(Vector3D center, double radius, MaterialTypeEnum material, Vector3D emitance, Vector3D color) :
			center(center),
			radius(radius),
			material(material),
			emitance(emitance),
			color(color) {};
		MaterialTypeEnum material;
		Vector3D emitance;
		Vector3D center;
		Vector3D color;
		double radius;
		bool intersection_check(const Ray &ray, double &t)
		{
			//(OC+Dt)^2-R^2=0
			Vector3D OC = ray.origin - center;
			double b = 2 * (ray.direction * OC);
			double c = OC * OC - radius * radius;
			double disc = b * b - 4 * c; // D нормализован
			if(disc < 0) return false;

			double squareRoot = sqrt(disc);
			double t0 = ((-b+squareRoot)/2);
			double t1 = ((-b-squareRoot)/2);
			t = (t0 < t1 ? t0 : t1);
			return true;

//			Vector3D tempCenter(-center.x, -center.y, -center.z);// Вектор идёт от центра к началу
//			double b = 2*(dir * tempCenter);  // dir нормализован
//			double c = tempCenter * tempCenter - radius*radius; // Возможно ненужная операция, оба значения - константы
//			double disc = b*b - 4*c;
//			if(disc<0) return false;
//			double squareRoot = sqrt(disc);
//			double t0 = ((-b+squareRoot)/2);
//			double t1 = ((-b-squareRoot)/2);
//			t = (t0 < t1 ? t0 : t1);
//			return true;
		}
};

#endif
