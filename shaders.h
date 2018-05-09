#ifndef SHADERS_H_
#define SHADERS_H_

#include <png++/png.hpp>
#include "color.h"
#include "vec3d.h"
#include "object.h"
#include "lights.h"
#include "access.h"
#include <list>

using namespace v3d;
using namespace color;
using namespace objects;
using namespace lights;

namespace shaders
{
	class shader
	{
	public: 
		virtual rgbColor operator()(ambientLight a, std::list<light*> * l, object * obj, ray3d viewRay, access_n::access * getObject, F64 x, F64 y,
			       	UInt64 depth=0) const = 0;
	};
	
	class phong : public shader
	{
	public:
		virtual rgbColor operator()(ambientLight a, std::list<light*> * l, object * obj, ray3d viewRay, access_n::access * getObject, F64 x, F64 y,
				UInt64 depth=0) const override;
	};

	class reflection : public shader
	{
	public:
		virtual rgbColor operator()(ambientLight a, std::list<light*> * l, object * obj, ray3d viewRay, access_n::access * getObject, F64 x, F64 y,
			       	UInt64 depth=0) const override;
	};

	class fresnel : public shader
	{
	public:
		virtual rgbColor operator()(ambientLight a, std::list<light*> * l, object * obj, ray3d viewRay, access_n::access * getObject, F64 x, F64 y,
			       	UInt64 depth=0) const override;
	};
	
	class volumeTest : public shader
	{
	public:
		virtual rgbColor operator()(ambientLight a, std::list<light*> * l, object * obj, ray3d viewRay, access_n::access * getObject, F64 x, F64 y,
				UInt64 depth=0) const override;
	};
}



#endif
