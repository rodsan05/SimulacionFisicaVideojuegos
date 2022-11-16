#pragma once
#include "WindForceGenerator.h"
class WhirlwindForceGenerator : public WindForceGenerator
{
public:
	WhirlwindForceGenerator(Vector3 orig, float k_, float k1, float k2, float dragToCenter);
	~WhirlwindForceGenerator();

	void updateForce(Particle* particle, double t) override;

protected:
	Vector3 origen;
	float K; //fuerza del torbellino
	float dragToCenterCoef; //coeficiente de arrastre al centro del torbellino
};

