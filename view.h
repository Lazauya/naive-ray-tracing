#ifndef VIEW_H_
#define VIEW_H_

#include "vec3d.h"
#include <random>
#include <utility>

using namespace v3d;

namespace view
{
	typedef std::pair<UInt64, UInt64> resolution;
	typedef std::pair<F64, F64> dimensions;

	class viewWindow
	{
	public:
		point windowMiddle;
		F64 angleTheta;
		F64 anglePhi;
		resolution res;
		dimensions dim;
		UInt64 subdivisions;
		std::default_random_engine gen;
		std::uniform_real_distribution<F64> dist;
		viewWindow(point mid, F64 at, F64 ap, resolution r, dimensions d, UInt64 s) : windowMiddle(mid), angleTheta(at), anglePhi(ap), res(r), dim(d), subdivisions(s) {};
	public:
		virtual ~viewWindow() = default;
		virtual ray3d *** rays() = 0;
	};

	class perspectiveViewWindow : public viewWindow
	{
		point startPoint;
		vec3d ** vectors;
	public:
		perspectiveViewWindow(point start, point middle, F64 angleT, F64 angleP, resolution _res, dimensions _dim, UInt64 subs);
		perspectiveViewWindow(F64 distBehind, point middle, F64 angleT, F64 angleP, resolution _res, dimensions _dim, UInt64 subs);
		virtual ~perspectiveViewWindow() override;
		virtual ray3d *** rays() override;
	};

	class orthographicViewWindow : public viewWindow
	{
		vec3d direction;
		point ** startPoints;
	public:
		orthographicViewWindow(point middle, F64 angleT, F64 andleP, resolution _res, dimensions _dim, UInt64 subs);
		virtual ~orthographicViewWindow() override;
		virtual ray3d *** rays() override;
	};
}

#endif
