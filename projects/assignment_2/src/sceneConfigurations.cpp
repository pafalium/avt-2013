//sceneConfigurations.cpp

#include "sceneConfigurations.h"

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

void SceneConfiguration::addWorldObject(std::string name, RenderModel *renderModel, Vector3f loc, GLfloat rot)
{
	m_objects[name] = ModelConfig(name, loc, rot);
	m_renderModels[name] = renderModel;
}

SceneConfiguration::WorldObject SceneConfiguration::getWorldObject(std::string name)
{
	ModelConfig &config = m_objects[name];
	RenderModel *model = m_renderModels[name];

	return WorldObject(&config, model);
}