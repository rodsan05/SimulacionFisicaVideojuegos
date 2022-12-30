#include "Particle.h"

Particle::Particle() : vel(), damping(), a(), pose(), color(), renderItem(), lifeTime(), m(), scale(), iniTime(), lifeDistance(), _type(Default)
{
}

Particle::Particle(Vector3 Pos, Vector3 Vel, Vector3 a_, float damping_, float scale_, Color color_, float lifeTime_, float lifeDist_, float m_, ParticleShape shape_, Vector3 dimensions_)
{
	force = Vector3(0);

	scale = scale_;
	vel = Vel;

	damping = damping_;
	a = a_;

	pose = physx::PxTransform(Pos);

	color = color_;

	shape = shape_;

	dimensions = dimensions_;

	partShape = createShape(shape, scale, dimensions);

	renderItem = new RenderItem(partShape, &pose, color);

	m = m_;

	lifeTime = lifeTime_;
	lifeDistance = lifeDist_;

	initPos = Pos;
	iniTime = glutGet(GLUT_ELAPSED_TIME);

	_type = Default;
}

Particle::~Particle()
{
	DeregisterRenderItem(renderItem);
}

bool Particle::isAlive()
{
	return alive;
}
void Particle::setAlive(bool set)
{
	alive = set;
}

void Particle::integrate(double t)
{
	if (_static) return;

	float inverse_mass = 1.0 / m;

	if (inverse_mass <= 0.0f) return;

	if (!eulerSemiImplicito)
	{
		pose.p += vel * t;

		Vector3 totalAccel = a;
		totalAccel += force * inverse_mass;

		//update linear vel
		vel += totalAccel * t;
		//damping
		vel *= powf(damping, t);
	}

	else
	{
		Vector3 totalAccel = a;
		totalAccel += force * inverse_mass;

		//update linear vel
		vel += totalAccel * t;
		//damping
		vel *= powf(damping, t);

		pose.p += vel * t;
	}

	clearForce();

	//anteriores practicas
	//// p(i+1) = p(i) + v * t
	//pose.p = pose.p + vel * t;

	//// vf = v0*d^t + a*t
	//vel = vel * pow(damping, t) + a * t;

	//comprueba si debe morir por tiempo
	if (lifeTime != -1)
	{
		auto time = glutGet(GLUT_ELAPSED_TIME);

		auto elapsedTime = time - iniTime;

		if (elapsedTime > lifeTime)
		{
			alive = false;
		}
	}

	//comprueba si debe morir por distancia
	if (lifeDistance != -1)
	{
		auto distance = (pose.p - initPos).magnitudeSquared();

		if (distance > pow(lifeDistance, 2))
		{
			alive = false;
		}
	}
}

Particle* Particle::clone(ParticleShape _shape) const
{
	if (shape != None)
		return new Particle(pose.p, vel, a, damping, scale, color, lifeTime, lifeDistance, m, _shape);

	return new Particle(pose.p, vel, a, damping, scale, color, lifeTime, lifeDistance, m, shape);
}

void Particle::setPos(Vector3 pos_)
{
	pose.p = pos_;
	initPos = pos_;
}

void Particle::setVel(Vector3 vel_)
{
	vel = vel_;
}

void Particle::setLifeTime(float time)
{
	lifeTime = time;
}

void Particle::setLifeDist(float dist)
{
	lifeDistance = dist;
}

void Particle::setColor(Color c)
{
	color = c;

	DeregisterRenderItem(renderItem);
	delete renderItem;
	renderItem = new RenderItem(partShape, &pose, color);
}

void Particle::setScale(float s)
{
	scale = s;

	partShape = createShape(shape, scale, dimensions);

	DeregisterRenderItem(renderItem);
	delete renderItem;
	renderItem = new RenderItem(partShape, &pose, color);
}

void Particle::setMass(float mass)
{
	m = mass;
}

physx::PxShape* Particle::createShape(ParticleShape shape_, float scale_, Vector3 dimensions)
{
	physx::PxShape* partShape_;

	switch (shape_)
	{
	case Sphere:
		partShape_ = CreateShape(physx::PxSphereGeometry(scale_));
		volume = (4 / 3) * physx::PxPi * pow(scale_, 3);
		break;
	case Cube:
		partShape_ = CreateShape(physx::PxBoxGeometry(scale_, scale_, scale_));
		volume = pow(scale_, 3);
		break;
	case Capsule:
		partShape_ = CreateShape(physx::PxCapsuleGeometry(scale_ / 2, scale_));
		volume = (physx::PxPi * pow(scale_ / 2, 2) * scale_ * 2) + ((4 / 3) * physx::PxPi * pow(scale / 2, 3)); //cilinder area + sphere area
		break;
	case Plane:
		partShape_ = CreateShape(physx::PxBoxGeometry(scale_, 0.5, scale_));
		volume = pow(scale_, 2) * 0.5;
		break;
	case Prism:
		partShape_ = CreateShape(physx::PxBoxGeometry(scale_ * dimensions.x, scale_ * dimensions.y, scale_ * dimensions.z));
		volume = scale_ * dimensions.x + scale_ * dimensions.y + scale_ * dimensions.z;
		break;
	default:
		partShape_ = CreateShape(physx::PxSphereGeometry(scale_));
		volume = (4 / 3) * physx::PxPi * pow(scale_, 3);
		break;
	}

	return partShape_;
}

void Particle::deregisterRender()
{
	DeregisterRenderItem(renderItem);
}

void Particle::clearForce()
{
	force *= 0;
}

void Particle::addForce(const Vector3& f)
{
	force += f;
}

void Particle::setParticle(Vector3 Pos, Vector3 Vel, Vector3 a_, float damping_, float scale_, Color color_, float lifeTime_, float lifeDist_, float m_)
{
	force = Vector3(0);

	scale = scale_;
	vel = Vel;

	damping = damping_;
	a = a_;

	pose = physx::PxTransform(Pos);

	color = color_;

	renderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(scale_)), &pose, color);

	m = m_;

	lifeTime = lifeTime_;
	lifeDistance = lifeDist_;

	initPos = Pos;
	iniTime = glutGet(GLUT_ELAPSED_TIME);
}
