//scene.cpp

#include "scene.h"

////////////////////////////////////////////////////////////////// SCENE
void Scene::drawScene(ShaderProgram *program)
{
	for (sceneObjectsIterator it = m_sceneObjects.begin();
		it != m_sceneObjects.end(); 
		++it)
	{
		it->second->drawSceneObject(program);
	}
}

void Scene::addObject(SceneObject *object)
{
	m_sceneObjects[object->name()] = object;
}

SceneObject *Scene::getObject(std::string objectName)
{
	sceneObjectsIterator it = m_sceneObjects.find(objectName);
	if (it == m_sceneObjects.end())
		return 0;
	return it->second;
}

/////////////////////////////////////////////////////////////////// SCENEOBJECT
SceneObject::SceneObject(std::string name, RenderModel *model, Vector3f loc, Vector3f col, Quaternion rot)
: m_name(name), m_renderModel(model), m_location(loc), m_color(col), m_rotation(rot)
{}

SceneObject::SceneObject(std::string name, RenderModel *model, Vector3f loc, Vector3f col)
: SceneObject(name, model, loc, col, qFromAngleAxis(0.0f, Vector3f(1.0f,0.0f,0.0f)))
{}

void SceneObject::drawSceneObject(ShaderProgram *program)
{
	Matrix4 modelMatrix = Matrices::translate(m_location) * Matrices::qGLMatrix(m_rotation);
	program->sendUniformMat4(Uniforms::MATRIX,modelMatrix);
	program->sendUniformVec3(Uniforms::COLOR, m_color);
	m_renderModel->drawModel();
}

void SceneObject::composeRotation(const Quaternion &rot)
{
	m_rotation = qMultiply(rot, m_rotation);
}

const std::string &SceneObject::name() const
{
	return m_name;
}
const Vector3f &SceneObject::location() const
{
	return m_location;
}
const Vector3f &SceneObject::color() const
{
	return m_color;
}
const Quaternion &SceneObject::rotation() const
{
	return m_rotation;
}
void SceneObject::location(const Vector3f &loc)
{
	m_location = loc;
}
void SceneObject::rotation(const Quaternion &rot)
{
	m_rotation = rot;
}
