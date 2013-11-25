#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(void)
{
}

ParticleSystem::~ParticleSystem(void)
{
	for (int i = 0; i < m_particles.size(); i++)
		delete m_particles[i];
	for (int i = 0; i < m_forces.size(); i++)
		delete m_forces[i];
}

Vecd ParticleSystem::getState() const {

	Vecd state(this->getDimension());
	for (int i = 0; i < m_particles.size(); i++) {
		state[6*i    ] = m_particles[i]->pos[0];
		state[6*i + 1] = m_particles[i]->pos[1];
		state[6*i + 2] = m_particles[i]->pos[2];
		state[6*i + 3] = m_particles[i]->vel[0];
		state[6*i + 4] = m_particles[i]->vel[1];
		state[6*i + 5] = m_particles[i]->vel[2];
	}	
	return state;

}

Vecd ParticleSystem::getPreviousState() const {

	Vecd state(this->getDimension());
	for (int i = 0; i < m_particles.size(); i++) {
		state[6*i    ] = m_particles[i]->prevPos[0];
		state[6*i + 1] = m_particles[i]->prevPos[1];
		state[6*i + 2] = m_particles[i]->prevPos[2];
		state[6*i + 3] = m_particles[i]->prevVel[0];
		state[6*i + 4] = m_particles[i]->prevVel[1];
		state[6*i + 5] = m_particles[i]->prevVel[2];
	}	
	return state;

}

Vecd ParticleSystem::getDerivative() const {

	// clear force accumulators
	for (int i = 0; i < m_particles.size(); i++) {
		m_particles[i]->force = Vec3d(0.0, 0.0, 0.0);
	}

	// apply forces
	for (int i = 0; i < m_forces.size(); i++) {
		m_forces[i]->apply();
	}

	// compute derivatives
	Vecd deriv(this->getDimension());
	for (int i = 0; i < m_particles.size(); i++) {
		deriv[6*i    ] = m_particles[i]->vel[0];
		deriv[6*i + 1] = m_particles[i]->vel[1];
		deriv[6*i + 2] = m_particles[i]->vel[2];
		deriv[6*i + 3] = m_particles[i]->force[0]/m_particles[i]->mass;
		deriv[6*i + 4] = m_particles[i]->force[1]/m_particles[i]->mass;
		deriv[6*i + 5] = m_particles[i]->force[2]/m_particles[i]->mass;
	}
	
	return deriv;

}

Vecd ParticleSystem::getSecondDerivative() const {

	// clear force accumulators
	for (int i = 0; i < m_particles.size(); i++) {
		m_particles[i]->force = Vec3d(0.0, 0.0, 0.0);
	}

	// apply forces
	for (int i = 0; i < m_forces.size(); i++) {
		m_forces[i]->apply();
	}

	// compute derivatives
	Vecd deriv(this->getDimension());
	for (int i = 0; i < m_particles.size(); i++) {
		deriv[6*i + 0] = m_particles[i]->force[0]/m_particles[i]->mass;
		deriv[6*i + 1] = m_particles[i]->force[1]/m_particles[i]->mass;
		deriv[6*i + 2] = m_particles[i]->force[2]/m_particles[i]->mass;
		deriv[6*i + 3] = 0;
		deriv[6*i + 4] = 0;
		deriv[6*i + 5] = 0;
	}
	
	return deriv;

}

void ParticleSystem::setState(const Vecd& state) {

	for (int i = 0; i < m_particles.size(); i++) {
		m_particles[i]->pos[0]  = state[6*i    ];
		m_particles[i]->pos[1]  = state[6*i + 1];
		m_particles[i]->pos[2]  = state[6*i + 2];
		m_particles[i]->vel[0]  = state[6*i + 3];
		m_particles[i]->vel[1]  = state[6*i + 4];
		m_particles[i]->vel[2]  = state[6*i + 5];
	}

}

void ParticleSystem::setPreviousState(const Vecd& state) {

	for (int i = 0; i < m_particles.size(); i++) {
		m_particles[i]->prevPos[0]  = state[6*i    ];
		m_particles[i]->prevPos[1]  = state[6*i + 1];
		m_particles[i]->prevPos[2]  = state[6*i + 2];
		m_particles[i]->prevVel[0]  = state[6*i + 3];
		m_particles[i]->prevVel[1]  = state[6*i + 4];
		m_particles[i]->prevVel[2]  = state[6*i + 5];
	}

}
