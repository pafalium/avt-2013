//sceneConfigurations.h

#ifndef __SCENE_CONFIGURATIONS_H__
#define __SCENE_CONFIGURATIONS_H__

#include <map>
#include <GL/glew.h>
#include "RenderModel.h"
#include "vector3f.h"
#include "matrices.h"

class TwoSceneLerpConfig;

//Super class for scene configurations.
//A client can ask for a specific WorldObject though
//getWorldObject.
class SceneConfiguration {
protected:
	struct ModelConfig {
		std::string m_name;
		Vector3f m_location;
		GLfloat m_rotation;

		ModelConfig();
		ModelConfig(std::string name, Vector3f loc, GLfloat rot);
		Matrix4 getModelMatrix();
		const Vector3f &location() const;
		const GLfloat &rotation() const;
	};
public:
	class WorldObject {
		ModelConfig *m_config;
		RenderModel *m_renderModel;

		//FIXME BAD HACK
		friend class TwoSceneLerpConfig;
	public:
		WorldObject(ModelConfig *config, RenderModel *renderModel);
		Matrix4 getModelMatrix();
		void drawRenderModel();
	};

	virtual WorldObject getWorldObject(std::string objectName)=0;
};


//A StaticSceneConfiguration holds the location and rotation of it's
//WorldObjects at a moment in time.
class StaticSceneConfiguration : public SceneConfiguration {
	std::map<std::string, ModelConfig> m_objects;
	std::map<std::string, RenderModel *> m_renderModels;

public:
	void addWorldObject(std::string objectName, RenderModel *renderModel, Vector3f loc, GLfloat rot);
	WorldObject getWorldObject(std::string objectName);
};


class TwoSceneLerpConfig : public SceneConfiguration {
	SceneConfiguration *m_startConfig, *m_endConfig;
	float m_durationMSecs;
	float m_currentMSecs;

	ModelConfig m_retConf;
public:
	TwoSceneLerpConfig(SceneConfiguration *startConfig, SceneConfiguration *endConfig, float durationMSecs);
	//Returns the WorldObject resulting from the interpolation of those returned
	//by the two configurations.
	//Linear interpolation is done considering the total duration and the current
	//time within this config.
	WorldObject getWorldObject(std::string objectName);
	//Set current time to 0 msecs.
	void resetCurrentMSecs();
	//Increment current time by the absolute value of the argument.
	//When the current time exceeds total duration-> current time = total duration.
	void incrCurrentMScecs(float msecs);
};

#endif