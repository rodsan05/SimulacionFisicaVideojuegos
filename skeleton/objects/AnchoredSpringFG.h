#pragma once
#include "SpringForceGenerator.h"
class AnchoredSpringFG : public SpringForceGenerator
{
public:
	AnchoredSpringFG(double k_, double resting_, const Vector3& anchor_pos);

	~AnchoredSpringFG();
};