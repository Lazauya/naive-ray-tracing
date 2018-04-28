#ifndef BVH_H
#define BVH_H

#include "access.h"

#include <cmath>
#include <initializer_list>
#include <algorithm>
#include <vector>

using namespace v3d;

//namespace objects
//{
	//class object;
	//class triangle;
//}

namespace bvh_n
{

class bvh : public access_n::access
{
public:
	struct volume
	{
		volume(point p1, point p2, UInt64 no, objects::object ** o, UInt64 ind) : numObjs(no), index(ind)
		{
			points = new point[2];

			points[0] = point(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z)); 
			points[1] = point(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
			
			v = new point[8];
	       		v[0] = point(points[1].x, points[0].y, points[0].z);
			v[1] = point(points[0].x, points[0].y, points[0].z);
			v[2] = point(points[1].x, points[1].y, points[0].z);
			v[3] = point(points[0].x, points[1].y, points[0].z);
			v[4] = point(points[1].x, points[0].y, points[1].z);
			v[5] = point(points[0].x, points[0].y, points[1].z);
			v[6] = point(points[1].x, points[1].y, points[1].z);
			v[7] = point(points[0].x, points[1].y, points[1].z);

			obj = new objects::object*[numObjs];
			for(UInt64 i = 0; i < numObjs; i++)
			{
				obj[i] = o[i];
			}
			mid = (points[0]+points[1])*.5;
		}
		volume(volume v1, volume v2, UInt64 ind) : obj(nullptr), numObjs(0), index(ind)
		{
			points = new point[2];
			points[0] = point(std::max(v1.points[0].x, v2.points[0].x),
					std::max(v1.points[0].y, v2.points[0].y),
					std::max(v1.points[0].z, v2.points[0].z));
			points[1] = point(std::min(v1.points[1].x, v2.points[1].x),
					std::min(v1.points[1].y, v2.points[1].y),
					std::min(v1.points[1].z, v2.points[1].z));
			v = new point[8];
			v[0] = point(points[1].x, points[0].y, points[0].z);
			v[1] = point(points[0].x, points[0].y, points[0].z);
			v[2] = point(points[1].x, points[1].y, points[0].z);
			v[3] = point(points[0].x, points[1].y, points[0].z);
			v[4] = point(points[1].x, points[0].y, points[1].z);
			v[5] = point(points[0].x, points[0].y, points[1].z);
			v[6] = point(points[1].x, points[1].y, points[1].z);
			v[7] = point(points[0].x, points[1].y, points[1].z);
			mid = (points[0]+points[1])*.5;
		}
		volume() : obj(nullptr), numObjs(0), index(0)
		{
			points = new point[2];
			points[0] = point(INVALID_COORDINATE, 0, 0);
			points[1] = point(INVALID_COORDINATE, 0, 0);
			v = new point[8];
			v[0] = point(INVALID_COORDINATE, 0, 0);
			v[1] = point(INVALID_COORDINATE, 0, 0);
			v[2] = point(INVALID_COORDINATE, 0, 0);
			v[3] = point(INVALID_COORDINATE, 0, 0);
			v[4] = point(INVALID_COORDINATE, 0, 0);
			v[5] = point(INVALID_COORDINATE, 0, 0);
			v[6] = point(INVALID_COORDINATE, 0, 0);
			v[7] = point(INVALID_COORDINATE, 0, 0);
			mid = point(INVALID_COORDINATE, 0, 0);
		}
		volume(const volume & other)
		{
			points = new point[2];
			v = new point[8];
			obj = new objects::object*[other.numObjs];
			for(UInt64 i = 0; i < 2; i++)
			{
				points[i] = other.points[i];
			}
			for(UInt64 i = 0; i < 8; i++)
			{
				v[i] = other.v[i];
			}
			for(UInt64 i = 0; i < other.numObjs; i++)
			{
				obj[i] = other.obj[i];
			}
			mid = other.mid;
			numObjs = other.numObjs;
			index = other.index;
		}
		volume & operator=(const volume & other)
		{
			if(v)
				delete[] v;
			if(obj)
				delete[] obj;
			if(points)
				delete[] points;

			//std::cout << "its called" << std::endl;
			points = new point[2];
			v = new point[8];
			obj = new objects::object*[other.numObjs];
			for(UInt64 i = 0; i < 2; i++)
			{
				points[i] = other.points[i];
			}
			for(UInt64 i = 0; i < 8; i++)
			{
				v[i] = other.v[i];
			}
			for(UInt64 i = 0; i < other.numObjs; i++)
			{
				obj[i] = other.obj[i];
			}
			mid = other.mid;
			numObjs = other.numObjs;
			index = other.index;
		}
		~volume()
		{
			if(points)
			{
				delete[] points;
				points = nullptr;
			}
			if(v)
			{
				delete[] v;
				v = nullptr;
			}
			if(obj)
				delete[] obj;
		}
		bool intersects(ray3d r) const
		{
			bool doesIntersect = false;
			vec3d i(1,0,0);
			vec3d j(0,1,0);
			vec3d k(0,0,1);
			vec3d dir = r.direction;

			//first check if the ray start is in the volume; this means it intersects
			if(points[0].x >= r.start.x && points[0].y >= r.start.y && points[0].z >= r.start.z
			&& points[1].x <= r.start.x && points[1].y <= r.start.y && points[1].z <= r.start.z)
			{
				return true;
			}
			
			//we assume outward facing normals
			//we only want to get the visible faces to preform collision detection on, make sure we can "see them" with an angle test
			if(i.dot(dir) <= 0) //if outward facing normal of face and ray direction form an obtuse angle, then the face is visible
			{
				//posx 3 1 7 5
				doesIntersect = doesIntersect || rectangle(vec3d(v[3], v[1]), vec3d(v[5], v[1]), v[1], i).intersects(r);
			}
			else
			{
				//negx 2 0 6 4
				doesIntersect = doesIntersect || rectangle(vec3d(v[4], v[6]), vec3d(v[2], v[6]), v[6], -1*i).intersects(r);
			}
			
			if(j.dot(dir) <= 0)
			{
				//posy 0 1 4 5
				doesIntersect = doesIntersect || rectangle(vec3d(v[0], v[1]), vec3d(v[5], v[1]), v[1], j).intersects(r);	
			}
			else
			{
				//negy 2 3 6 7
				doesIntersect = doesIntersect || rectangle(vec3d(v[2], v[6]), vec3d(v[7], v[6]), v[6], -1*j).intersects(r);
			}

			if(k.dot(dir) <= 0)
			{
				//posz 0 1 2 3
				doesIntersect = doesIntersect || rectangle(vec3d(v[0], v[1]), vec3d(v[3], v[1]), v[1], k).intersects(r);
			}
			else
			{
				//negz 4 5 6 7
				doesIntersect = doesIntersect || rectangle(vec3d(v[4], v[6]), vec3d(v[7], v[6]), v[6], -1*k).intersects(r);
			}
			return doesIntersect;
			//return true;
		};	
		void print() const
		{
			std::cout << "Volume [ "; pp(points[0]); std::cout << " "; pp(points[1]); std::cout << " ]" << std::endl;
		}

		point * points; //0th is toprightbackmost //1st is bottomleftfrontmost
		point * v; //vertices
		objects::object ** obj;
		point mid;
		UInt64 numObjs;
		UInt64 index;
		
		struct rectangle
		{
			vec3d side1;
			vec3d side2;
			point start;
			vec3d normal;

			rectangle(vec3d s1, vec3d s2, point s, vec3d n) : side1(s1), side2(s2), start(s), normal(n) {};
			rectangle() : side1(vec3d()), side2(vec3d()), start(point()), normal(vec3d()) {};
			void print()
			{
				std::cout << "|";
				pp(start);
			       	pv(side1+(vec3d)start); std::cout << std::endl;
				pv(side2+(vec3d)start);
				pv(side1+side2+(vec3d)start); std::cout << "|" << std::endl;
			}	
			inline bool intersects(ray3d r)
			{
				if(r.direction.dot(normal))
				{
					F64 t = vec3d(start, r.start).dot(normal)/r.direction.dot(normal);
					point pos = t*r.direction+r.start;
					vec3d u1 = side1.unit();
					vec3d u2 = side2.unit();	
					F64 maxX = (side1+side2).dot(u1);
					F64 maxY = (side1+side2).dot(u2);
					F64 actX = (vec3d(pos, start)).dot(u1);
					F64 actY = (vec3d(pos, start)).dot(u2);
					if(t >= 0)
					{
						if(0 <= actX && actX <= maxX && 0 <= actY && actY <= maxY)
						{
							return true;
						}
					}
				}
				return false;
			};
		};	
		
	};
	
	bvh(std::list<objects::object*> & objs, UInt64 no);
	//bvh(const bvh & other);
	bvh() : heir(std::vector<volume>()) {};
	~bvh() = default;
	objects::object * closestIntersection(ray3d r) const;
	objects::object * closestWithOmit(ray3d r, objects::object * obj) const;

	virtual objects::object * operator()(ray3d r) const override { return closestIntersection(r); };
	virtual objects::object * operator()(ray3d r, objects::object * obj) const override { return closestWithOmit(r, obj); };
	virtual void * getVolumes() override { return &heir; };
private:
	std::vector<objects::object*> boundless;
	std::vector<volume> heir;
	UInt64 numObjs;

	void buildHeir(std::vector<objects::object**> & objs, SInt64 index, SInt64 objsBeg, SInt64 objsEnd);
	objects::object * closestIntersection_(ray3d r) const;
	objects::object * closestWithOmit_(ray3d r, objects::object * obj) const;
};

}
#define BVH_H_END_
#endif
