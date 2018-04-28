#ifndef _ACCESS_H_
#define _ACCESS_H_

#include "object.h" 
#include "vec3d.h"

#include <list>

/*namespace objects
{
	class object;
}*/

namespace access_n
{
	class access
	{
	public:
		virtual objects::object * operator()(ray3d r) const = 0;
		virtual objects::object * operator()(ray3d r, objects::object * obj) const = 0;
		virtual void * getVolumes() = 0;
	};

	class linearAccess : public access
	{
		std::list<objects::object *> objs;
	public:
		linearAccess(std::list<objects::object *> & o) : objs(o) {};
		virtual objects::object * operator()(ray3d r) const override;
		virtual objects::object * operator()(ray3d r, objects::object * obj) const override;
		virtual void * getVolumes() override { return nullptr; };
	};
}


#endif
