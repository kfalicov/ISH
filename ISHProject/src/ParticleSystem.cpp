#include "ParticleSystem.h"

Emitter::Emitter() 
{
	particles = vector<Particle>();
	Start();
}

Emitter::~Emitter()
{

}

void Emitter::Update()
{
	if (running)
	{
		for (std::vector<Particle>::iterator it = particles.begin(); it != particles.end(); ++it)
		{
			if (it->isDead) {
				particles.erase(it);
			}
			else
			{
				it->Update();
			}
		}
		if (spawning) {
			particles.push_back(Particle());
		}
	}
}

void Emitter::Render()
{
	for(Particle p: particles)
	{
		SDL_Rect offset;
		offset.x = p.pos[0];
		offset.y = p.pos[1];
		SDL_Surface* type;// = SpriteSheet[p.type];
		//TODO store a single spritesheet in Emitter, and 
		//access which Surface based on the particle's stored type.
		//This avoids storing a Surface for every particle.

		SDL_BlitSurface(type, NULL, surface, &offset);
	}
	
}

void Emitter::Unpause()
{
	running = true;
}

void Emitter::Start()
{
	spawning = true;
}

void Emitter::Stop()
{
	spawning = false;
}

void Emitter::Pause()
{
	running = false;
}

Particle::Particle(int id = 0, int lifespan = 50)
{
	vel = 5;
	dir = vec2::randV();
	this->lifespan = lifespan;
	this->id = id;
}
Particle::~Particle()
{

}

void Particle::Update()
{	
	lifespan--;
	if (lifespan <= 0)
	{
		isDead = true;
	}
	else
	{
		pos += dir * vel;
	}
}