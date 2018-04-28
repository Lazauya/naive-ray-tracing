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
	cin >> x; //5.5
	cin >> y; //8
	cin >> z; //6.5
	std::string inp;
	UInt64 sph = 100;
	UInt64 res = 16;
	//cin >> inp;
	//cin >> sph;
	//cin >> res;
	//perspectiveViewWindow * v = new perspectiveViewWindow(100, point(0,-100,0), 0, 0, resolution(res,res), dimensions(100, 100), 1);
	perspectiveViewWindow * v = new perspectiveViewWindow(9, point(x, y, z), 0, 90, resolution(512,512), dimensions(5, 5), 3);
	//orthographicViewWindow * v = new orthographicViewWindow(point(0,0,0), 0, 0, resolution(1024,1024), dimensions(14.5,14.5), 5);
	object * s = new sphere(point(0,3,0), 3, rgbColor(1,0,0, true), 100, rgbColor(1,1,1, true));
	object * t = new triangle(point(-10,7,3), point(8,7,6), point(10,7,2), rgbColor(1,.1,.1, true), 1, rgbColor(1,1,1, true));
	object * p = new plane(vec3d(0,-1,0), point(0, 8, 0), rgbColor(0, .847, 1, true), 10, rgbColor(1,1,1, true));
	light * d = new directionalLight(vec3d(1,1,1).unit(), rgbColor(.25,.25,.25, true), 1);
	light * pl = new pointLight(point(3,0,-4), rgbColor(.25,.25,.25, true), 1);		
	auto dist = std::uniform_real_distribution<F64>(-100,100);
	auto dist_2 = std::uniform_real_distribution<F64>(0,1);
	auto gen = std::default_random_engine();

	basicScene<phong> myScene;
	BVHScene<phong> mySecondScene;
	myScene.window = v;
	//myScene.addObj(s);
	//myScene.addObj(s2);
	//myScene.addObj(t);
	//myScene.addObj(p);
	myScene.addLight(d);
	myScene.addLight(pl);	
	myScene.ambience = ambientLight(rgbColor(.2,.2,.2,true), rgbColor(.3,.3,.7, true), 1);
		
	mySecondScene.window = v;
	//mySecondScene.addObj(s);
	//mySecondScene.addObj(s2);
	//mySecondScene.addObj(t);
	//mySecondScene.addObj(p);
	mySecondScene.addLight(d);
	mySecondScene.addLight(pl);	
	mySecondScene.ambience = ambientLight(rgbColor(.2,.2,.2,true), rgbColor(.3,.3,.7, true), 1);	
	/*for(int i = 0; i < sph; i++)
	{
		sphere * myS = new sphere(point(dist(gen), dist(gen), dist(gen)), 3, rgbColor(dist_2(gen), dist_2(gen), dist_2(gen), true), 100, rgbColor(1,1,1,true));
		myScene.addObj(myS);
		mySecondScene.addObj(myS);
	}*/
	
	object * myMesh = new mesh("teapot.obj", rgbColor(1,0,0, true), 100, rgbColor(1,1,1, true)); 
	mySecondScene.addObj(myMesh);
	//if(inp == "bvh")
	//{
		std::cout << "BVH" << std::endl;
		image<rgb_pixel> * mySecondIm = mySecondScene.render(); mySecondIm->write("renderTest.png");
	//}
	//else
	//{
	//std::cout << "############################################################" << std::endl;	
	//std::cout << "NON BVH" << std::endl;
	//image<rgb_pixel> * myIm = myScene.render(); myIm->write("render.png");
	//}
	/*image<rgb_pixel> * myIm = myScene.render();
	std::cout << "############################################################" << std::endl;	
	image<rgb_pixel> * mySecondIm = mySecondScene.render();*/
	//myIm->write("render.png");
	//mySecondIm->write("renderTest.png");
}
