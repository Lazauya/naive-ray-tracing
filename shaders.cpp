#include "shaders.h"
#include "bvh.h"
#include <cmath>

#include <iostream>
namespace shaders
{
		
	rgbColor phong::operator()(ambientLight a, std::list<light*> * l, object * obj, ray3d viewRay, access_n::access * getObject, F64 x, F64 y) const 
	{
		if(obj) 
		{
			point intersPoint = obj->intersect(viewRay);
			if(intersPoint.x == INVALID_COORDINATE)
			{
				return a.background(x, y);
			}
			vec3d N = obj->normal(viewRay, intersPoint, viewRay.start);
		
			rgbColor out = obj->color()*a.color();
			
			for (auto m = l->begin(); m != l->end(); m++)
			{
				vec3d L = (*m)->getDirection(intersPoint).unit();
				
				

				//shadow casting
				if((*getObject)(ray3d(intersPoint, L), obj)) 	//if there is an object to speak of
					continue;//std::cout << (*getObject)(ray3d(intersPoint, L), obj) << std::endl;		//dont do specular and all that jazz
			
				//coloring
				vec3d R = L.rotate(N, PI); //rotate the L vec 180 deg (PI rad) around the normal vec to get the reflection
				vec3d V =-1*viewRay.direction.unit();
				F64 LDotN = L.dot(N);
				if(LDotN > 0)
				{
					F64 RDotV = R.dot(V);
					if(RDotV > 0)
					{
						out = out + LDotN*obj->color()*(*m)->color() + pow(RDotV, obj->shininess())*obj->specular()*(*m)->color();
					}
					else
					{
						out = out + LDotN*obj->color()*(*m)->color();
					}
				}
			}
			return out;
		}
		else
		{
			return a.background(x, y);
		}
	}

	rgbColor fresnel::operator()(ambientLight a, std::list<light*> * l, object * obj, ray3d viewRay, access_n::access * getObject, F64 x, F64 y) const
	{

	}

	rgbColor volumeTest::operator()(ambientLight a, std::list<light*> * l, object * obj, ray3d viewRay, access_n::access * getObject, F64 x, F64 y) const
	{
		auto vs = reinterpret_cast<std::vector<bvh_n::bvh::volume*>* >(getObject->getVolumes());
		rgbColor out = a.background(x,y);
		for(int i = 1; i < vs->size(); i++)
		{
			if(!(*vs)[i])
				continue;
			//(*vs)[i]->print();
			//std::cout << "=======================" << std::endl;	
			if((*vs)[i]->intersects(viewRay))
			{
				//std::cout << "Intersects 0" << std::endl;
				out = out + rgbColor(1,0,0,true);
			}
			
		}
		return out;
	}
}
