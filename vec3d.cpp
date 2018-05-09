#include <cmath>
#include "vec3d.h"


namespace v3d
{
	point point::operator+(point rhs) const
	{
		return point(rhs.x+x, rhs.y+y, rhs.z+z);
	}
	point point::operator-(point rhs) const
	{
		return point(x-rhs.x, y-rhs.y, z-rhs.z);
	}
	point point::operator*(F64 scalar) const
	{
		return point(scalar*x, scalar*y, scalar*z);
	}
	bool point::operator==(point & rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z;
	}
	F64 point::distSqr(point other) const
	{
		return (other.x-x)*(other.x-x) + (other.y-y)*(other.y-y) + (other.z-z)*(other.z-z);
	}
	point operator*(F64 s, point p)
	{
		return p*s;
	}

	vec3d vec3d::operator*(F64 scalar) const
	{
		return vec3d(x*scalar, y*scalar, z*scalar);
	}
	
	vec3d vec3d::operator+(vec3d rhs) const
	{
		return vec3d(x+rhs.x, y+rhs.y, z+rhs.z);
	}
	vec3d vec3d::operator-(vec3d rhs) const
	{
		return vec3d(x-rhs.x, y-rhs.y, z-rhs.z);
	}
	vec3d operator*(F64 s, vec3d v)
	{
		return vec3d(s*v.x, s*v.y, s*v.z);
	}
	vec3d vec3d::normal(const vec3d & direction) const
	{
		return direction - ((direction.dot(*this))/(this->dot(*this)))*(*this);
	}
	
	F64 vec3d::dot(const vec3d & rhs) const
	{
		return x*rhs.x + y*rhs.y + z*rhs.z;
	}
	
	vec3d vec3d::unit() const
	{
		return (1/this->magnitude())*(*this);
	}
	
	F64 vec3d::magnitude() const
	{
		return sqrt(this->dot(*this));
	}

	vec3d vec3d::cross(const vec3d & rhs) const
	{
		
		F64 i = y*rhs.z-z*rhs.y;
		F64 j = -x*rhs.z+z*rhs.x;
		F64 k = x*rhs.y-y*rhs.x;
		return vec3d(i, j, k);
	}
	
	vec3d vec3d::rotate(const vec3d & direction, F64 angle) const
	{
		auto dir = direction;	
		if((x == 0 && y == 0 && z == 0))
			return vec3d(0,0,0);
		if(direction.x == 0 && direction.y == 0 && direction.z == 0)
			return *this;
		if(direction.cross(*this).magnitude() == 0)
			return *this;
		vec3d norm = dir.normal(*this);// pv(norm); 
		vec3d para = (*this) - norm;// pv(para);
		vec3d w = dir.cross(norm); //pv(w);
		F64 x1 = cos(angle)/norm.magnitude(); //std::cout << x1 << std::endl;
		F64 x2 = sin(angle)/w.magnitude(); //std::cout << x2 << std::endl;
		vec3d normrot = norm.magnitude()*(x1*norm + x2*w); //pv(normrot);
		return normrot + para; 
	}
	
	point ray3d::cast(F64 t) const
	{
		return point(direction.x*t+start.x, direction.y*t+start.y, direction.z*t+start.z);
	}

	void pr(ray3d r) { std::cout << "[ "; pp(r.start); std::cout << ", "; pv(r.direction); std::cout << " >"; }
	void pv(vec3d v) { std::cout << "<" << v.x << ", " << v.y << ", " << v.z << ">"; }
	void pp(point p) { std::cout << "(" << p.x << ", " << p.y << ", " << p.z << ")"; }

}
