#ifndef _FORCE_
#define _FORCE_

#include <vector>
#include "Particle.h"

class Force
{

public:

	Force(void);
	virtual ~Force(void);

	virtual void apply() = 0;

	void addInfluencedParticle(Particle* p);
	void setInfluencedParticles(const std::vector<Particle*>& particles);
	void clearInfluencedParticles();

	std::vector<Particle*> getInfluencedParticles() const;

protected:

	std::vector<Particle*>	m_particles;	
	
};


inline void Force::addInfluencedParticle(Particle* p) {
	m_particles.push_back(p);
}

inline void Force::setInfluencedParticles(const std::vector<Particle*>& particles) {
	m_particles = particles;
}

inline void Force::clearInfluencedParticles() {
	m_particles.clear();
}

inline std::vector<Particle*> Force::getInfluencedParticles() const {
	return m_particles;
}

#endif