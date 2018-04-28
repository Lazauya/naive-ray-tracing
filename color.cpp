#include "color.h"
#include <cmath>

namespace color
{
	rgbColor::rgbColor(F64 _r, F64 _g, F64 _b, bool isZeroOne) : r_(_r), g_(_g), b_(_b), mode_(isZeroOne)
	{
		//normalize();
	}
	void rgbColor::changeMode(bool newMode)
	{
		if(newMode == mode_)
			return;
		if(newMode) //if new mode is zero-one
		{
			r_ /= 255.0;
			g_ /= 255.0;
			b_ /= 255.0;
		}
		else //if new mode is 0-255
		{
			r_ *= 255.0;
			g_ *= 255.0;
			b_ *= 255.0;
		}
		mode_ = newMode;
	}
	void rgbColor::setR(F64 _r)
	{
		r_ = _r;
		//normalize();
	}
	void rgbColor::setG(F64 _g)
	{
		g_ = _g;
		//normalize();
	}
	void rgbColor::setB(F64 _b)
	{
		b_ = _b;
		//normalize();
	}
	
	rgbColor rgbColor::operator+(const rgbColor c1) const
	{
		rgbColor copy1 = *this;
		rgbColor copy2 = c1;
		
		copy1.changeMode(true);
		copy2.changeMode(true);
		return rgbColor(copy1.r() + copy2.r(), copy1.g() + copy2.g(), copy1.b() + copy2.b(), true);

	}

	rgbColor rgbColor::operator*(const rgbColor c1) const
	{
		rgbColor copy1 = *this;
		rgbColor copy2 = c1;
		
		copy1.changeMode(true);
		copy2.changeMode(true);
		return rgbColor(copy1.r()*copy2.r(), copy1.g()*copy2.g(), copy1.b()*copy2.b(), true);
	}

	rgbColor operator*(const F64 mult, const rgbColor rhs)
	{
		rgbColor copy1 = rhs;
		
		copy1.changeMode(true);
		return rgbColor(copy1.r()*mult, copy1.g()*mult, copy1.b()*mult, true);
	}
	rgbColor operator*(const rgbColor lhs, const F64 mult)
	{
		return mult*lhs;
	}

	void pc(rgbColor c) { std::cout << "RGB[" << c.r() << ", " << c.g() << ", " << c.b() << "]" << std::endl; }	

	void rgbColor::normalize()
	{	
		if(std::fmax(std::fmax(r_, g_), b_) > (mode_ ? 1 : 255))
		{
			F64 m = std::fmax(std::fmax(r_, g_), b_);
			r_ /= m;
			g_ /= m;
			b_ /= m;
			//std::cout << "After: "; pc(*this);

		}
	}

	/*rgbColor addWithoutNormal(const rgbColor c1) const
	{
		return rgbColor(c
	}*/

	/*rgbaColor::rgbaColor(F64 _r, F64 _g, F64 _b, F64 _a, bool isZeroOne) : rgbColor(_r, _g, _b, isZeroOne)
	{
		if(isZeroOne)
			a_ = _a > 0 ? (_a > 1 ? 1 : _a) : 0;
		else
			a_ = _a > 0 ? (_a > 255 ? 255 : _a) : 0;
	}
	void rgbaColor::setA(F64 _a)
	{
		if(mode_)
			a_ = _a > 0 ? (_a > 1 ? 1 : _a) : 0;
		else
			a_ = _a > 0 ? (_a > 255 ? 255 : _a) : 0;
	}

	rgbaColor rgbaColor::operator+(const rgbaColor c1) const
	{
		rgbaColor copy1 = *this;
		rgbaColor copy2 = c1;
		
		copy1.changeMode(true);
		copy2.changeMode(true);
		return rgbaColor(
			copy1.r_+copy2.r_ < 1 ? copy1.r_+copy2.r_ : 1,
			copy1.g_+copy2.g_ < 1 ? copy1.g_+copy2.g_ : 1,
			copy1.b_+copy2.b_ < 1 ? copy1.b_+copy2.b_ : 1,
			copy1.a_+copy2.a_ < 1 ? copy1.a_+copy2.a_ : 1,
			true);

	}
	
	rgbaColor rgbaColor::operator*(const rgbaColor c1) const
	{
		rgbaColor copy1 = *this;
		rgbaColor copy2 = c1;
		
		copy1.changeMode(true);
		copy2.changeMode(true);
		return rgbaColor(copy1.r_*copy2.r_, copy1.g_*copy2.g_, copy1.b_*copy2.b_, copy1.a_*copy2.a_, true);
	}*/
}
