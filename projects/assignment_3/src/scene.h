//scene.h

#pragma once

#include <map>
#include <string>

#include "Vector3f.h"
#include "quaternion.h"

#include "ShaderProgram.h"
#include "RenderModel.h"

#include "uniformsAttribs.h"

class SceneObject;

// A Scene represents a 3d world object configuration.
// The Scene is not responsible for managing the objects' lifecycle it 'stores'.
class Scene {
	// A map containing all the scenes objects.
	// Scene objects are uniquely identified by their name.
	std::map<std::string, SceneObject *> m_sceneObjects;

	typedef std::map<std::string, SceneObject *>::iterator sceneObjectsIterator;
public:
	// Draws all the scenes objects using the given ShaderProgram.
	void drawScene(ShaderProgram *program);
	// Adds the given SceneObject to the scene.
	// If the object is already present or there is an already an object with
	// the same name -> the existing object is replaced. (NOT deleted)
	void addObject(SceneObject *object);
	// Get the SceneObject with the given name.
	// If no object exists with the given name a null pointer is returned.
	SceneObject *getObject(std::string objectName);
};

// A SceneObject represents a renderable Model positioned in world space.
// The SceneObject is not responsible for its RenderModel lifecycle.
class SceneObject {
	// The SceneObject unique name. (within the Scene context)
	std::string m_name;
	// The 3d renderable model of this SceneObject.
	RenderModel *m_renderModel;
	// The object's current location.
	Vector3f m_location;
	// The object's color in RGB color model.
	Vector3f m_color;
	// A unit quaternion representing the object's current rotation.
	Quaternion m_rotation;
public:
	SceneObject(std::string name, RenderModel *model, Vector3f loc, Vector3f col, Quaternion rot);
	// Create a SceneObject with 'identity' rotation.
	SceneObject(std::string name, RenderModel *model, Vector3f loc, Vector3f col);
	// Draw the object using the given ShaderProgram.
	// The program has already been 'activated' and it's only used to set uniforms.
	void drawSceneObject(ShaderProgram *program);

	const std::string &name() const;
	const Vector3f &location() const;
	const Vector3f &color() const;
	const Quaternion &rotation() const;
	void location(const Vector3f &loc);
	void rotation(const Quaternion &rot);
	// Compose the object's rotation so that the given rotation is done
	// after the current rotation.
	void composeRotation(const Quaternion &rot);
};