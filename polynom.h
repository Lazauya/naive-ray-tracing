#ifndef POLYNOM_H_
#define POLYNOM_H_

#include <cmath>

typedef double F64;

namespace polynom
{
	struct posNeg //for answers involving +-
	{
		F64 pos;
		F64 neg;
	};
	
	F64 bisolve(F64 a, F64 b);
	posNeg trisolve(F64 a, F64 b, F64 c);
}

#endif
