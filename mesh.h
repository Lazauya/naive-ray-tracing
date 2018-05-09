#include <string>
#include "bvh.h"

#ifndef MESH_H_ //do this so we can find the bvh
#define MESH_H_

namespace objects
{
	class mesh : public object
	{
		bvh_n::bvh heir;
		std::vector<object*> objs;
		point b1;
		point b2;
	public:
		mesh(std::list<object*> & f, rgbColor col, F64 sh, rgbColor s, rgbColor ref=rgbColor(0,0,0,true), F64 t=0.0, F64 scat=0.0, F64 eta=1.0);
		mesh(std::string filename, rgbColor col, F64 sh, rgbColor s, rgbColor ref=rgbColor(0,0,0,true), F64 t=0.0, F64 scat=0.0, F64 eta=1.0); //must be a .off file
		virtual ~mesh() override = default;
		virtual point intersect(ray3d r) const override;
		virtual vec3d normal(ray3d r, point p, point t) const override;
		virtual point * bounds() const override;
		void rotate(point cen, vec3d dir, F64 rad);
		void translate(F64 x, F64 y, F64 z);
		void scale(point cen, F64 s);
	};
}

#endif

