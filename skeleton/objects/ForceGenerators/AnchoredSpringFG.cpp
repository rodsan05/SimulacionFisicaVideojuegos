#include "AnchoredSpringFG.h"

AnchoredSpringFG::AnchoredSpringFG(double k_, double resting_, const Vector3& anchor_pos) : SpringForceGenerator(k_, resting_, nullptr)
{
	_other = new Particle(anchor_pos, Vector3(0), Vector3(0), 0, 0.2, Color(0.7, 0.7, 0.7, 1), -1, -1, 1e6, Cube);
	_other->setAffectedByGravity(false);
}

AnchoredSpringFG::~AnchoredSpringFG()
{
}
