#include <string>
#include "bvh.h"

#ifndef MESH_H_ //do this so we can find the bvh
#define MESH_H_

namespace objects
{
	class mesh : public object
	{
		bvh_n::bvh heir;
		point b1;
		point b2;
	public:
		mesh(std::list<object*> & f, rgbColor col, F64 sh, rgbColor s);
		mesh(std::string filename, rgbColor col, F64 sh, rgbColor s); //must be a .off file
		virtual ~mesh() override = default;
		virtual point intersect(ray3d r) const override;
		virtual vec3d normal(ray3d r, point p, point t) const override;
		virtual point * bounds() const override;
	};
}

#endif

