#ifndef _PARTICLE_SYSTEM_
#define _PARTICLE_SYSTEM_

#include <vector>
#include "Particle.h"
#include "Force.h"
#include "PhysicsSystem.h"
#include "defines.h"

class ParticleSystem : public PhysicsSystem
{

public:

	ParticleSystem(void);
	~ParticleSystem(void);

	virtual int  getDimension()			const;
	virtual Vecd getState()				const;
	virtual Vecd getPreviousState()		const;
	virtual Vecd getDerivative()		const;
	virtual Vecd getSecondDerivative()	const;
	
	virtual void setState(const Vecd& state);
	virtual void setPreviousState(const Vecd& prev);

	Particle* getParticle(unsigned int i) const;
	Force*    getForce   (unsigned int i) const;
	std::vector<Particle*> getParticles() const;
	std::vector<Force*>    getForces()    const;
	int  getNumParticles() const;
	int  getNumForces()    const;
	void addParticle(Particle* p);
	void addForce   (Force* f);
	void clearParticles();
	void clearForces();

protected:

	std::vector<Particle*>	m_particles;
	std::vector<Force*>		m_forces;

};


inline int ParticleSystem::getDimension() const {
	return Particle::Dimension * m_particles.size();
}

inline int ParticleSystem::getNumParticles() const {
	return m_particles.size();
}

inline int ParticleSystem::getNumForces() const {
	return m_forces.size();
}

inline Particle* ParticleSystem::getParticle(unsigned int i) const {
	if (i < m_particles.size()) return m_particles[i];
	return NULL;
}
inline Force* ParticleSystem::getForce(unsigned int i) const {
	if (i < m_forces.size()) return m_forces[i];
	return NULL;
}

inline std::vector<Particle*> ParticleSystem::getParticles() const {
	return m_particles;
}

inline std::vector<Force*> ParticleSystem::getForces() const {
	return m_forces;
}

inline void ParticleSystem::addParticle(Particle *p) {
	p->id = m_particles.size();
	m_particles.push_back(p);
}

inline void ParticleSystem::addForce(Force *f) {
	m_forces.push_back(f);
}

inline void ParticleSystem::clearParticles() {
	for (std::vector<Particle*>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
		delete (*it);
	m_particles.clear();
}

inline void ParticleSystem::clearForces() {
	for (std::vector<Force*>::iterator it = m_forces.begin(); it != m_forces.end(); it++)
		delete (*it);
	m_forces.clear();
}


#endif