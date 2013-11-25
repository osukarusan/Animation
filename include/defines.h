#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <vl/VLd.h>
#include <vl/VLf.h>

#define PS_INFINITE		1e32

inline double random01() {
	return double(rand()%32768)/32768.0;
}

#endif

