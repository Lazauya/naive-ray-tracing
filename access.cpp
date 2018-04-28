#include "access.h"
#include "vec3d.h"

namespace access_n
{
	objects::object * linearAccess::operator()(ray3d r) const
	{
		point c(INVALID_COORDINATE, 0, 0);
		objects::object * best = nullptr;
		for(auto & o : objs)
		{
			point ip = o->intersect(r); 
			if(ip.x != INVALID_COORDINATE && ip.distSqr(r.start) < c.distSqr(r.start))
			{
				c = ip;
				best = o;
			}
		}
		return best;
	}
	objects::object * linearAccess::operator()(ray3d r, objects::object * obj) const
	{
		point c(INVALID_COORDINATE, 0, 0);
		objects::object * best = nullptr;
		for(auto & o : objs)
		{
			point ip = o->intersect(r); 
			if(ip.x != INVALID_COORDINATE && ip.distSqr(r.start) < c.distSqr(r.start) && o != obj)
			{
				c = ip;
				best = o;
			}
		}
		return best;
	}
}	
