//model_setup.cpp

#include <GL/glew.h>
#include <vector>
#include <sstream>

#include "utils.h"

#include "model_setup.h"


namespace Models {

	RenderModel *BigTriModel,
		*MedTriModel,
		*SmallTriModel,
		*QuadModel,
		*SquareModel,
		*BackPlaneModel;

	void setupModels()
	{
		std::string resourcePath("../resources/");
		std::string bigtrifile("BigTri.obj");
		std::string medtrifile("MedTri.obj");
		std::string smalltrifile("SmallTri.obj");
		std::string quadfile("Quad.obj");
		std::string squarefile("Square.obj");
		std::string backplanefile("BackPlane.obj");

		ModelLoader loader;
		BigTriModel = loader.loadModel(readFromFile(resourcePath + bigtrifile));
		MedTriModel = loader.loadModel(readFromFile(resourcePath + medtrifile));
		SmallTriModel = loader.loadModel(readFromFile(resourcePath + smalltrifile));
		QuadModel = loader.loadModel(readFromFile(resourcePath + quadfile));
		SquareModel = loader.loadModel(readFromFile(resourcePath + squarefile));
		BackPlaneModel = loader.loadModel(readFromFile(resourcePath + backplanefile));

		BigTriModel->setupModel();
		MedTriModel->setupModel();
		SmallTriModel->setupModel();
		QuadModel->setupModel();
		SquareModel->setupModel();
		BackPlaneModel->setupModel();
	}

	void cleanupModels()
	{
		BigTriModel->cleanupModel();
		MedTriModel->cleanupModel();
		SmallTriModel->cleanupModel();
		QuadModel->cleanupModel();
		SquareModel->cleanupModel();
		BackPlaneModel->cleanupModel();
		delete BigTriModel;
		delete MedTriModel;
		delete SmallTriModel;
		delete QuadModel;
		delete BackPlaneModel;
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


	//TODO add class for drawing giving a color as an uniform
	void setupSquareTangramConfig()
	{
		SquareTangramConfig.addWorldObject(ObjectNames::BIG_TRI_1, Models::BigTriModel, Vector3f(0, .66666, 0), 0);
		SquareTangramConfig.addWorldObject(ObjectNames::BIG_TRI_2, Models::BigTriModel, Vector3f(-.66666, 0, 0), deg2Rad(90));
		SquareTangramConfig.addWorldObject(ObjectNames::MED_TRI_1, Models::MedTriModel, Vector3f(.66666, -.66666, 0), 0);
		SquareTangramConfig.addWorldObject(ObjectNames::SML_TRI_1, Models::SmallTriModel, Vector3f(.83333, .5, 0), 0);
		SquareTangramConfig.addWorldObject(ObjectNames::SML_TRI_2, Models::SmallTriModel, Vector3f(0, -.33333, 0), deg2Rad(-90));
		SquareTangramConfig.addWorldObject(ObjectNames::SQR, Models::SquareModel, Vector3f(.5, 0, 0), 0);
		SquareTangramConfig.addWorldObject(ObjectNames::QUAD, Models::QuadModel, Vector3f(-.25, -.75, 0), deg2Rad(90));
	}

	void setupFigureTangramConfig()
	{
		FigureTangramConfig.addWorldObject(ObjectNames::BIG_TRI_1, Models::BigTriModel, Vector3f(.42872, -.32383, .9), 0);
		FigureTangramConfig.addWorldObject(ObjectNames::BIG_TRI_2, Models::BigTriModel, Vector3f(.67121, .81169, .8), deg2Rad(210));
		FigureTangramConfig.addWorldObject(ObjectNames::MED_TRI_1, Models::MedTriModel, Vector3f(-.69603, -.37143, .85), deg2Rad(135));
		FigureTangramConfig.addWorldObject(ObjectNames::SML_TRI_1, Models::SmallTriModel, Vector3f(-.55596, .62340, .83), deg2Rad(90));
		FigureTangramConfig.addWorldObject(ObjectNames::SML_TRI_2, Models::SmallTriModel, Vector3f(-1.57758, -.39284, .84), deg2Rad(90));
		FigureTangramConfig.addWorldObject(ObjectNames::SQR, Models::SquareModel, Vector3f(-1.07758, .27403, .835), 0);
		FigureTangramConfig.addWorldObject(ObjectNames::QUAD, Models::QuadModel, Vector3f(1.78149, .28546, .88), deg2Rad(-90));
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

RenderModel *ModelLoader::loadModel(const std::string& objString)
{
	parseString(objString);
	prepareModelData();
	createRenderModel();
	cleanup();
	return m_loadedModel;
}

void ModelLoader::parseString(const std::string &objString)
{
	//create string input stream
	std::istringstream objStream(objString);
	//for each line
	//get line
	std::string line;
	while (std::getline(objStream, line))
	{
		//parse line
		parseLine(line);
	}
}

void ModelLoader::parseLine(const std::string &line) {
	std::istringstream lineStream(line);
	std::string lineType;
	lineStream >> lineType;

	if (lineType.compare("v") == 0) parseVertex(lineStream);
	if (lineType.compare("vn") == 0) parseNormal(lineStream);
	if (lineType.compare("vt") == 0) parseTexCoord(lineStream);
	if (lineType.compare("f") == 0) parseFace(lineStream);
}

void ModelLoader::parseVertex(std::istringstream &stream)
{
	Vertex v;
	stream >> v.x >> v.y >> v.z;
	m_vertices.push_back(v);
}

void ModelLoader::parseNormal(std::istringstream &stream)
{
	Normal n;
	stream >> n.x >> n.y >> n.z;
	m_normals.push_back(n);
}

void ModelLoader::parseTexCoord(std::istringstream &stream)
{
	TexCoord vt;
	stream >> vt.s >> vt.t;
	m_texCoords.push_back(vt);
}

void ModelLoader::parseFace(std::istringstream &stream)
{
	std::string vertexString;
	stream >> vertexString;
	parseVertexIndexes(vertexString);
	stream >> vertexString;
	parseVertexIndexes(vertexString);
	stream >> vertexString;
	parseVertexIndexes(vertexString);
}

void ModelLoader::parseVertexIndexes(const std::string &vertexString)
{
	std::istringstream vertexStream(vertexString);
	std::string vInd, tInd, nInd;

	std::getline(vertexStream, vInd, '/');
	std::getline(vertexStream, tInd, '/');
	std::getline(vertexStream, nInd, '/');

	unsigned int verInd, texInd, norInd;
	verInd = std::stoul(vInd);
	texInd = std::stoul(tInd);
	norInd = std::stoul(nInd);

	Index ind;
	ind.v = verInd;
	ind.vn = norInd;
	ind.vt = texInd;

	m_indexes.push_back(ind);
}

void ModelLoader::prepareModelData()
{
	for (Index &ind : m_indexes) {
		m_vertexData.push_back(m_vertices[ind.v - 1]);
		m_normalData.push_back(m_normals[ind.vn - 1]);
		m_texCoordData.push_back(m_texCoords[ind.vt - 1]);
	}
}

void ModelLoader::createRenderModel()
{
	m_loadedModel = new RenderModel(m_vertexData, m_normalData, m_texCoordData);
}

void ModelLoader::cleanup()
{
	m_vertices.clear();
	m_vertexData.clear();
	m_normals.clear();
	m_normalData.clear();
	m_texCoords.clear();
	m_texCoordData.clear();
	m_indexes.clear();
}