#pragma once

#include <float.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <assert.h>
#include <stdio.h>      
#include <stdlib.h>    
#include <time.h>  

#if 1
#define SINGLE_PRECISION
typedef float real;
#define REAL_MAX FLT_MAX
#define real_sqrt sqrtf
#define real_abs fabsf
#define real_sin sinf
#define real_cos cosf
#define real_exp expf
#define real_tan tanf
#define real_pow powf
#define real_fmod fmodf
#define real_epsilon FLT_EPSILON
#define R_PI 3.14159f
#define real_round roundf
#define real_acos acos
#define real_atan2 atan2
#define real_floor floor

#else
#define DOUBLE_PRECISION
typedef double real;
#define REAL_MAX DBL_MAX
#define real_sqrt sqrt
#define real_abs fabs
#define real_sin sin
#define real_cos cos
#define real_tan tan
#define real_exp exp
#define real_pow pow
#define real_fmod fmod
#define real_epsilon DBL_EPSILON
#define R_PI 3.14159265358979
#define real_round roundl
#define real_acos acos
#define real_atan2 atan2
#define real_floor floor
#endif


#define DegToRad (R_PI / (real)180)


template <typename T>
T clamp(const T& n, const T& lower, const T& upper) {
	return std::max(lower, std::min(n, upper));
}