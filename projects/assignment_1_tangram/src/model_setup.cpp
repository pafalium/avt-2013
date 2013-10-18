
#include <GL/glew.h>
#include <vector>

#include "renderInfo.h"
#include "RenderModel.h"

#include "model_setup.h"


std::vector<Vertex> bigTriVerts = {
	{ { 1, .3333, 0, 1 }, { .25, .8, 0, 1 } },
	{ { 0, -.6666, 0, 1 }, { .25, .8, 0, 1 } },
	{ { -1, .3333, 0, 1 }, { .25, .8, 0 , 1} }
}, medTriVerts = {
	{ {.3333, .6666, 0, 1}, {.8, .2, 0, 1} },
	{ {.3333, -.3333, 0, 1}, { .8, .2, 0, 1 } },
	{ {-.6666, -.3333, 0, 1}, { .8, .2, 0, 1 } }
}, smallTriVerts = {
	{ {.1666, .5, 0, 1}, {.8, .8, 0, 1} },
	{ {.1666, -.5, 0, 1}, { .8, .8, 0, 1 } },
	{ {-.3333, 0, 0, 1}, { .8, .8, 0, 1 } }
}, quadVerts = {
	{ {.25, -.25, 0, 1}, {.8, .65, 0, 1} },
	{ {.25, .75, 0, 1}, { .8, .65, 0, 1 } },
	{ {-.25, .25, 0, 1 }, { .8, .65, 0, 1 } },
	{ {-.25, -.75, 0, 1 }, { .8, .65, 0, 1 } }
}, squareVerts = {
	{ { 0, .5, 0, 1 }, { 0, 0, .8, 1 } },
	{ { .5, 0, 0, 1 }, { 0, 0, .8, 1 } },
	{ { 0, -.5, 0, 1 }, { 0, 0, .8, 1 } },
	{ { -.5, 0, 0, 1 }, { 0, 0, .8, 1 } },
};

std::vector<GLuint> bigTriInds = { 2, 1, 0 },
medTriInds = { 2, 1, 0 },
smallTriInds = { 2, 1, 0 },
quadInds = { 2, 1, 0, 0, 3, 2 },
squareInds = { 2, 1, 0, 0, 3, 2 };

namespace Models {

	RenderModel BigTriModel(bigTriVerts, bigTriInds),
		MedTriModel(medTriVerts, medTriInds),
		SmallTriModel(smallTriVerts, smallTriInds),
		QuadModel(quadVerts, quadInds),
		SquareModel(squareVerts, squareInds);

	void setupModels()
	{
		BigTriModel.setupModel();
		MedTriModel.setupModel();
		SmallTriModel.setupModel();
		QuadModel.setupModel();
		SquareModel.setupModel();
	}

	void cleanupModels()
	{
		BigTriModel.cleanupModel();
		MedTriModel.cleanupModel();
		SmallTriModel.cleanupModel();
		QuadModel.cleanupModel();
		SquareModel.cleanupModel();
	}

}