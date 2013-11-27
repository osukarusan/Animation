#ifndef MODEL_H
#define MODEL_H

#include "cal3d/cal3d.h"
#include "GL/glut.h"
#include "defines.h"


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
	float getModelScale();
	void  getBoundingBox(Vec3f& bmin, Vec3f& bmax);
	
	void  setLodLevel(float lodLevel);
	void  setMotionBlend(float *pMotionBlend, float delay);
	void  setState(int state, float delay);
	void  setModelScale(float s);

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
	Vec3f			 m_bbMin, m_bbMax;

	float m_motionBlend[3];
	float m_modelScale;
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

inline float Model::getModelScale() {
	return m_modelScale;
}

inline void Model::getBoundingBox(Vec3f& bmin, Vec3f& bmax) {
	bmin[0] = m_bbMin[0];
	bmin[1] = m_bbMin[1];
	bmin[2] = m_bbMin[2];
	bmax[0] = m_bbMax[0];
	bmax[1] = m_bbMax[1];
	bmax[2] = m_bbMax[2];
}

inline void Model::setCoreModel(CalCoreModel* coremodel) {
	m_calCoreModel = coremodel;
}

inline void Model::setAnimationIds(const std::vector<int>& ids) {
	m_animationId = ids;
}

inline void Model::setModelScale(float s) {
	m_modelScale = s;
}


#endif
