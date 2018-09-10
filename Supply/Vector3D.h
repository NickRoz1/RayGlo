//---------------------------------------------------------------------------

#ifndef Vector3DH
#define Vector3DH
//---------------------------------------------------------------------------

#include "Math.h"

//---------------------------------------------------------------------------

class Vector3D{
	public:
		double x,y,z;
		Vector3D() : x(0), y(0), z(0) {};
		Vector3D(double xi, double yi, double zi) : x(xi), y(yi), z(zi) {};
	 	Vector3D(const Vector3D &vector) : x(vector.x), y(vector.y), z(vector.z) {};
		Vector3D &normalize();
		Vector3D multiply(Vector3D b){        // ПЕРЕДЕЛАТЬ
			return Vector3D(x*b.x, y*b.y, z*b.z);
		}
};

Vector3D& Vector3D::normalize(){
	double length = sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
	this->x/=length;
	this->y/=length;
	this->z/=length;
	return *this;
}

Vector3D operator + (const Vector3D &a, const Vector3D &b){
	return Vector3D(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector3D& operator +=(Vector3D &a, const Vector3D &b){
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	return a;
}

Vector3D operator - (const Vector3D &a, const Vector3D &b){
	return Vector3D(a.x - b.x, a.y - b.y, a.z - b.z);
}

double operator * (const Vector3D &a, const Vector3D &b){
	return (a.x)*(b.x) + (a.y)*(b.y) + (a.z)*(b.z);
}

Vector3D& operator *= (const double &b, Vector3D &a){  //Возможно нелегитимный приём.
	a.x *= b;
	a.y *= b;
	a.z *= b;
	return a;
}

Vector3D& operator *= (Vector3D &a, const double &b){
	a.x *= b;
	a.y *= b;
	a.z *= b;
	return a;
}

Vector3D operator * (const Vector3D &a, const double &b){
	return Vector3D(a.x*b, a.y*b, a.z*b);
}

Vector3D operator / (const Vector3D &a, const double &b){
	return Vector3D(a.x/b, a.y/b, a.z/b);
}


Vector3D operator % (const Vector3D &a, const Vector3D &b){
    return Vector3D(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

struct Ray{
	Ray(Vector3D origin, Vector3D direction) : origin(origin), direction(direction){}
	Vector3D origin;
	Vector3D direction;
};

#endif

