#include "ParticleSystem.h"

Emitter::Emitter(vec2 pos = vec2(0,0)) //optional: specify the point particles are centered on
{
	//TODO specify surface size
	surface->w = 10;
	surface->h = 10;
	this->pos = pos-vec2(surface->w/2, surface->h/2);
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

void Emitter::Render() //renders particles to the emitter's surface
{
	for(Particle p: particles)
	{
		SDL_Rect offset;
		offset.x = p.pos[0] + surface->w / 2;
		offset.y = p.pos[1] + surface->h / 2;
		Sprite* spr = AssetHandler::Instance()->GetSprite("Assets/AnimTest.png", p.id);
		SDL_BlitSurface(spr->spriteSheet, &(spr->srcRect), surface, &offset);
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
	pos = vec2(0, 0);
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