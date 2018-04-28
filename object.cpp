#include "object.h"
#include "polynom.h"

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>

namespace objects
{
	plane::plane(F64 c1, F64 c2, F64 c3, F64 c4, rgbColor c, F64 sh, rgbColor s) : object(c, sh, s)
	{
		normal_ = vec3d(c1, c2, c3).unit();
		includedPoint_ = point(0, 0, -c4/c3);
	}

	point plane::intersect(ray3d r) const
	{
		if(r.direction.dot(normal_))
		{
			F64 t = vec3d(includedPoint_, r.start).dot(normal_)/r.direction.dot(normal_);
			if(t >= 0)
				return (point)(t*r.direction + r.start);
		}
		return point(INVALID_COORDINATE, 0, 0);
	}

	point * plane::bounds() const
	{
		point * p = new point[2];
		p[0] = point(INVALID_COORDINATE, 0, 0);
		p[1] = point(INVALID_COORDINATE, 0, 0);
		return p;
	}
	
	triangle::triangle(point p1, point p2, point p3, rgbColor c, F64 sh, rgbColor s) : plane(c, sh, s)
	{
		normal_ = vec3d(p1, p2).cross(vec3d(p3, p2)).unit();
		includedPoint_ = p1;
		v1 = p1;
		v2 = p2;
		v3 = p3;
	}
		
	triangle::~triangle()
	{
		//nothing
	}
	F64 triangle::area(point p1, point p2, point p3) const
	{
		return (vec3d(p2, p1).cross(vec3d(p3, p1)).magnitude())/2;
	}
	
	//adapted from https://blogs.msdn.microsoft.com/rezanour/2011/08/07/barycentric-coordinates-and-point-in-triangle-tests/
	point triangle::intersect(ray3d r) const
	{
		point possInters = plane(this->normal_, this->includedPoint_, color(), shininess(), specular()).intersect(r);
		if(possInters.x == INVALID_COORDINATE)
		{
			return possInters;
		}
		
		vec3d vVec = vec3d(v2, v1);
		vec3d uVec = vec3d(v3, v1);
		vec3d wVec = vec3d(possInters, v1);

		vec3d vCrossW = vVec.cross(wVec);
		vec3d vCrossU = vVec.cross(uVec);
		
		if(vCrossW.dot(vCrossU) < 0)
		{
			return point(INVALID_COORDINATE, 0, 0);
		}

		vec3d uCrossW = uVec.cross(wVec);
		vec3d uCrossV = uVec.cross(vVec);

		if(uCrossW.dot(uCrossV) < 0)
		{
			return point(INVALID_COORDINATE, 0, 0);
		}
		
		F64 denom = vCrossU.magnitude();
		F64 r_ = vCrossW.magnitude()/denom;
		F64 t = uCrossW.magnitude()/denom;

		if(r_ > 1)
		{
			return point(INVALID_COORDINATE, 0, 0);
		}

		if(t > 1)
		{
			return point(INVALID_COORDINATE, 0, 0);
		}

		if(r_+t > 1)
		{
			return point(INVALID_COORDINATE, 0, 0);
		}

		return possInters;
	}
	
	point * triangle::bounds() const
	{
		point * p = new point[2];
		p[0] = point(std::max({v1.x, v2.x, v3.x}), std::max({v1.y, v2.y, v3.y}), std::max({v1.z, v2.z, v3.z}));
		p[1] = point(std::min({v1.x, v2.x, v3.x}), std::min({v1.y, v2.y, v3.y}), std::min({v1.z, v2.z, v3.z}));
		return p;
	}

	rectangle::rectangle(point p1, point p2, vec3d _side, rgbColor c, F64 sh, rgbColor s) : plane(c, sh, s), v1(p1), v2(p2)
	{
		vec3d diag(v2, v1);
		normal_ = diag.cross(_side);
		side = _side.unit();
		oSide = (diag - (side.dot(diag)/side.dot(side))*side).unit();
	}

	point rectangle::intersect(ray3d r) const
	{
		vec3d diag(v2, v1);
		//map to a space in R2
		F64 cornerX = side.dot(diag);
		F64 cornerY = oSide.dot(diag);
		point possInters = plane(this->normal_, this->includedPoint_, color(), shininess(), specular()).intersect(r);
		F64 x = side.dot(possInters);
		F64 y = oSide.dot(possInters);
		if(0 <= x && x <= cornerX && 0 <= y && y <= cornerY)
			return possInters;
		return point(INVALID_COORDINATE, 0, 0);
	}

	point * rectangle::bounds() const
	{
		point * p = new point[2];
		p[0] = point(std::max({v1.y, v2.y}), std::max({v1.y, v2.y}), std::max({v1.z, v2.z}));
		p[1] = point(std::min({v1.x, v2.x}), std::min({v1.y, v2.y}), std::min({v1.z, v2.z}));
		return p;
	}
		
	point sphere::intersect(ray3d r) const
	{
		vec3d oMinC = vec3d(r.start, center);
		F64 a = r.direction.dot(r.direction);
		F64 b = 2*oMinC.dot(r.direction);
		F64 c = oMinC.dot(oMinC)-radius*radius;
		polynom::posNeg ans = polynom::trisolve(a, b, c);
		F64 minT = std::fmin(ans.pos, ans.neg);
		F64 maxT = std::fmax(ans.pos, ans.neg);
		if(minT > 0)
		{
			return (point)(minT*r.direction+r.start);
		}
		else if(maxT > 0)
		{
			return (point)(maxT*r.direction+r.start);
		}
		return point(INVALID_COORDINATE, 0, 0);
	}

	point * sphere::bounds() const
	{
		point * p = new point[2];
		p[0] = point(radius+center.x, radius+center.y, radius+center.z);
		p[1] = point(center.x-radius, center.y-radius, center.z-radius);
		return p;
	}

	/*#ifdef MESH_OBJ	

	

	#endif*/
}
