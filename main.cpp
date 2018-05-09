#include "vec3d.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <iterator>
#include <png++/png.hpp>

#include "object.h"
#include "lights.h"
#include "view.h"
#include "shaders.h"
#include "color.h"
#include "scene.h"

#include "mesh.h"

using namespace std;
using namespace v3d;
using namespace png;
using namespace lights;
using namespace scene;
using namespace objects;
using namespace view;
using namespace shaders;

inline void printPoint(point p)
{
	cout << "(" << p.x << ", " << p.y << ", " << p.z << ")";
}

inline void printVector(vec3d v)
{
	cout << "<" << v.x << ", " << v.y << ", " << v.z << ">";
}

int main()
{
	double x, y, z;
	//cin >> x; //0
	//cin >> y; //-5
	//cin >> z; //2
	x = 0; y = -5; z = 0;
	std::string inp;
	UInt64 sph = 100;
	UInt64 res = 16;
	//cin >> inp;
	//cin >> sph;
	//cin >> res;
	//perspectiveViewWindow * v = new perspectiveViewWindow(100, point(0,-100,0), 0, 0, resolution(res,res), dimensions(100, 100), 1);
	perspectiveViewWindow * v = new perspectiveViewWindow(9, point(x, y, z), 0, 0, resolution(1024,1024), dimensions(5, 5), 1);
	//orthographicViewWindow * v = new orthographicViewWindow(point(0,0,0), 0, 0, resolution(1024,1024), dimensions(14.5,14.5), 1);
	object * s = new sphere(point(0,3,0), 3, rgbColor(1,0,0, true), 100, rgbColor(1,1,1, true));
	object * t = new triangle(point(-6,7,0), point(8,4,4), point(5,7,-.5), rgbColor(1,.1,.1, true), 1, rgbColor(1,1,1, true), rgbColor(1,1,1,true), 0, 0, 1); //1.458
	object * p = new plane(vec3d(0,0,1), point(0, 0, -3), rgbColor(0, .847, 1, true), 10, rgbColor(1,1,1, true));
	light * d = new directionalLight(vec3d(1,1,-1).unit(), rgbColor(.25,.25,.25, true), 1);
	light * pl = new pointLight(point(3,0,4), rgbColor(.25,.25,.25, true), 1);		
	
	auto dist = std::uniform_real_distribution<F64>(-100,100);
	auto dist_2 = std::uniform_real_distribution<F64>(0,1);
	auto gen = std::default_random_engine();

	basicScene<phong> myScene;
	BVHScene<fresnel> mySecondScene;
	myScene.window = v;	
	
	mySecondScene.window = v;
	//mySecondScene.addObj(s);
	//mySecondScene.addObj(s2);
	mySecondScene.addObj(t);
	mySecondScene.addObj(p);
	mySecondScene.addLight(d);
	mySecondScene.addLight(pl);	
	mySecondScene.ambience = ambientLight(rgbColor(.2,.2,.2,true), rgbColor(.3,.3,.7, true), 1);	

	object * myMesh = new mesh("meshs/cube.obj", rgbColor(1,0,0, true), 100, rgbColor(1,1,1, true), rgbColor(1,1,1,true), 1, 0, 1.1); 
	((mesh*)myMesh)->rotate(point(0,0,0), vec3d(1,0,0), PI/4);
	((mesh*)myMesh)->rotate(point(0,0,0), vec3d(0,0,1), PI/4);
	((mesh*)myMesh)->translate(-2,0,0);
	((mesh*)myMesh)->scale(point(0,0,0), 1);
	mySecondScene.addObj(myMesh);
	
	object * mySMesh = new mesh("meshs/cube.obj", rgbColor(1,0,0, true), 100, rgbColor(1,1,1, true), rgbColor(1,1,1,true), 1, 0, 1.1); 
	((mesh*)mySMesh)->rotate(point(0,0,0), vec3d(1,0,0), PI/4);
	((mesh*)mySMesh)->rotate(point(0,0,0), vec3d(0,0,1), PI/4);
	((mesh*)mySMesh)->translate(2,0,0);
	((mesh*)mySMesh)->scale(point(0,0,0), 1);
	mySecondScene.addObj(mySMesh);

	std::cout << "BVH" << std::endl;
	image<rgb_pixel> * mySecondIm = mySecondScene.render(); mySecondIm->write("renderBox.png");
	
}
