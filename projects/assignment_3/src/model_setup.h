//model_setup.h

#ifndef __MODEL_SETUP_H__
#define __MODEL_SETUP_H__

#include "RenderModel.h"
#include "scene.h"
#include "animation.h"
#include "sceneConfigurations.h"

class ModelLoader {
	struct Index {
		unsigned int v, vn, vt;
	};
	std::vector<Vertex> m_vertices, m_vertexData;
	std::vector<Normal> m_normals, m_normalData;
	std::vector<TexCoord> m_texCoords, m_texCoordData;
	std::vector<Index> m_indexes;
	
	RenderModel *m_loadedModel;
private:
	void parseString(const std::string &objString);
	void parseLine(const std::string &line);
	void prepareModelData();
	void createRenderModel();

	void parseVertex(std::istringstream &stream);
	void parseNormal(std::istringstream &stream);
	void parseTexCoord(std::istringstream &stream);
	void parseFace(std::istringstream &stream);
	void parseVertexIndexes(const std::string &vertexString);
public:
	RenderModel *loadModel(const std::string &objString);
	void cleanup();
};

namespace Models {
	extern RenderModel 
		*BigTriModel,
		*MedTriModel,
		*SmallTriModel,
		*QuadModel,
		*SquareModel,
		*BackPlaneModel;

	void setupModels();
	void cleanupModels();
}

namespace Scenes {
	namespace ObjectNames {
		extern const std::string BIG_TRI_1;
		extern const std::string BIG_TRI_2;
		extern const std::string MED_TRI_1;
		extern const std::string SML_TRI_1;
		extern const std::string SML_TRI_2;
		extern const std::string SQR;
		extern const std::string QUAD;

		extern const std::vector<std::string> ALL_NAMES;
	}

	extern StaticSceneConfiguration SquareTangramConfig;
	extern StaticSceneConfiguration FigureTangramConfig;
	extern TwoSceneLerpConfig SqrFigTangramConfig;

	void setupTangramConfigs();
}

namespace SingleScene {
	namespace SceneObjects {
		extern SceneObject *BT1, *BT2, *MT1, *ST1, *ST2, *SQR, *QUAD;
		namespace Names {
			extern const std::string BIG_TRI_1;
			extern const std::string BIG_TRI_2;
			extern const std::string MED_TRI_1;
			extern const std::string SML_TRI_1;
			extern const std::string SML_TRI_2;
			extern const std::string SQR;
			extern const std::string QUAD;
		}
	}
	namespace Animations {
		extern Animation *INIT, *MBT1, *MBT2, *MMT1, *MST1, *MST2, *MSQR, *MQUAD;
		namespace Names {
			extern const std::string INIT;
			extern const std::string MBT1;
			extern const std::string MBT2;
			extern const std::string MMT1;
			extern const std::string MST1;
			extern const std::string MST2;
			extern const std::string MSQR;
			extern const std::string MQUAD;
		}
	}

	extern Scene *TangramScene;
	extern AnimationManager *TangramAnimManager;

	void setupSceneObjects();
	void cleanupSceneObjects();
	void setupScene();
	void cleanupScene();
	void setupAnimations();
	void cleanupAnimations();

}

#endif