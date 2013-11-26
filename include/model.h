#ifndef MODEL_H
#define MODEL_H

#include "cal3d/cal3d.h"
#include "GL/glut.h"


class Model
{

public:
  static const int STATE_IDLE;
  static const int STATE_FANCY;
  static const int STATE_MOTION;

public:
	Model();
	virtual ~Model();

	void  setCoreModel(CalCoreModel* coremodel);
	void  setAnimationIds(const std::vector<int>& animationIds);

	float getLodLevel();
	void  getMotionBlend(float *pMotionBlend);
	int   getState();
	
	void  setLodLevel(float lodLevel);
	void  setMotionBlend(float *pMotionBlend, float delay);
	void  setState(int state, float delay);

	bool  onInit();
	void  onShutdown();
	void  onUpdate(float elapsedSeconds);
	void  onRender();

	void  executeAction(int action);

protected:
	
	void renderMesh(bool bWireframe, bool bLight);
	void renderSkeleton();
	void renderBoundingBox();

protected:

	int m_state;
	
	CalModel*        m_calModel;
	CalCoreModel*    m_calCoreModel;
	std::vector<int> m_animationId;

	float m_motionBlend[3];
	float m_lodLevel;

};



inline float Model::getLodLevel() {
    return m_lodLevel;
}

inline void Model::getMotionBlend(float *pMotionBlend) {
    pMotionBlend[0] = m_motionBlend[0];
    pMotionBlend[1] = m_motionBlend[1];
    pMotionBlend[2] = m_motionBlend[2];
}

inline int Model::getState() {
    return m_state;
}


inline void Model::setCoreModel(CalCoreModel* coremodel) {
	m_calCoreModel = coremodel;
}

inline void Model::setAnimationIds(const std::vector<int>& ids) {
	m_animationId = ids;
}


#endif
