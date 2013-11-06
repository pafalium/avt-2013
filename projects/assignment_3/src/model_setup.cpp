//model_setup.cpp

#include <GL/glew.h>
#include <vector>

#include "utils.h"

#include "model_setup.h"

std::vector<Vertex> bigTri1Verts = {
	{ { 1, .3333, 0, 1 }, { .25, .8, 0, 1 } },
	{ { 0, -.6666, 0, 1 }, { .25, .8, 0, 1 } },
	{ { -1, .3333, 0, 1 }, { .25, .8, 0 , 1} }
}, bigTri2Verts = {
	{ { 1, .3333, 0, 1 }, { .8, .11, .11, 1 } },
	{ { 0, -.6666, 0, 1 }, { .8, .11, .11, 1 } },
	{ { -1, .3333, 0, 1 }, { .8, .11, .11, 1 } }
}, medTriVerts = {
	{ {.3333, .6666, 0, 1}, {.8, .2, 0, 1} },
	{ {.3333, -.3333, 0, 1}, { .8, .2, 0, 1 } },
	{ {-.6666, -.3333, 0, 1}, { .8, .2, 0, 1 } }
}, smallTri1Verts = {
	{ {.1666, .5, 0, 1}, {.8, .8, 0, 1} },
	{ {.1666, -.5, 0, 1}, { .8, .8, 0, 1 } },
	{ {-.3333, 0, 0, 1}, { .8, .8, 0, 1 } }
}, smallTri2Verts = {
	{ { .1666, .5, 0, 1 }, { 0, .8, .35, 1 } },
	{ { .1666, -.5, 0, 1 }, { 0, .8, .35, 1 } },
	{ { -.3333, 0, 0, 1 }, { 0, .8, .35, 1 } }
}, quadVerts = {
	{ {.25, .25, 0, 1}, {.8, .65, 0, 1} },
	{ {.25, -.75, 0, 1}, { .8, .65, 0, 1 } },
	{ {-.25, -.25, 0, 1 }, { .8, .65, 0, 1 } },
	{ {-.25, .75, 0, 1 }, { .8, .65, 0, 1 } }
}, squareVerts = {
	{ { 0, .5, 0, 1 }, { 0, 0, .8, 1 } },
	{ { .5, 0, 0, 1 }, { 0, 0, .8, 1 } },
	{ { 0, -.5, 0, 1 }, { 0, 0, .8, 1 } },
	{ { -.5, 0, 0, 1 }, { 0, 0, .8, 1 } },
}, backPlaneVerts = {
	{ { .9, .9, 1, 1 }, { .2, .2, .2, 1 } },
	{ { -.9, .9, 1, 1 }, { .2, .2, .2, 1 } },
	{ { -.9, -.9, 1, 1 }, { .2, .2, .2, 1 } },
	{ { .9, -.9, 1, 1 }, { .2, .2, .2, 1 } }
};

std::vector<GLuint> bigTriInds = { 2, 1, 0 },
medTriInds = { 2, 1, 0 },
smallTriInds = { 2, 1, 0 },
quadInds = { 2, 1, 0, 0, 3, 2 },
squareInds = { 2, 1, 0, 0, 3, 2 },
backPlaneInds = { 0, 1, 2, 0, 2, 3 };

namespace Models {

	RenderModel BigTri1Model(bigTri1Verts, bigTriInds),
		BigTri2Model(bigTri2Verts, bigTriInds),
		MedTriModel(medTriVerts, medTriInds),
		SmallTri1Model(smallTri1Verts, smallTriInds),
		SmallTri2Model(smallTri2Verts, smallTriInds),
		QuadModel(quadVerts, quadInds),
		SquareModel(squareVerts, squareInds),
		BackPlaneModel(backPlaneVerts,backPlaneInds);

	static std::vector<RenderModel *> allModels = {
		&BigTri1Model, &BigTri2Model, &MedTriModel,
		&SmallTri1Model, &SmallTri2Model, &QuadModel,
		&SquareModel, &BackPlaneModel
	};

	void setupModels()
	{
		for (RenderModel *model : allModels)
			model->setupModel();
	}

	void cleanupModels()
	{
		for (RenderModel *model : allModels)
			model->cleanupModel();
	}

}

namespace Scenes {
	namespace ObjectNames {
		const std::string BIG_TRI_1 = "bgtri1";
		const std::string BIG_TRI_2 = "bgtri2";
		const std::string MED_TRI_1 = "mdtri1";
		const std::string SML_TRI_1 = "smtri1";
		const std::string SML_TRI_2 = "smtri2";
		const std::string SQR = "sqr";
		const std::string QUAD = "quad";

		const std::vector<std::string> ALL_NAMES = {
			BIG_TRI_1, BIG_TRI_2, MED_TRI_1, SML_TRI_1, SML_TRI_2, SQR, QUAD
		};
	}

	StaticSceneConfiguration SquareTangramConfig;
	StaticSceneConfiguration FigureTangramConfig;
	TwoSceneLerpConfig SqrFigTangramConfig(&SquareTangramConfig,&FigureTangramConfig,1000);

	void setupSquareTangramConfig()
	{
		SquareTangramConfig.addWorldObject(ObjectNames::BIG_TRI_1, &Models::BigTri1Model, Vector3f(0, .66666, .9), 0);
		SquareTangramConfig.addWorldObject(ObjectNames::BIG_TRI_2, &Models::BigTri2Model, Vector3f(-.66666, 0, .8), deg2Rad(90));
		SquareTangramConfig.addWorldObject(ObjectNames::MED_TRI_1, &Models::MedTriModel, Vector3f(.66666, -.66666, .85), 0);
		SquareTangramConfig.addWorldObject(ObjectNames::SML_TRI_1, &Models::SmallTri1Model, Vector3f(.83333, .5, .83), 0);
		SquareTangramConfig.addWorldObject(ObjectNames::SML_TRI_2, &Models::SmallTri2Model, Vector3f(0, -.33333, .84), deg2Rad(-90));
		SquareTangramConfig.addWorldObject(ObjectNames::SQR, &Models::SquareModel, Vector3f(.5, 0, .835), 0);
		SquareTangramConfig.addWorldObject(ObjectNames::QUAD, &Models::QuadModel, Vector3f(-.25, -.75, .88), deg2Rad(90));
	}

	void setupFigureTangramConfig()
	{
		FigureTangramConfig.addWorldObject(ObjectNames::BIG_TRI_1, &Models::BigTri1Model, Vector3f(.42872, -.32383, .9), 0);
		FigureTangramConfig.addWorldObject(ObjectNames::BIG_TRI_2, &Models::BigTri2Model, Vector3f(.67121, .81169, .8), deg2Rad(210));
		FigureTangramConfig.addWorldObject(ObjectNames::MED_TRI_1, &Models::MedTriModel, Vector3f(-.69603, -.37143, .85), deg2Rad(135));
		FigureTangramConfig.addWorldObject(ObjectNames::SML_TRI_1, &Models::SmallTri1Model, Vector3f(-.55596, .62340, .83), deg2Rad(90));
		FigureTangramConfig.addWorldObject(ObjectNames::SML_TRI_2, &Models::SmallTri2Model, Vector3f(-1.57758, -.39284, .84), deg2Rad(90));
		FigureTangramConfig.addWorldObject(ObjectNames::SQR, &Models::SquareModel, Vector3f(-1.07758, .27403, .835), 0);
		FigureTangramConfig.addWorldObject(ObjectNames::QUAD, &Models::QuadModel, Vector3f(1.78149, .28546, .88), deg2Rad(-90));
	}

	void setupSqrFigTangramConfig()
	{
		//Nothing to do here for now...
	}

	void setupTangramConfigs()
	{
		setupSquareTangramConfig();
		setupFigureTangramConfig();
		setupSqrFigTangramConfig();
	}
}