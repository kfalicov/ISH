#pragma once

#include <iostream>
#include <stdlib.h>

// min-max macros
#define MIN(A,B) ((A) < (B) ? (A) : (B))
#define MAX(A,B) ((A) > (B) ? (A) : (B))

// error handling macro
#define V_ERROR(E) { std::cerr << E; exit(1); }

class vec2;

enum { VX, VY, VZ, VW };		    // axes

class vec2
{
protected:
	double n[2];
public:
	// Constructors
	vec2();
	vec2(const double x, const double y);
	vec2(const double d);
	vec2(const vec2& v);			// copy constructor

	// Assignment operators
	vec2& operator	= (const vec2& v);	// assignment of a vec2
	vec2& operator += (const vec2& v);	// incrementation by a vec2
	vec2& operator -= (const vec2& v);	// decrementation by a vec2
	vec2& operator *= (const double d);	// multiplication by a constant
	vec2& operator /= (const double d);	// division by a constant
	double& operator [] (int i);		// indexing

	// special functions
	double length();			// length of a vec2
	double length2();			// squared length of a vec2
	vec2& normalize();			// normalize a vec2

	// friends
	friend vec2 operator - (const vec2& v);			    // -v1
	friend vec2 operator + (const vec2& a, const vec2& b);	    // v1 + v2
	friend vec2 operator - (const vec2& a, const vec2& b);	    // v1 - v2
	friend vec2 operator * (const vec2& a, const double d);	    // v1 * 3.0
	friend vec2 operator * (const double d, const vec2& a);	    // 3.0 * v1
	friend double operator * (const vec2& a, const vec2& b);    // dot product
	friend vec2 operator / (const vec2& a, const double d);	    // v1 / 3.0
	friend int operator == (const vec2& a, const vec2& b);	    // v1 == v2 ?
	friend int operator != (const vec2& a, const vec2& b);	    // v1 != v2 ?
	friend vec2 minv(const vec2& a, const vec2& b);		    // min(v1, v2)
	friend vec2 maxv(const vec2& a, const vec2& b);		    // max(v1, v2)
	friend vec2 prod(const vec2& a, const vec2& b);		    // term by term *
};

static double lerp(double a, double b, double t) {
	return a * (1 - t) + b * t;
}