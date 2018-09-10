#include <stdlib.h>
#include <cstdlib.h>
#include <stdio.h>
#include <math.h>
#include <fstream.h>
#include <iostream.h>
#include <vector>

#include <Vector3D.h>
#include <Sphere.h>
#include <Light.h>

const int SCREEN_HEIGHT = 255;
const int SCREEN_WIDTH = 255;
const int SAMPLES = 10000;
const Vector3D lightPoint(0,0,10);

Sphere Spheres[] = {
	Sphere(Vector3D(0,0,-150), 35., DIFFUSIVE, Vector3D(0,0,0), Vector3D(20,90,180)),
	Sphere(Vector3D(10e5,0,50), 10e5-1070, DIFFUSIVE, Vector3D(0,0,0), Vector3D(250,70,150)),
	Sphere(Vector3D(-10e5,0,50), 10e5-1050, DIFFUSIVE, Vector3D(0,0,0), Vector3D(250,70,150)),
	Sphere(Vector3D(0,0,10e5), 10e5-500, DIFFUSIVE, Vector3D(0,0,0), Vector3D(0,70,150)),
	Sphere(Vector3D(0,10e5,0), 10e5-500, DIFFUSIVE, Vector3D(0,0,0), Vector3D(250,0,150)),
	Sphere(Vector3D(0,-10e5,0), 10e5-500, DIFFUSIVE, Vector3D(0,0,0), Vector3D(250,70,0)),
	Sphere(Vector3D(0,-20,0), 5., DIFFUSIVE, Vector3D(255,255,255), Vector3D())
};

bool intersect(Ray ray, double &t, int &numberOfSphere){
	int sphereQuantity = sizeof(Spheres)/sizeof(Sphere);
	double O = 10e5;
	double eps = 10e-4;
	t = O;
	for(int i = 0; i < sphereQuantity; i++){
		double temp;
		if(Spheres[i].intersection_check(ray,temp) && temp < t && temp > eps){
			t = temp;
			numberOfSphere = i;
		}
	}
	return t < (O - 1) ? true : false;
}

int clamp( double sample ){
	if(sample>255) return 255;
	if(sample<0) return 0;
	else return int( sample + 0.5 );
}

Vector3D rayTrace(Ray iRay, int depth, int E){
	int numberOfSphere;
	double t;
	if(!intersect(iRay, t, numberOfSphere)) return Vector3D();
	Sphere object = Spheres[numberOfSphere];
	if(depth>5) return object.emitance*E;
	Vector3D pointOnSurface = iRay.direction*t;

	Vector3D resRadiance;

	if(object.material == DIFFUSIVE){
		Vector3D currentColor = object.color;

		double firstRandParameter = 2*M_PI*double(rand())/RAND_MAX;
		double secondRandParameter = double(rand())/RAND_MAX;
		double thirdRandParameter = sqrt(secondRandParameter);
		
		Vector3D normalAtPoint = (pointOnSurface - object.center).normalize();
		Vector3D localBasis1 = (normalAtPoint%Vector3D(1,0,0)).normalize();
		Vector3D localBasis2 = normalAtPoint%localBasis1;

		Vector3D dirForNext = localBasis1*std::cos(firstRandParameter)*thirdRandParameter +
							  localBasis2*std::sin(firstRandParameter)*thirdRandParameter +
							  normalAtPoint*sqrt(1-secondRandParameter);

		dirForNext.normalize();
		

		
		for(int i = sizeof(Spheres)/sizeof(Sphere)-1; i >= 0; i--){
			if(Spheres[i].emitance.x == 0 &&
			   Spheres[i].emitance.y == 0 &&
			   Spheres[i].emitance.z == 0) continue;

			   double firstRandParameter = double(rand())/RAND_MAX;
			   double secondRandParameter = double(rand())/RAND_MAX;
			   
			   Vector3D vectorLightPoint = (Spheres[i].center - pointOnSurface);//ÍÅ ÍÎÐÌÀËÈÇÎÂÀÍ
			   Vector3D localBasis1 = (fabs(vectorLightPoint.x)>0.1 ?
									   Vector3D(1,0,0) : Vector3D(0,1,0))%vectorLightPoint;
			   localBasis1.normalize();
			   Vector3D localBasis2 = vectorLightPoint%localBasis1;
				//double check = 1-(Spheres[i].radius*Spheres[i].radius)/(vectorLightPoint*vectorLightPoint+1E-5);

			   double cos_max = sqrt(1-(Spheres[i].radius*Spheres[i].radius)/
									(vectorLightPoint*vectorLightPoint+1E-5));
                // Äëÿ ñëó÷àÿ, êîãäà òî÷êà íà ñâåòèëüíèêå. Èíà÷å ìîæåò áûòü ìåíüøå íóëÿ ïîä êîðíåì
			   double cos = 1 - firstRandParameter + cos_max*firstRandParameter;
			   double sin = sqrt(1 - cos*cos);
			   double eps = 2*M_PI*secondRandParameter;

			   vectorLightPoint.normalize();
			   Vector3D lightVector = localBasis1*(std::cos(eps))*sin +
									  localBasis2*(std::sin(eps))*sin +
									  vectorLightPoint*cos;
			   lightVector.normalize();
			   double angleLightNormal = lightVector*normalAtPoint;
			   intersect(Ray(pointOnSurface, lightVector), t, numberOfSphere);
			   if( t<10e5)
			   int b = 5;
			   if(intersect(Ray(pointOnSurface, lightVector), t, numberOfSphere) && numberOfSphere == i){
				   resRadiance += object.emitance +
				   currentColor.multiply(Spheres[i].emitance*angleLightNormal)*M_1_PI;
			   }

		}
		return object.emitance*E + resRadiance + rayTrace(Ray(pointOnSurface, dirForNext), ++depth, 0) ;
	}
	return Vector3D();
}

int main() {
	std::srand(time(NULL));
	Ray cam(Vector3D(),Vector3D(0,0,-1).normalize());
	Vector3D cameraX(double(SCREEN_WIDTH)/SCREEN_HEIGHT,0,0); //Îòíîøåíèå øèðèíà ê âûñîòå ýêðàíà
	Vector3D cameraY((cam.direction % cameraX).normalize()*-1); //Y îò ìèíóñà ê ïëþñó, ñâåðõó âíèç
	Vector3D *output = new Vector3D[SCREEN_HEIGHT*SCREEN_WIDTH];
	// Íåòó ìàòðèöû ïåðåõîäà, íåîáõîäèìî çàäàòü x

	for(int i=0; i < SCREEN_HEIGHT; i++)
	{
		if(i%100==0) cout << "Process is on " << i << " pixel\n";
		for(int j=0; j<SCREEN_WIDTH; j++)
		{
			int pixelNumber = i*SCREEN_WIDTH+j;
			for(int subPixI = 0; subPixI < 2; subPixI++){
				for(int subPixJ = 0; subPixJ < 2; subPixJ++){
					Vector3D colorOfSubPixel;
					for(int sample = 0; sample < SAMPLES; sample++){
						double firstRandParameter = 2*double(rand())/RAND_MAX;
						double secondRandParameter = 2*double(rand())/RAND_MAX;
						double tempX = firstRandParameter < 1 ? sqrt(firstRandParameter) - 1 :
						1 - sqrt(2 - firstRandParameter);
						double tempY = secondRandParameter < 1 ? sqrt(secondRandParameter) - 1 :
						1 - sqrt(2 - secondRandParameter);
						//Ãåíåðèðóåì ñëó÷àéíûå çíà÷åíèÿ, òðàíñôîðìèðóåì èõ â äèàïîçîí îò -1 äî 1
						Vector3D dir = cameraX*(double((subPixJ + 0.5 + tempX)/2 + j)/SCREEN_WIDTH-0.5) +
									   cameraY*(double((subPixI + 0.5 + tempY)/2 + i)/SCREEN_HEIGHT-0.5) +
									   cam.direction;
						Vector3D check = cameraY*(double((subPixI + 0.5 + tempY)/2 + i)/SCREEN_HEIGHT-0.5);
						double checkInt = (double((subPixI + 0.5 + tempY)/2 + i)/SCREEN_HEIGHT-0.5);
						dir.normalize();  // Âîçìîæíî, íåïðàâèëüíîå îòíîøåíèå ðàçìåðà ýêðàíà ê ðàññòîÿíèþ äî íåãî
						if(i==123 && j ==123)
						int b = 3;
						Vector3D sampleRes = (rayTrace(Ray(cam.origin, dir),0,1) / double(SAMPLES));
						colorOfSubPixel += sampleRes;

					}
					output[pixelNumber]+=(colorOfSubPixel*=0.25);
				}
			}
		}
	}
	cout << "Printing in file\n";
	int imageArea = SCREEN_HEIGHT * SCREEN_WIDTH;
	ofstream fin("E:\\Ray_Tracer\\image1.ppm");
	fin << "P3 ";
	fin << SCREEN_WIDTH << " " << SCREEN_HEIGHT;
	fin << " 255 ";
	for(int i = 0; i<imageArea; i++){
		fin << clamp( output[i].x ) << " " << clamp( output[i].y ) << " " <<  clamp( output[i].z ) << " ";
	}
	fin.close();
    delete output;
	system("E:\\Ray_Tracer\\image1.ppm");
//	system("PAUSE");
}
