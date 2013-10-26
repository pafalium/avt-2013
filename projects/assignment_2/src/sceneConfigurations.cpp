//sceneConfigurations.cpp

#include "sceneConfigurations.h"
#include <cmath>

SceneConfiguration::ModelConfig::ModelConfig(std::string name, Vector3f loc, GLfloat rot)
: m_name(name), m_location(loc), m_rotation(rot)
{}

SceneConfiguration::ModelConfig::ModelConfig()
: m_name(), m_location(0, 0, 0), m_rotation(0)
{}

Matrix4 SceneConfiguration::ModelConfig::getModelMatrix()
{
	Matrix4 rotation = Matrices::axisRotate(0, 0, 1, m_rotation);
	Matrix4 translation = Matrices::translate(m_location.x(), m_location.y(), m_location.z());

	return translation * rotation;
}

const Vector3f &SceneConfiguration::ModelConfig::location() const
{
	return m_location;
}
const GLfloat &SceneConfiguration::ModelConfig::rotation() const
{
	return m_rotation;
}

SceneConfiguration::WorldObject::WorldObject(SceneConfiguration::ModelConfig *config, RenderModel *renderModel)
: m_config(config), m_renderModel(renderModel)
{}

Matrix4 SceneConfiguration::WorldObject::getModelMatrix()
{
	return m_config->getModelMatrix();
}

void SceneConfiguration::WorldObject::drawRenderModel()
{
	m_renderModel->drawModel();
}

void StaticSceneConfiguration::addWorldObject(std::string name, RenderModel *renderModel, Vector3f loc, GLfloat rot)
{
	m_objects[name] = ModelConfig(name, loc, rot);
	m_renderModels[name] = renderModel;
}

SceneConfiguration::WorldObject StaticSceneConfiguration::getWorldObject(std::string name)
{
	ModelConfig &config = m_objects[name];
	RenderModel *model = m_renderModels[name];

	return WorldObject(&config, model);
}


TwoSceneLerpConfig::TwoSceneLerpConfig(SceneConfiguration *startConfig, SceneConfiguration *endConfig, float durationMsecs)
: m_startConfig(startConfig), m_endConfig(endConfig), m_durationMSecs(durationMsecs), m_currentMSecs(0), m_retConf()
{
	if (m_durationMSecs == 0)
		m_durationMSecs = 1.0;
}

void TwoSceneLerpConfig::incrCurrentMScecs(float msecs)
{
	float increment = std::abs(msecs);
	m_currentMSecs += increment;

	if (m_currentMSecs > m_durationMSecs)
		m_currentMSecs = m_durationMSecs;
}

void TwoSceneLerpConfig::resetCurrentMSecs()
{
	m_currentMSecs = 0;
}

//FIXME BAD HACK
SceneConfiguration::WorldObject TwoSceneLerpConfig::getWorldObject(std::string objectName)
{
	WorldObject startObject = m_startConfig->getWorldObject(objectName);
	WorldObject endObject = m_endConfig->getWorldObject(objectName);

	ModelConfig *startMConf = startObject.m_config, *endMConf = endObject.m_config;

	float lerpFactor = m_currentMSecs / m_durationMSecs;

	Vector3f locLerp = startMConf->location() + (endMConf->location() - startMConf->location())*lerpFactor;
	GLfloat rotLerp = startMConf->rotation() + (endMConf->rotation() - startMConf->rotation())*lerpFactor;

	m_retConf.m_location = locLerp;
	m_retConf.m_rotation = rotLerp;
	m_retConf.m_name = objectName;

	return WorldObject(&m_retConf, startObject.m_renderModel);
}