#ifndef SCENE_H_
#define SCENE_H_

#include <list>
#include <png++/png.hpp>
#include "view.h"
#include "object.h"
#include "lights.h"
#include "access.h"
#include "bvh.h"
#include <string>

using namespace objects;
using namespace lights;
using namespace view;

typedef unsigned char UInt8;

namespace scene
{
	template<class shader>
	class basicScene
	{
	public:
		basicScene() : objectList(std::list<typename objects::object*>()), lightList(std::list<typename lights::light*>()), ambience(ambientLight()), window(nullptr){};
		std::list<typename objects::object*> objectList;
		std::list<typename lights::light*> lightList; //for direction/point lights only;
		
		inline void addObj(typename objects::object * obj) { objectList.push_back(obj); };
		inline void addLight(typename lights::light * l) { lightList.push_back(l); };
		inline void remObj(typename objects::object * obj) { objectList.remove(obj); };
		inline void remLight(typename lights::light * l) { lightList.remove(l); };
		
		ambientLight ambience;
		viewWindow * window;

		png::image<png::rgb_pixel> * render()
		{
			png::image<png::rgb_pixel> * outImage = new png::image<png::rgb_pixel>(window->res.first, window->res.second);
			auto rays = this->window->rays();
			access_n::linearAccess acc(objectList);

			UInt64 pixel = 0;
			for(UInt64 x = 0; x < window->res.first; x++)
			{
				for(UInt64 y = 0; y < window->res.second; y++)
				{
					rgbColor avg(0,0,0, true);
					for (UInt64 r = 0; r < pow(window->subdivisions,2); r++)
					{
						
						//first find the intersection point closest to the origin
						
						shader shad;
						rgbColor col = shad(ambience,
							       	&lightList,
							       	acc(rays[x][y][r]),
							       	rays[x][y][r],
							       	&acc,
								x,
								y);
						avg = avg + col;
					}
					avg = avg*(1/pow(window->subdivisions,2));
					avg.normalize();
					avg.changeMode(false);
					outImage->set_pixel(x, outImage->get_height()-1-y, png::rgb_pixel((UInt8)round(avg.r()),  (UInt8)round(avg.g()), (UInt8)round(avg.b())));
					if(!(pixel%100))
						std::cout << (F64)(pixel)/(F64)(window->res.second*window->res.first)*100 << "%" << std::endl;
					pixel++;
				}
			}
			return outImage;
		}	
	};

	template<class shader>
	class BVHScene
	{
	public:
		BVHScene() : objectList(std::list<typename objects::object*>()), lightList(std::list<typename lights::light*>()), ambience(ambientLight()), window(nullptr){};
		std::list<typename objects::object*> objectList;
		std::list<typename lights::light*> lightList; //for direction/point lights only;
		ambientLight ambience;
		viewWindow * window;
		inline void addObj(typename objects::object * obj) { objectList.push_back(obj); };
		inline void addLight(typename lights::light * l) { lightList.push_back(l); };
		inline void remObj(typename objects::object * obj) { objectList.remove(obj); };
		inline void remLight(typename lights::light * l) { lightList.remove(l); };

		png::image<png::rgb_pixel> * render()
		{
			png::image<png::rgb_pixel> * outImage = new png::image<png::rgb_pixel>(window->res.first, window->res.second);
			auto rays = this->window->rays();
			bvh_n::bvh heirarchy(objectList, 6);
			
			UInt64 pixel = 0;
			for(UInt64 x = 0; x < window->res.first /*/2*/; x++)
			{
				for(UInt64 y = 0 /*window->res.second /2+window->res.second/4*/; y < window->res.second; y++)
				{
					rgbColor avg(0,0,0, true);
					for (UInt64 r = 0; r < pow(window->subdivisions,2); r++)
					{
						
						//first find the intersection point closest to the origin
						shader shad;
						#ifdef DEBUG_SHAD
						std::cout << "Shade begin" << std::endl;
						#endif
						rgbColor col = shad(ambience, 
								&lightList, 
								heirarchy(rays[x][y][r]), 
								rays[x][y][r], 
								&heirarchy,
								x,
								y);
						avg = avg + col;
					}
					avg = avg*(1/pow(window->subdivisions,2));
					avg.normalize();
					avg.changeMode(false);
					outImage->set_pixel(x, outImage->get_height()-1-y, png::rgb_pixel((UInt8)round(avg.r()),  (UInt8)round(avg.g()), (UInt8)round(avg.b())));
					//debug info
					if(!(pixel%100))
						std::cout << (F64)(pixel)/(F64)(window->res.second*window->res.first)*100 << "%" << std::endl;
					pixel++;
				}
			}
			return outImage;

		}
	};
}

#endif
