
#ifndef __MODEL_SETUP_H__
#define __MODEL_SETUP_H__

#include "RenderModel.h"

namespace Models {
	extern RenderModel 
		BigTriModel,
		MedTriModel,
		SmallTriModel,
		QuadModel,
		SquareModel;

	void setupModels();
	void cleanupModels();
}

#endif