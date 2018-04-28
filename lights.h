#ifndef LIGHTS_H_
#define LIGHTS_H_

#include "vec3d.h"
#include "color.h"

using namespace color;
using namespace v3d;

namespace lights
{
	class ambientLight
	{
		rgbColor color_;
		F64 intensity;
		rgbColor background_;
	public:
		ambientLight(rgbColor col, rgbColor back, F64 intens) : color_(col), intensity(intens), background_(back) {};
		ambientLight() : color_(rgbColor(0,0,0,true)), intensity(0), background_(color_) {};
		rgbColor color() { return color_*intensity; };
		rgbColor background(F64 x, F64 y) { return background_; };
	};
	
	class light
	{
	protected:
		rgbColor color_;
		F64 intensity;
		light(rgbColor col, F64 intens) : color_(col), intensity(intens) {};
	public:
		virtual ~light() = default;
		virtual vec3d getDirection(point to) = 0; //vestigial perameter "to" needed for point lights
		rgbColor color() const { return color_; };
	};

	class pointLight : public light
	{
		point source;
	public:
		pointLight(point _source, rgbColor col, F64 intens) : light(col, intens), source(_source) {};
		virtual ~pointLight() override = default;
		virtual vec3d getDirection(point to) override { return (1/(vec3d(source, to).dot(vec3d(source, to))))*vec3d(source, to); };
		
	};
	
	class directionalLight : public light
	{
		vec3d direction;
	public:
		directionalLight(vec3d dir, rgbColor col, F64 intens) : light(col, intens), direction(dir.unit()) {};
		virtual ~directionalLight() override = default;
		virtual vec3d getDirection(point to) override { return -1*direction; };
	};
}

#endif
