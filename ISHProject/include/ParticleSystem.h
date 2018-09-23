#pragma once
#include "Util.h"
#include <vector>

using std::vector;

class Particle {
public:
	Particle();
	~Particle();
	bool isDead;
	vec2 pos;
	vec2 dir;
	double vel;
	int lifespan;
	void Update();
};

class Emitter {
public:
	Emitter();
	~Emitter();

	void Update();
	void Render();
private:
	vector<Particle> particles;
};