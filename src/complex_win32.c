#include "complex_win32.h"

_Fcomplex fcadd(const _Fcomplex x, const _Fcomplex y) {
	_Fcomplex r = x;
	r._Val[0] += y._Val[0];
	r._Val[1] += y._Val[1];
	return r;
}

_Fcomplex fcaddr(const _Fcomplex x, const float y) {
	_Fcomplex r = x;
	r._Val[0] += y;
	return r;
}
