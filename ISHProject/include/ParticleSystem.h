#pragma once
#include "Util.h"
#include "SDL.h"
#include <vector>
#include "AssetHandler.h"

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
	Emitter(vec2 pos); //creates and starts the particle emitter
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
	vec2 pos; //the position of the Emitter in the world
	vector<Particle> particles;
	SDL_Surface* surface;

};