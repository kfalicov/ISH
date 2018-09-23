#pragma once
#include "Util.h"
#include "SDL.h"
#include <vector>

using std::vector;

class Particle {
public:
	Particle(int id, int lifespan);
	~Particle();
	bool isDead;
	vec2 pos;
	vec2 dir;
	double vel;
	int lifespan;
	int id;
	void Update();
};

class Emitter {
public:
	Emitter(); //creates and starts the particle emitter
	~Emitter();

	void Update();
	void Render();

	void Pause(); //pauses and unpauses particle updates
	void Unpause();

	void Start(); //starts spawning particles
	void Stop(); //stops spawning new particles
private:
	bool running;
	bool spawning;
	vector<Particle> particles;
	SDL_Surface* surface;

};