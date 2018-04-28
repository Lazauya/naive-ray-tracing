#include "scene.h"

namespace scene
{
	/*typedef png::image<png::rgb_pixel> img;

	template<class shader>
	img * basicScene<shader>::render() const
	{
		png::image<png::rgb_pixel> * outImage = new png::image<png::rgb_pixel>(window->res.first, window->res.second);
		auto rays = this->window->rays();
		for(UInt64 x = 0; x < window->res.first; x++)
		{
			for(UInt64 y = 0; y < window->res.second; y++)
			{
				rgbColor avg(0,0,0, true);

				for (UInt64 r = 0; r < pow(window->subdivisions,2); r++)
				{
					//first find the intersection point closest to the origin
					F64 leastMag = 0;
					object * closest = objectList.front;
					for (auto obj = objectList.begin(); obj != objectList.end(); obj++)
					{
						point pos = (*obj)->intersect(rays[x][y][r]);

						if(pos.x != INVALID_COORDINATE)
						{
							F64 mag = vec3d(pos,rays[x][y][r].start).magnitude();
							if(mag < leastMag)
							{
								leastMag = mag;
								closest = (*obj);
							}
						}
					}
					rgbColor col = shader(lightList, closest, rays[x][y][r]);
					if(!r)
						avg = col;
					avg = rgbColor((col.r() + avg.r())/pow(window->subdivisions,2), 
							(col.g() + avg.g())/pow(window->subdivisions,2),
							(col.b() + avg.b())/pow(window->subdivisions,2), true);
				}
				avg.changeMode(false);
				outImage[x][y] = png::rgb_pixel(avg.r(), avg.b(), avg.g());
			}
		}
		return outImage;
	}*/

	//always the templates giving me trouble, eh?
}
