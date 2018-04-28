#include "polynom.h"

namespace polynom
{
	F64 bisolve(F64 a, F64 b)
	{
		return (-1*b)/a;
	}
	posNeg trisolve(F64 a, F64 b, F64 c)
	{
		posNeg pn;
		if((b*b - 4*a*c) < 0)
		{
			pn.pos = -1;
			pn.neg = -1;
			return pn;
		}
		pn.pos = (-1*b + sqrt(b*b - 4*a*c))/(2*a);
		pn.neg = (-1*b - sqrt(b*b - 4*a*c))/(2*a);
		return pn;
	}
}
