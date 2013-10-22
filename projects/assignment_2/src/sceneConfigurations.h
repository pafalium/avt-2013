//sceneConfigurations.h

#ifndef __SCENE_CONFIGURATIONS_H__
#define __SCENE_CONFIGURATIONS_H__

#include <map>
#include <GL/glew.h>
#include "RenderModel.h"
#include "vector3f.h"
#include "matrices.h"

class SceneConfiguration {
	struct ModelConfig {
		std::string m_name;
		Vector3f m_location;
		GLfloat m_rotation;

		ModelConfig();
		ModelConfig(std::string name, Vector3f loc, GLfloat rot);
		Matrix4 getModelMatrix();
	};


	std::map<std::string, ModelConfig> m_objects;
	std::map<std::string, RenderModel *> m_renderModels;

public:
	class WorldObject {
		ModelConfig *m_config;
		RenderModel *m_renderModel;

	public:
		WorldObject(ModelConfig *config, RenderModel *renderModel);
		Matrix4 getModelMatrix();
		void drawRenderModel();
	};

	void addWorldObject(std::string objectName, RenderModel *renderModel, Vector3f loc, GLfloat rot);
	WorldObject getWorldObject(std::string objectName);
};

#endif