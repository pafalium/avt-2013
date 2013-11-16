//model_setup.cpp

#include <GL/glew.h>
#include <vector>
#include <sstream>

#include "utils.h"

#include "model_setup.h"

//TODO store all created RenderModels
//TODO store all created SceneObjects

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

//TODO remove
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

namespace SingleScene {
	namespace SceneObjects {
		SceneObject *BP, *BT1, *BT2, *MT1, *ST1, *ST2, *SQR, *QUAD;
		namespace Names {
			const std::string BACK_PLANE = "bkpln";
			const std::string BIG_TRI_1 = "bgtri1";
			const std::string BIG_TRI_2 = "bgtri2";
			const std::string MED_TRI_1 = "mdtri1";
			const std::string SML_TRI_1 = "smtri1";
			const std::string SML_TRI_2 = "smtri2";
			const std::string SQR = "sqr";
			const std::string QUAD = "quad";
		}
		namespace Colors {
			const Vector3f BACK_PLANE(0.8f, 0.8f, 0.0f);
			const Vector3f BIG_TRI_1(0.8f,0.1f,0.1f);
			const Vector3f BIG_TRI_2(0.26f,0.8f,0.0f);
			const Vector3f MED_TRI_1(0.8f,0.2f,0.0f);
			const Vector3f SML_TRI_1(0.8f,0.75f,0.0f);
			const Vector3f SML_TRI_2(0.0f,0.8f,0.34f);
			const Vector3f SQR(0.0f,0.0f,0.8f);
			const Vector3f QUAD(0.8f,0.0f,0.65f);
		}
		namespace InitLoc {
			const Vector3f BACK_PLANE(0.0f, 0.0f, 0.0f);
			const Vector3f BIG_TRI_1(0, .66666, 0);
			const Vector3f BIG_TRI_2(-.66666, 0, 0);
			const Vector3f MED_TRI_1(.66666, -.66666, 0);
			const Vector3f SML_TRI_1(.83333, .5, 0);
			const Vector3f SML_TRI_2(0, -.33333, 0);
			const Vector3f SQR(.5, 0, 0);
			const Vector3f QUAD(-.25, -.75, 0);
		}
		namespace InitRot {
			const Quaternion BACK_PLANE = qFromAngleAxis(0, Vector3f(0.0f, 0.0f, 1.0f));
			const Quaternion BIG_TRI_1 = qFromAngleAxis(0, Vector3f(0.0f, 0.0f, 1.0f));
			const Quaternion BIG_TRI_2 = qFromAngleAxis(90.0f, Vector3f(0.0f, 0.0f, 1.0f));
			const Quaternion MED_TRI_1 = qFromAngleAxis(0, Vector3f(0.0f, 0.0f, 1.0f));
			const Quaternion SML_TRI_1 = qFromAngleAxis(0, Vector3f(0.0f, 0.0f, 1.0f));
			const Quaternion SML_TRI_2 = qFromAngleAxis(-90.0f, Vector3f(0.0f, 0.0f, 1.0f));
			const Quaternion SQR = qFromAngleAxis(0, Vector3f(0.0f, 0.0f, 1.0f));
			const Quaternion QUAD = qFromAngleAxis(90, Vector3f(0.0f, 0.0f, 1.0f));
		}
	}

	namespace Animations {
		Animation *INIT, *MBT1, *MBT2, *MMT1, *MST1, *MST2, *MSQR, *MQUAD;
		namespace Names {
			const std::string INIT("INIT");
			const std::string MBT1("MBT1");
			const std::string MBT2("MBT2");
			const std::string MMT1("MMT1");
			const std::string MST1("MST1");
			const std::string MST2("MST2");
			const std::string MSQR("MSQR");
			const std::string MQUAD("MQUAD");
		}
	}

	Scene *TangramScene;
	AnimationManager *TangramAnimManager;

	void setupSceneObjects()
	{
		SceneObjects::BP = new SceneObject(SceneObjects::Names::BACK_PLANE, Models::BackPlaneModel,
			SceneObjects::InitLoc::BACK_PLANE, SceneObjects::Colors::BACK_PLANE, SceneObjects::InitRot::BACK_PLANE);
		SceneObjects::BT1 = new SceneObject(SceneObjects::Names::BIG_TRI_1, Models::BigTriModel, 
			SceneObjects::InitLoc::BIG_TRI_1, SceneObjects::Colors::BIG_TRI_1, SceneObjects::InitRot::BIG_TRI_1);
		SceneObjects::BT2 = new SceneObject(SceneObjects::Names::BIG_TRI_2, Models::BigTriModel,
			SceneObjects::InitLoc::BIG_TRI_2, SceneObjects::Colors::BIG_TRI_2, SceneObjects::InitRot::BIG_TRI_2);
		SceneObjects::MT1 = new SceneObject(SceneObjects::Names::MED_TRI_1, Models::MedTriModel,
			SceneObjects::InitLoc::MED_TRI_1, SceneObjects::Colors::MED_TRI_1, SceneObjects::InitRot::MED_TRI_1);
		SceneObjects::ST1 = new SceneObject(SceneObjects::Names::SML_TRI_1, Models::SmallTriModel,
			SceneObjects::InitLoc::SML_TRI_1, SceneObjects::Colors::SML_TRI_1, SceneObjects::InitRot::SML_TRI_1);
		SceneObjects::ST2 = new SceneObject(SceneObjects::Names::SML_TRI_2, Models::SmallTriModel,
			SceneObjects::InitLoc::SML_TRI_2, SceneObjects::Colors::SML_TRI_2, SceneObjects::InitRot::SML_TRI_2);
		SceneObjects::SQR = new SceneObject(SceneObjects::Names::SQR, Models::SquareModel,
			SceneObjects::InitLoc::SQR, SceneObjects::Colors::SQR, SceneObjects::InitRot::SQR);
		SceneObjects::QUAD = new SceneObject(SceneObjects::Names::QUAD, Models::QuadModel,
			SceneObjects::InitLoc::QUAD, SceneObjects::Colors::QUAD, SceneObjects::InitRot::QUAD);
	}
	void cleanupSceneObjects()
	{
		delete SceneObjects::BP;
		delete SceneObjects::BT1;
		delete SceneObjects::BT2;
		delete SceneObjects::MT1;
		delete SceneObjects::QUAD;
		delete SceneObjects::SQR;
		delete SceneObjects::ST1;
		delete SceneObjects::ST2;
	}
	void setupScene()
	{
		setupSceneObjects();
		TangramScene = new Scene();
		TangramScene->addObject(SceneObjects::BP);
		TangramScene->addObject(SceneObjects::BT1);
		TangramScene->addObject(SceneObjects::BT2);
		TangramScene->addObject(SceneObjects::ST1);
		TangramScene->addObject(SceneObjects::ST2);
		TangramScene->addObject(SceneObjects::MT1);
		TangramScene->addObject(SceneObjects::SQR);
		TangramScene->addObject(SceneObjects::QUAD);
	}
	void cleanupScene()
	{
		delete TangramScene;
		cleanupSceneObjects();
	}
	void setupAnimations()
	{
		TangramAnimManager = new AnimationManager(TangramScene);
		Animations::INIT = new Animation(Animations::Names::INIT);
		TangramAnimManager->addAnimation(Animations::INIT);
		Animations::INIT->addObjectKeyFrame(SceneObjects::BT1, KeyFrame(0, SceneObjects::InitLoc::BIG_TRI_1, SceneObjects::InitRot::BIG_TRI_1));
		Animations::INIT->addObjectKeyFrame(SceneObjects::BT2, KeyFrame(0, SceneObjects::InitLoc::BIG_TRI_2, SceneObjects::InitRot::BIG_TRI_2));
		Animations::INIT->addObjectKeyFrame(SceneObjects::MT1, KeyFrame(0, SceneObjects::InitLoc::MED_TRI_1, SceneObjects::InitRot::MED_TRI_1));
		Animations::INIT->addObjectKeyFrame(SceneObjects::ST1, KeyFrame(0, SceneObjects::InitLoc::SML_TRI_1, SceneObjects::InitRot::SML_TRI_1));
		Animations::INIT->addObjectKeyFrame(SceneObjects::ST2, KeyFrame(0, SceneObjects::InitLoc::SML_TRI_2, SceneObjects::InitRot::SML_TRI_2));
		Animations::INIT->addObjectKeyFrame(SceneObjects::SQR, KeyFrame(0, SceneObjects::InitLoc::SQR, SceneObjects::InitRot::SQR));
		Animations::INIT->addObjectKeyFrame(SceneObjects::QUAD, KeyFrame(0, SceneObjects::InitLoc::QUAD, SceneObjects::InitRot::QUAD));

		Animations::MBT1 = new Animation(Animations::Names::MBT1);
		TangramAnimManager->addAnimation(Animations::MBT1);
		Animations::MBT1->addObjectKeyFrame(SceneObjects::BT1, KeyFrame(0, SceneObjects::InitLoc::BIG_TRI_1, SceneObjects::InitRot::BIG_TRI_1));
		Animations::MBT1->addObjectKeyFrame(SceneObjects::BT1, KeyFrame(1000, Vector3f(.4287, .1295, .7489), qFromTwoVectors(Vector3f(0,0,1),Vector3f(0,-1,0))));

		Animations::MBT2 = new Animation(Animations::Names::MBT2);
		TangramAnimManager->addAnimation(Animations::MBT2);
		Animations::MBT2->addObjectKeyFrame(SceneObjects::BT2, KeyFrame(0, SceneObjects::InitLoc::BIG_TRI_2, SceneObjects::InitRot::BIG_TRI_2));
		Animations::MBT2->addObjectKeyFrame(SceneObjects::BT2, KeyFrame(1000, Vector3f(.6712, .1295, 1.8844), qMultiply(qFromTwoVectors(Vector3f(0, 0, 1), Vector3f(0, -1, 0)), qFromAngleAxis(-150, Vector3f(0, 0, 1)))));

		Animations::MMT1 = new Animation(Animations::Names::MMT1);
		TangramAnimManager->addAnimation(Animations::MMT1);
		Animations::MMT1->addObjectKeyFrame(SceneObjects::MT1, KeyFrame(0, SceneObjects::InitLoc::MED_TRI_1, SceneObjects::InitRot::MED_TRI_1));
		Animations::MMT1->addObjectKeyFrame(SceneObjects::MT1, KeyFrame(1000, Vector3f(-.6960, .0995, .7013), qMultiply(qFromTwoVectors(Vector3f(0,0,1),Vector3f(0,-1,0)), qFromAngleAxis(135, Vector3f(0,0,1)))));

		Animations::MST1 = new Animation(Animations::Names::MST1);
		TangramAnimManager->addAnimation(Animations::MST1);
		Animations::MST1->addObjectKeyFrame(SceneObjects::ST1, KeyFrame(0, SceneObjects::InitLoc::SML_TRI_1, SceneObjects::InitRot::SML_TRI_1));
		Animations::MST1->addObjectKeyFrame(SceneObjects::ST1, KeyFrame(1000, Vector3f(-.5559, .1295, 1.6961), qMultiply(qFromTwoVectors(Vector3f(0, 0, 1), Vector3f(0, -1, 0)), qFromAngleAxis(90, Vector3f(0, 0, 1)))));

		Animations::MST2 = new Animation(Animations::Names::MST2);
		TangramAnimManager->addAnimation(Animations::MST2);
		Animations::MST2->addObjectKeyFrame(SceneObjects::ST2, KeyFrame(0, SceneObjects::InitLoc::SML_TRI_2, SceneObjects::InitRot::SML_TRI_2));
		Animations::MST2->addObjectKeyFrame(SceneObjects::ST2, KeyFrame(1000, Vector3f(-1.5775, .1295, .6798), qMultiply(qFromTwoVectors(Vector3f(0, 0, 1), Vector3f(0, -1, 0)), qFromAngleAxis(90, Vector3f(0, 0, 1)))));

		Animations::MSQR = new Animation(Animations::Names::MSQR);
		TangramAnimManager->addAnimation(Animations::MSQR);
		Animations::MSQR->addObjectKeyFrame(SceneObjects::SQR, KeyFrame(0, SceneObjects::InitLoc::SQR, SceneObjects::InitRot::SQR));
		Animations::MSQR->addObjectKeyFrame(SceneObjects::SQR, KeyFrame(1000, Vector3f(-1.0775, .1295, 1.3467), qFromTwoVectors(Vector3f(0, 0, 1), Vector3f(0, -1, 0))));

		Animations::MQUAD = new Animation(Animations::Names::MQUAD);
		TangramAnimManager->addAnimation(Animations::MQUAD);
		Animations::MQUAD->addObjectKeyFrame(SceneObjects::QUAD, KeyFrame(0, SceneObjects::InitLoc::QUAD, SceneObjects::InitRot::QUAD));
		Animations::MQUAD->addObjectKeyFrame(SceneObjects::QUAD, KeyFrame(1000, Vector3f(1.7814, .0795, 1.3581), qMultiply(qFromTwoVectors(Vector3f(0, 0, 1), Vector3f(0, -1, 0)), qFromAngleAxis(-90, Vector3f(0, 0, 1)))));
	}
	void cleanupAnimations()
	{
		delete TangramAnimManager;
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