#pragma once
#include <iostream>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "Util.h"

#ifndef COMPASS_VECTOR
//(0, -1)
vec2 vec2::NORTH = vec2(0, -1);
//(1, 0)
vec2 vec2::EAST = vec2(1, 0);
//(0, 1)
vec2 vec2::SOUTH = vec2(0, 1);
//(-1, 0)
vec2 vec2::WEST = vec2(-1, 0);
#endif // !COMPASS_VECTOR

/****************************************************************
*								*
*		    vec2 Member functions			*
*								*
****************************************************************/

// CONSTRUCTORS

vec2::vec2() { n[VX] = 0; n[VY] = 0; }
vec2::vec2(const double x, const double y) { n[VX] = x; n[VY] = y; }
vec2::vec2(const double d) { n[VX] = n[VY] = d; }
vec2::vec2(const vec2& v) { n[VX] = v.n[VX]; n[VY] = v.n[VY]; }


// ASSIGNMENT OPERATORS

vec2& vec2::operator = (const vec2& v) { n[VX] = v.n[VX]; n[VY] = v.n[VY]; return *this; }
vec2& vec2::operator += (const vec2& v) { n[VX] += v.n[VX]; n[VY] += v.n[VY]; return *this; }
vec2& vec2::operator -= (const vec2& v) { n[VX] -= v.n[VX]; n[VY] -= v.n[VY]; return *this; }
vec2& vec2::operator *= (const double d) { n[VX] *= d; n[VY] *= d; return *this; }
vec2& vec2::operator /= (const double d) { double d_inv = 1. / d; n[VX] *= d_inv; n[VY] *= d_inv; return *this; }
double vec2::operator [] (int i) const {
	if (i < VX || i > VY)
		V_ERROR("vec2 [] operator: illegal access; index = " << i << '\n')
		return n[i];
}
double& vec2::operator [] (int i) {
	if (i < VX || i > VY)
		V_ERROR("vec2 [] operator: illegal access; index = " << i << '\n')
		return n[i];
}


// SPECIAL FUNCTIONS

double vec2::length() { return sqrt(length2()); }
double vec2::length2() { return n[VX] * n[VX] + n[VY] * n[VY]; }
// it is up to caller to avoid divide-by-zero
vec2& vec2::normalize() { *this /= length(); return *this; }


// FRIENDS

vec2 operator - (const vec2& a) { return vec2(-a.n[VX], -a.n[VY]); }
vec2 operator + (const vec2& a, const vec2& b) { return vec2(a.n[VX] + b.n[VX], a.n[VY] + b.n[VY]); }
vec2 operator - (const vec2& a, const vec2& b) { return vec2(a.n[VX] - b.n[VX], a.n[VY] - b.n[VY]); }
vec2 operator * (const vec2& a, const double d) { return vec2(d*a.n[VX], d*a.n[VY]); }
vec2 operator * (const double d, const vec2& a) { return a * d; }
double operator * (const vec2& a, const vec2& b) { return (a.n[VX] * b.n[VX] + a.n[VY] * b.n[VY]); }
vec2 operator / (const vec2& a, const double d) { double d_inv = 1. / d; return vec2(a.n[VX] * d_inv, a.n[VY] * d_inv); }
int operator == (const vec2& a, const vec2& b) { return (a.n[VX] == b.n[VX]) && (a.n[VY] == b.n[VY]); }
int operator != (const vec2& a, const vec2& b) { return !(a == b); }
vec2 minv(const vec2& a, const vec2& b) { return vec2(MIN(a.n[VX], b.n[VX]), MIN(a.n[VY], b.n[VY])); }
vec2 maxv(const vec2& a, const vec2& b) { return vec2(MAX(a.n[VX], b.n[VX]), MAX(a.n[VY], b.n[VY])); }
vec2 prod(const vec2& a, const vec2& b) { return vec2(a.n[VX] * b.n[VX], a.n[VY] * b.n[VY]); }

std::ostream & operator<<(std::ostream & os, const vec2 & a)
{
	os << '(' << a[0] << ',' << a[1] << ')';
	return os;
}
/****************************************************************
*								*
*		    Noise Member functions			*
*								*
****************************************************************/

static double Lerp(double a, double b, double t) { return a + t * (b - a); }

noise::noise() { seed = 0; }
void noise::SetSeed() {}