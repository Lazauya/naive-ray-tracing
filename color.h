#ifndef COLOR_H_
#define COLOR_H_

#include <iostream>

typedef double F64;

namespace color
{
	class rgbColor
	{
		bool mode_; //true - color is on unit interval; false - color is from 0 to 255
		F64 r_;
		F64 g_;
		F64 b_;
		
	public:
		rgbColor(F64 _r, F64 _g, F64 _b, bool isZeroOne);
		void changeMode(bool newMode);
		inline F64 mode() const { return mode_; }
		inline F64 r() const { return r_; }
		inline F64 g() const { return g_; }
		inline F64 b() const { return b_; }
		void setR(F64 _r);
		void setG(F64 _g);
		void setB(F64 _b);

		void normalize();
		rgbColor operator+(const rgbColor c1) const;
		rgbColor operator*(const rgbColor c1) const;
	};

	rgbColor operator*(F64 mult, rgbColor rhs);
	rgbColor operator*(rgbColor lhs, F64 mult);
	
	void pc(rgbColor c);
		
	/*class rgbaColor : public rgbColor
	{
		F64 a_;
	public:
		rgbaColor(F64 _r, F64 _g, F64 _b, F64 _a, bool isZeroOne);
		F64 a() { return a_; }
		void setA(F64 _a);

		rgbaColor operator+(const rgbaColor c1) const;
		rgbaColor operator*(const rgbaColor c1) const;
	};*/
}

#endif
