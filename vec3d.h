#ifndef VEC3D_H_
#define VEC3D_H_

#define INVALID_COORDINATE 0xFFFFFFFFFFFFFFFF
#define PI 3.14159265

#include <iostream>

typedef double F64;
typedef unsigned long UInt64;
typedef signed long SInt64;

namespace v3d
{
	class point
	{
	public:
		point(F64 _x, F64 _y, F64 _z) : x(_x), y(_y), z(_z) {};
		point(const point & other) : x(other.x), y(other.y), z(other.z) {};
		point() : x(0), y(0), z(0) {};
		point operator+(point rhs) const;
		point operator-(point rhs) const;
		point operator*(F64 scalar) const;
		bool operator==(point & rhs) const;
		F64 distSqr(point other) const; //squared distance
		F64 x;
		F64 y;
		F64 z;
	};
	
	point operator*(F64 s, point p);

	class vec3d : public point
	{
	public:
		vec3d operator*(F64 scalar) const;
		vec3d operator-(vec3d rhs) const;
		vec3d operator+(vec3d rhs) const;
		vec3d(F64 _x, F64 _y, F64 _z) : point(_x, _y, _z) {};
		vec3d(point p1, point p2) : point(p1-p2) {};
		vec3d(point p) : point(p) {};
		vec3d() : point() {};
		vec3d normal(const vec3d & direction) const; //returns the projection of the vector "direction" onto the normal plane of this
		vec3d rotate(const vec3d & direction, F64 angle) const; //rotate the currect vector around another vector for a give angle
		vec3d unit() const;
		F64 dot(const vec3d & rhs) const;
		vec3d cross(const vec3d & rhs) const;
		F64 magnitude() const;
	};

	vec3d operator*(F64 s, vec3d v);
		
	class ray3d
	{
	public:
		vec3d direction;
		point start;
		ray3d(F64 _xs, F64 _ys, F64 _zs, F64 _xd, F64 _yd, F64 _zd) : start(point(_xs, _ys, _zs)), direction(vec3d(_xd, _yd, _zd)) {};
		ray3d(point s, vec3d d) : start(s), direction(d) {};
		ray3d() : start(point()), direction(vec3d()) {};
		point cast(F64 t) const;	
	};

	void pr(ray3d r);
	void pv(vec3d v);
	void pp(point p);
}

#endif
