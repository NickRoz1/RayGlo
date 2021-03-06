//---------------------------------------------------------------------------

#ifndef LightH
#define LightH
//---------------------------------------------------------------------------

#include <cstdlib>
#include <iostream.h>

#include "Vector3D.h"


//---------------------------------------------------------------------------

enum LightTypeEnum{
	PointLight,
	DirectionalLight
};

struct Color{
	Color(int R, int G, int B){
		if( R < 0 || R > 255){
			std::cout << "Error! Component of color cant be more than 255 or less than 0\n";
			abort();
		}
		else if( G < 0 || G > 255){
			std::cout << "Error! Component of color cant be more than 255 or less than 0\n";
			abort();
		}
		else if(B < 0 || B > 255){
			std::cout << "Error! Component of color cant be more than 255 or less than 0\n";
			abort();
		}
		else{
			this->R = R;
			this->G = G;
			this->B = B;
		}
	}
	int R;
	int G;
	int B;
};


class Light{
	public:
		Light(LightTypeEnum lightType, Vector3D lightDirection,
			  Vector3D lightOrigin, Color lightColor) :
			  lightType(lightType), lightDirection(lightDirection),
			  lightOrigin(lightOrigin), lightColor(lightColor) {};

		LightTypeEnum lightType;
		Vector3D lightDirection;
		Vector3D lightOrigin;
		Color lightColor;
};

//double lightning(Vector3D hitPoint){
//	Vector3D dir = hitPoint - lightPoint;  //�������� �������� ��� ��������� ����� ����?..
//
//	Vector3D normalToSphere = hitPoint - NANI.center;
//	normalToSphere.normalize();
//	dir.normalize();
//	double a[2] = {normalToSphere * normalToSphere, dir * dir};
//	double angle = dir * normalToSphere;
//	if(angle>0) {
//		if(angle>1) cout << "������ ����������������������������";
//		cout << "angle > 0\n";
//		return 0;
//	}
//	return -angle;
//}

#endif
