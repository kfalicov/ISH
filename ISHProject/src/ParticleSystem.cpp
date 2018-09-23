#include "ParticleSystem.h"

Emitter::Emitter() 
{

}

Emitter::~Emitter()
{

}

void Emitter::Update()
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
}

void Emitter::Render()
{
}

Particle::Particle()
{
	vel = 5;
	dir = vec2::randV();
	int lifespan = 50;
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