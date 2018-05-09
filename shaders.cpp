#include "shaders.h"
#include "bvh.h"
#include <cmath>

#define ETA_AIR 1
#define MAX_DEPTH 10

#include <iostream>
namespace shaders
{
	rgbColor phong::operator()(ambientLight a, std::list<light*> * l, object * obj, ray3d viewRay, access_n::access * getObject, F64 x, F64 y, UInt64 depth) const 
	{
		if(depth > MAX_DEPTH)
		{
			return rgbColor(0,0,0,true);
		}
		else if(obj) 
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
	
	rgbColor reflection::operator()(ambientLight a, std::list<light*> * l, object * obj, ray3d viewRay, access_n::access * getObject, F64 x, F64 y, UInt64 depth) const
	{
		if(depth > MAX_DEPTH)
		{
			return rgbColor(1,1,1,true);
		}
		else if(obj)
		{
			point intersPoint = obj->intersect(viewRay);
			if(intersPoint.x == INVALID_COORDINATE)
			{
				return a.background(x, y);
			}

			vec3d N = obj->normal(viewRay, intersPoint, viewRay.start);
			
			rgbColor out = obj->color()*a.color();
			
			vec3d V =-1*viewRay.direction.unit();
			
			//transparency treated as a marker for reflectivity (for convienience, may fix later)

			if(obj->transparency() == 0)
			{
			for (auto m = l->begin(); m != l->end(); m++)
			{
				rgbColor outTemp(0,0,0,true);

				vec3d L = (*m)->getDirection(intersPoint).unit();		

				//shadows, assumed no transparency
				if((*getObject)(ray3d(intersPoint, L), obj))
					continue;	

				//coloring
				vec3d R = L.rotate(N, PI); //rotate the L vec 180 deg (PI rad) around the normal vec to get the reflection	
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
			}
			else
			{
			//reflections
			F64 VDotN = V.dot(N);
			F64 reflectionAngle = acos(VDotN); //actually used for refraction
			vec3d R_V = V.rotate(N, PI); //reflection vector of viewRay
			
			object * reflectionObj = (*getObject)(ray3d(intersPoint, R_V), obj);

			rgbColor reflectiveColor = (*this)(a, l, reflectionObj, ray3d(intersPoint, R_V), getObject, x, y, depth+1);
			#ifdef DEBUG_SHAD
			pc(reflectiveColor);
			#endif
			out = reflectiveColor;
			}
			return out;
		}
		else
		{
			return a.background(x, y);
		}
	}

	rgbColor fresnel::operator()(ambientLight a, std::list<light*> * l, object * obj, ray3d viewRay, access_n::access * getObject, F64 x, F64 y, UInt64 depth) const
	{
		if(depth > MAX_DEPTH)
		{
			return rgbColor(1,1,1,true);
		}
		else if(obj)
		{
			point intersPoint = obj->intersect(viewRay);
			if(intersPoint.x == INVALID_COORDINATE)
			{
				return a.background(x, y);
			}

			vec3d N = obj->normal(viewRay, intersPoint, viewRay.start);
			
			rgbColor out = obj->color()*a.color();
			
			vec3d V =-1*viewRay.direction.unit();

			for (auto m = l->begin(); m != l->end(); m++)
			{
				rgbColor outTemp(0,0,0,true);

				vec3d L = (*m)->getDirection(intersPoint).unit();	
				
				object * nextObj = obj;
				ray3d nextRay = ray3d(intersPoint, L);
				point nextInters = intersPoint;

				//shadow casting (no shadows from transparent objects)
				do
				{
					nextObj = (*getObject)(nextRay, nextObj);
					if(nextObj)
					{	
						nextInters = nextObj->intersect(nextRay);
						nextRay = ray3d(nextInters, L);
					}
					
				}
				while(nextObj && nextObj->transparency() > 0);

				//old shadows, ignore?
				//if((*getObject)(ray3d(intersPoint, L), obj))
				//	continue;
				
				if(nextObj)
					continue;

				//coloring
				vec3d R = L.rotate(N, PI); //rotate the L vec 180 deg (PI rad) around the normal vec to get the reflection	
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

			//reflections
			F64 VDotN = V.dot(N);
			F64 reflectionAngle = acos(VDotN); //actually used for refraction
			vec3d R_V = V.rotate(N, PI); //reflection vector of viewRay
			
			object * reflectionObj = (*getObject)(ray3d(intersPoint, R_V), obj);

			rgbColor reflectiveColor = (*this)(a, l, reflectionObj, ray3d(intersPoint, R_V), getObject, x, y, depth+1);
			#ifdef DEBUG_SHAD
			pc(reflectiveColor);
			#endif

			//refraction	
			F64 refractionAngle = asin(ETA_AIR*sin(reflectionAngle)/obj->eta());
			vec3d T = V.rotate(V.cross(N), reflectionAngle + PI - refractionAngle); //original refraction vector
			
			object * refractionObj = (*getObject)(ray3d(intersPoint, T), obj);

			rgbColor refractiveColor = (*this)(a, l, refractionObj, ray3d(intersPoint, T), getObject, x, y, depth+1);	
			#ifdef DEBUG_SHAD
			pc(reflectiveColor);
			#endif

			F64 F_Rperp = 0;
			F64 F_Rpara = 0;
			
			#ifdef DEBUG_SHAD
			std::cout <<
			"ETA_AIR: " << ETA_AIR << std::endl <<
			"cos(fr): " << cos(refractionAngle) << std::endl <<
			"obj.eta: " << obj->eta() << std::endl <<
			"cos(fl): " << cos(reflectionAngle) << std::endl;
			std::cout << "Denom: " << ETA_AIR*cos(refractionAngle)+obj->eta()*cos(reflectionAngle) << std::endl;
			#endif

			if(ETA_AIR*cos(refractionAngle)+obj->eta()*cos(reflectionAngle) != 0.0)
			{
				F_Rperp = pow(
					(ETA_AIR*cos(refractionAngle) - obj->eta()*cos(reflectionAngle))/
					(ETA_AIR*cos(refractionAngle) + obj->eta()*cos(reflectionAngle))
					, 2);
				F_Rpara = pow(
					(obj->eta()*cos(reflectionAngle) - ETA_AIR*cos(refractionAngle))/
					(ETA_AIR  * cos(refractionAngle) + obj->eta()*cos(reflectionAngle))
					, 2);
			}
			

			F64 F_R = .5*(F_Rperp+F_Rpara);
			F64 F_T = 1 - F_R; //fraction of refraction light

			out = (reflectiveColor*F_R + refractiveColor*F_T)*obj->transparency() + out*(1-obj->transparency());
			#ifdef DEBUG_SHAD
			pc(out);
			
			std::cout << "=========" << "depth: " << depth << "==========" << std::endl;
			#endif
			return out;
		}
		else
		{
			return a.background(x, y);
		}
	}

	rgbColor volumeTest::operator()(ambientLight a, std::list<light*> * l, object * obj, ray3d viewRay, access_n::access * getObject, F64 x, F64 y, UInt64 depth) const
	{
		if(depth > MAX_DEPTH)
		{
			return rgbColor(0,0,0,true);
		}
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
