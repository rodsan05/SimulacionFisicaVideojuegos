#include "BouyancyForceGenerator.h"

BouyancyForceGenerator::BouyancyForceGenerator() : _density(1000), _volume(1), _depth(1)
{
}

BouyancyForceGenerator::BouyancyForceGenerator(float density, float depth, float volume) : _density(density), _depth(depth), _volume(volume)
{
}

BouyancyForceGenerator::~BouyancyForceGenerator()
{
}

void BouyancyForceGenerator::updateForce(Particle* particle, double duration)
{
	float immersed;
	
	auto _height = particle->getHeight();
	auto h = abs(particle->getPos().y + _depth);

	if (h - _depth > _height * 0.5) { immersed = 0.0; }
	else if (_depth - h > _height * 0.5) { immersed = 1.0; }
	else { immersed = (_depth - h) / _height + 0.5; }

	Vector3 force(0);
	force.y = _density * _volume * immersed * 9.8;

	particle->addForce(force);
}
