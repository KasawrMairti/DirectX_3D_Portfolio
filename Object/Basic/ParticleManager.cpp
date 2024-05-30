#include "Framework.h"
#include "ParticleManager.h"


ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
	for (Particle* particle : particles)
	{
		delete particle;
	}
	particles.clear();
}

void ParticleManager::Update()
{
	for (UINT i = 0; i < particles.size(); i++)
	{
		if (particles[i]->TimeLimit() <= 0)
		{
			delete particles[i];
			particles.erase(particles.begin() + i, particles.begin() + i + 1);
			break;
		}
		else
			particles[i]->Update();
	}
}

void ParticleManager::Render()
{
	for (Particle* particle : particles)
	{
		particle->Render();
	}
}

void ParticleManager::Add(wstring file, float time, Vector3 pos, float scale)
{
	Particle* particle = new Particle(file, time, pos);
	particle->scale *= scale;
	particles.push_back(particle);
}