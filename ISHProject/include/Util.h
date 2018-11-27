// min-max macros
#define MIN(A,B) ((A) < (B) ? (A) : (B))
#define MAX(A,B) ((A) > (B) ? (A) : (B))

// error handling macro
#define V_ERROR(E) { std::cerr << E; exit(1); }

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
	double operator [] (int i) const;
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
	friend std::ostream& operator << (std::ostream& os, const vec2& a);	// for use in cout, requires iostream import

	//compass directions
	static vec2 NORTH;
	static vec2 EAST;
	static vec2 SOUTH;
	static vec2 WEST;
};

class noise
{
protected:
	int seed;
public:
	noise();
	void SetSeed();
};

static double lerp(double a, double b, double t) {
	if (t > 1) {
		return b;
	}
	return a * (1 - t) + b * t;
}

static vec2 lerp(vec2 a, vec2 b, double t) {
	if (t > 1) {
		return b;
	}
	return a * (1 - t) + b * t;
}

class Camera {
public:
	Camera() {};
	Camera(vec2 center, int w, int h) {
		setCenter(center);
		size = vec2(w, h);
	};
	~Camera() {};

	void Update() {
		setCenter(lerp(getCenter(), target, followSpeed));
	};

	void TrackTo(vec2 dest) {
		target = dest;
	};
	vec2 getPos() { return camPosition; };
	vec2 getCenter() { return camPosition + (size / 2); };
	vec2 getSize() { return size; };

	//sets the center of the camera
	void setCenter(vec2 dest) {
		camPosition = dest - (size / 2);
	};

	//sets the top left corner
	void setCorner(vec2 dest) {
		camPosition = dest;
	};

private:
	double followSpeed;
	vec2 camPosition; // The top left of the camera
	vec2 size;
	vec2 target; // Where the camera is moving to over time
};