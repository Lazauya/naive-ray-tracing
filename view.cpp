#include "view.h"
#include <iostream>

inline void pv(vec3d v) { std::cout << "<" << v.x << ", " << v.y << ", " << v.z << ">"; }


namespace view
{
	perspectiveViewWindow::perspectiveViewWindow(point start, point middle, F64 angleT, F64 angleP, resolution _res, dimensions _dim, UInt64 subs) : 
	viewWindow(middle, angleT, angleP, _res, _dim, subs > 0 ? subs : 1),
	startPoint(start)
	{
		dist = std::uniform_real_distribution<F64>(-dim.first/(res.first*subdivisions), dim.second/(res.second*subdivisions));

		vectors = new vec3d*[res.first*subdivisions];
		for (UInt64 i = 0; i < res.first*subdivisions; i++)
		{
			vectors[i] = new vec3d[res.second*subdivisions];
		}
		
		vec3d vertDir(cos(angleTheta)*cos(anglePhi), sin(angleTheta)*cos(anglePhi), sin(anglePhi));
		vec3d horizDir(cos(angleTheta+PI/2)*cos(anglePhi+PI/2), sin(angleTheta+PI/2)*cos(anglePhi+PI/2), sin(anglePhi+PI/2)); //same vec rotated 90 degs
		
		for (UInt64 t = 0; t < res.first*subdivisions; t++)
		{
			for (UInt64 s = 0; s < res.second*subdivisions; s++)
			{
				F64 subPixelVert = (F64)(t) - (res.first*subdivisions)/2;
				F64 subPixelHoriz = (F64)(s) - (res.second*subdivisions)/2;

				subPixelVert *= (dim.first/2)/(res.first*subdivisions/2);
				subPixelHoriz *= (dim.second/2)/(res.second*subdivisions/2);
				
				subPixelVert += dist(gen);
				subPixelHoriz += dist(gen);
				
				vec3d endPoint = vec3d(windowMiddle) + subPixelVert*vertDir + subPixelHoriz*horizDir;
				//pv(endPoint); std::cout << std::endl;
				vectors[t][s] = (endPoint-startPoint).unit();
				//pv(vectors[t][s]); std::cout << std::endl;
			}
		}
	}
	
	perspectiveViewWindow::perspectiveViewWindow(F64 distBehind, point middle, F64 angleT, F64 angleP, resolution _res, dimensions _dim, UInt64 subs) : 
	viewWindow(middle, angleT, angleP, _res, _dim, subs > 0 ? subs : 1)
	{
		dist = std::uniform_real_distribution<F64>(-dim.first/(res.first*subdivisions), dim.second/(res.second*subdivisions));

		vectors = new vec3d*[res.first*subdivisions];
		for (UInt64 i = 0; i < res.first*subdivisions; i++)
		{
			vectors[i] = new vec3d[res.second*subdivisions];
		}
		
		vec3d vertDir(cos(angleTheta)*cos(anglePhi), sin(angleTheta)*cos(anglePhi), sin(anglePhi));
		vec3d horizDir(cos(angleTheta+PI/2)*cos(anglePhi+PI/2), sin(angleTheta+PI/2)*cos(anglePhi+PI/2), sin(anglePhi+PI/2)); //same vec rotated 90 degs
		
		startPoint = (vec3d)middle+vertDir.cross(horizDir)*distBehind;

		for (UInt64 t = 0; t < res.first*subdivisions; t++)
		{
			for (UInt64 s = 0; s < res.second*subdivisions; s++)
			{
				F64 subPixelVert = (F64)(t) - (res.first*subdivisions)/2;
				F64 subPixelHoriz = (F64)(s) - (res.second*subdivisions)/2;

				subPixelVert *= (dim.first/2)/(res.first*subdivisions/2);
				subPixelHoriz *= (dim.second/2)/(res.second*subdivisions/2);
				
				subPixelVert += dist(gen);
				subPixelHoriz += dist(gen);
				
				vec3d endPoint = vec3d(windowMiddle) + subPixelVert*vertDir + subPixelHoriz*horizDir;
				//pv(endPoint); std::cout << std::endl;
				vectors[t][s] = (endPoint-startPoint).unit();
				//pv(vectors[t][s]); std::cout << std::endl;
			}
		}
	}

	perspectiveViewWindow::~perspectiveViewWindow()
	{
		
		for (UInt64 i = 0; i < res.first*subdivisions; i++)
		{
			delete[] vectors[i];
		}
		delete[] vectors;
	}

	ray3d *** perspectiveViewWindow::rays()
	{
		ray3d *** rays = new ray3d**[res.first];
		
		for (UInt64 i = 0; i < res.first; i++)
		{
			rays[i] = new ray3d*[res.second];
			for(UInt64 j = 0; j < res.second; j++)
			{
				rays[i][j] = new ray3d[subdivisions*subdivisions];
			}
		}

		for (UInt64 t = 0; t < res.first; t++)
		{
			for (UInt64 s = 0; s < res.second; s++)
			{
				for (UInt64 sub = 0; sub < subdivisions*subdivisions; sub++)
				{
					rays[t][s][sub] = ray3d(startPoint, vectors[t*subdivisions+sub/subdivisions][s*subdivisions+sub%subdivisions]);
				}
			}
		}
		return rays;
	}

	orthographicViewWindow::orthographicViewWindow(point middle, F64 angleT, F64 angleP, resolution _res, dimensions _dim, UInt64 subs) : 
       	viewWindow(middle, angleT, angleP, _res, _dim, subs > 0 ? subs : 1)
	{
		dist = std::uniform_real_distribution<F64>(-dim.first/(res.first*subdivisions), dim.second/(res.second*subdivisions));
		
		startPoints = new point*[res.first*subdivisions];
		for (UInt64 i = 0; i < res.first*subdivisions; i++)
		{
			startPoints[i] = new point[res.second*subdivisions];
		}
		
		//vec3d vertDir(cos(angleTheta)*cos(anglePhi), sin(angleTheta)*cos(anglePhi), sin(anglePhi));
		//vec3d horizDir(cos(angleTheta+PI/2)*cos(anglePhi+PI/2), sin(angleTheta+PI/2)*cos(anglePhi+PI/2), sin(anglePhi+PI/2)); //same vec rotated 90 degs
		
		vec3d vertDir(cos(angleTheta)*cos(anglePhi), sin(angleTheta)*cos(anglePhi), sin(anglePhi));
		vec3d horizDir(cos(angleTheta+PI/2)*cos(anglePhi+PI/2), sin(angleTheta+PI/2)*cos(anglePhi+PI/2), sin(anglePhi+PI/2)); //same vec rotated 90 degs

		direction = -1*vertDir.cross(horizDir).unit();

		v3d::pv(direction);

		for (UInt64 t = 0; t < res.first*subdivisions; t++)
		{
			for (UInt64 s = 0; s < res.second*subdivisions; s++)
			{
				F64 subPixelVert = (F64)(t) - (res.first*subdivisions)/2;
				F64 subPixelHoriz = (F64)(s) - (res.second*subdivisions)/2;
				subPixelVert *= (dim.first/2)/(res.first*subdivisions/2);
				subPixelHoriz *= (dim.second/2)/(res.second*subdivisions/2);
				
				subPixelVert += dist(gen);
				subPixelHoriz += dist(gen);

				vec3d startPoint = vec3d(windowMiddle) + subPixelVert*vertDir + subPixelHoriz*horizDir;
				startPoints[t][s] = (point)startPoint;
			}
		}
	}

	orthographicViewWindow::~orthographicViewWindow()
	{
		
		for (UInt64 i = 0; i < res.first*subdivisions; i++)
		{
			delete[] startPoints[i];
		}
		delete[] startPoints;
	}

	ray3d *** orthographicViewWindow::rays()
	{
		ray3d *** rays = new ray3d**[res.first];
		
		for (UInt64 i = 0; i < res.first; i++)
		{
			rays[i] = new ray3d*[res.second];
			for(UInt64 j = 0; j < res.second; j++)
			{
				rays[i][j] = new ray3d[subdivisions*subdivisions];
			}
		}

		for (UInt64 t = 0; t < res.first; t++)
		{
			for (UInt64 s = 0; s < res.second; s++)
			{
				for (UInt64 sub = 0; sub < subdivisions*subdivisions; sub++)
				{
					rays[t][s][sub] = ray3d(startPoints[t*subdivisions+sub/subdivisions][s*subdivisions+sub%subdivisions], direction);
					//pr(rays[t][s][sub]); std::cout << std::endl;
				}
			}
		}
		return rays;
	}
}
