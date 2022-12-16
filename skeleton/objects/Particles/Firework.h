#pragma once
#include "Particle.h"
#include <memory>
#include <list>

class ParticleSystem;
class ParticleGenerator;
class Firework : public Particle
{
public:
	Firework(ParticleSystem* pSys, Vector3 Pos, Vector3 Vel, Vector3 a_, float damping_, float scale_, Color color_, float lifeTime_, float lifeDist_, float m_);
	~Firework();

	Particle* clone(ParticleShape _shape = None) const override;

	void onDeath() override;

	void addParticleGen(ParticleGenerator* gen);

protected:
	ParticleSystem* _pSys;

	std::list<std::shared_ptr<ParticleGenerator>> _gens;
};

