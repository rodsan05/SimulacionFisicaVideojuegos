#include "Firework.h"
#include "ParticleSystem.h"

Firework::Firework(ParticleSystem* pSys, Vector3 Pos, Vector3 Vel, Vector3 a_, float dumping_, float scale_, Color color_, float lifeTime_, float lifeDist_, float m_) :
	Particle(Pos, Vel, a_, dumping_, scale_, color_, lifeTime_, lifeDist_, m)
{
	_pSys = pSys;
}

Firework::~Firework()
{
}

Particle* Firework::clone() const
{
	Firework* f = new Firework(_pSys, pose.p, vel, a, dumping, scale, color, lifeTime, lifeDistance, m);
	
	for (auto g : _gens)
		f->addParticleGen(g.get());

	return f;
}

void Firework::onDeath()
{
	std::list<Particle*> particles;

	for (auto gen : _gens) 
	{
		gen->setPos(pose.p);
		auto list = gen->generateParticles();

		particles.insert(particles.end(), list.begin(), list.end());
	}

	_pSys->appendFireworks(particles);
}

void Firework::addParticleGen(ParticleGenerator* gen)
{
	_gens.push_back(std::shared_ptr<ParticleGenerator>(gen));
}
