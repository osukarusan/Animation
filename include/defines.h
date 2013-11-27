#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <vl/VLd.h>
#include <vl/VLf.h>
#ifdef WIN32
#include <windows.h>
#include <stdio.h>
#else
#include <ctime>
#endif

#define PS_INFINITE	1e32
#define M_PI		3.14159265358979323846
#define DEG2RAD		M_PI/180.0
#define RAD2DEG		180.0/M_PI

inline double random01() {
	return double(rand()%32768)/32768.0;
}

inline unsigned long long getTimerFrequency() {
	unsigned long long f;
	#ifdef WIN32	
	QueryPerformanceFrequency((LARGE_INTEGER*)&f);
	#else			
	f = CLOCKS_PER_SEC;
	#endif
	return f;
}

inline unsigned long long getTime() {
	unsigned long long t;
	#ifdef WIN32	
	QueryPerformanceCounter((LARGE_INTEGER*)&t);
	#else			
	t = clock();
	#endif
	return t;
}


#endif

