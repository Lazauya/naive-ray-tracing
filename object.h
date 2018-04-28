#ifndef OBJECT_H_
#define OBJECT_H_

#include "vec3d.h"
#include "color.h"

using namespace v3d;
using namespace color;

namespace objects
{
	class object
	{
		rgbColor color_;
		rgbColor specular_;
		F64 shininess_;
		rgbColor relflectance_; //the color to reflect
		F64 scattering_; //the angle of light scattering; the roughness of the object surface
		F64 eta_; //the refraction number
	public:
		object(rgbColor c, F64 sh, rgbColor s, rgbColor ref=rgbColor(0,0,0,true), F64 scat=0.0, F64 eta=1.0) : color_(c), shininess_(sh), specular_(s),
	       												scattering_(scat), reflectance_(ref), eta_(eta) {};
		virtual ~object() = default;
		virtual point intersect(ray3d r) const = 0;
		virtual vec3d normal(ray3d r, point p, point t) const = 0;
		virtual point * bounds() const = 0;
		
		inline rgbColor color() const { return color_; };
		inline rgbColor specular() const { return specular_; };
		inline F64 shininess() const { return shininess_; };
		inline rgbColor relflectance() const { return reflectance_; };
		inline F64 shininess() const { return shininess_; };
		inline F64 eta() const { return eta_; };
		
		void setColor(rgbColor nc) { color_ = nc; };
		void setShininess(F64 nsh) { shininess_ = nsh; };
	};
	
	class plane : public object
	{
	protected:
		vec3d normal_;
		point includedPoint_;
		plane(rgbColor c, F64 sh, rgbColor s) : object(c, sh, s), normal_(vec3d()), includedPoint_(point()) {};
	public:
		plane(F64 c1, F64 c2, F64 c3, F64 c4, rgbColor c, F64 sh, rgbColor s); //initialize plane with given parameters (coefficients)
		plane(vec3d n, point p, rgbColor c, F64 sh, rgbColor s) : object(c, sh, s), normal_(n), includedPoint_(p) {};
		virtual ~plane() override = default;
		virtual point * bounds() const override;
		virtual point intersect(ray3d r) const;
		virtual vec3d normal(ray3d r, point p, point t) const { return vec3d(p,t).unit().dot(normal_.unit()) > 0 ? -1*normal_.unit() : normal_.unit();  };
	};
	
	class triangle : public plane
	{
		point v1;
		point v2;
		point v3;
		F64 area(point v1, point v2, point v3) const;
	public:
		triangle(point p1, point p2, point p3, rgbColor c, F64 sh, rgbColor s);
		virtual ~triangle() override;
		virtual point intersect(ray3d r) const override;
		virtual vec3d normal(ray3d r, point p, point t) const { return vec3d(p,t).unit().dot(normal_.unit()) > 0 ? -1*normal_.unit() : normal_.unit(); };
		virtual point * bounds() const override;
		void print() const { pp(v1); pp(v2); pp(v3); std::cout << std::endl; };
	};

	class rectangle : public plane
	{
		point v1;
		point v2;
		vec3d side;
		vec3d oSide;
		F64 area(point v1, point v2) const;
	public:
		rectangle(point p1, point p2, vec3d norm, rgbColor c, F64 sh, rgbColor s);
		virtual ~rectangle() override = default;
		virtual point intersect(ray3d r) const override;
		virtual vec3d normal(ray3d r, point p, point t) const override { return vec3d(p,t).unit().dot(normal_.unit()) > 0 ? -1*normal_.unit() : normal_.unit(); };
		virtual point * bounds() const override;
	};
	
	class sphere : public object
	{
		point center;
		F64 radius;
	public:
		sphere(point cen, F64 rad, rgbColor col, F64 sh, rgbColor s) : object(col, sh, s), center(cen), radius(rad) {};
		virtual ~sphere() override = default;
		virtual point intersect(ray3d r) const override;
		virtual vec3d normal(ray3d r, point p, point t) const override { return vec3d(p, center).unit(); };
		virtual point * bounds() const override;
	};
}
	
#endif
