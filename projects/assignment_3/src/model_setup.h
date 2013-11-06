//model_setup.h

#ifndef __MODEL_SETUP_H__
#define __MODEL_SETUP_H__

#include "RenderModel.h"
#include "sceneConfigurations.h"

namespace Models {
	extern RenderModel 
		BigTri1Model,
		BigTri2Model,
		MedTriModel,
		SmallTri1Model,
		SmallTri2Model,
		QuadModel,
		SquareModel,
		BackPlaneModel;

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

#endif