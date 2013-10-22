///////////////////////////////////////////////////////////////////////
//
// Assignment 2 consists in the following:
//
// - Rewrite the program using C++ classes for:
//   [DONE] - Matrix manipulation;
//   - Shader manipulation;
//   - Managing drawable entities.
//
// - Provide an UML diagram of your solution.
//
// - Add the following functionality:
//   [DONE] - Read shader code from external files;
//   [DONE] - Check shader compilation and linkage for error messages.
// 
// - Draw the following scene, minimizing the number of vertices on the GPU:
//   - A set of 7 TANs (i.e. TANGRAM shapes) of different colors;
//   - A flat surface on which the TANs will be placed (with an appropriate contrasting color).
//
// - Alternate between the following dispositions when the user presses the 't' key;
//   - The 7 TANs in their original square configuration;
//   - The 7 TANs put together to form a shape of your choice (6500 to choose from!);
//   - The silhouette of the shape of your choice on the flat surface.
//
// Further suggestions to verify your understanding of the concepts explored:
//
// - Use linear interpolation to create a transition between the two TAN configurations.
// - Parse geometrical information from external XML files.
//
// (c) 2013 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <string>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "readFile.h"

#include "ShaderProgram.h"
#include "RenderModel.h"
#include "matrices.h"
#include "model_setup.h"
#include "uniformsAttribs.h"

#define CAPTION "Hello New World"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

#define VERTICES 0
#define COLORS 1

GLuint VaoId, VboId[2];
GLint UniformId;

ShaderProgram *PassThroughProgram = 0;

/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
	bool isError = false;
	GLenum errCode;
	const GLubyte *errString;
	while ((errCode = glGetError()) != GL_NO_ERROR) {
		isError = true;
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
	}
	return isError;
}

void checkOpenGLError(std::string error)
{
	if(isOpenGLError()) {
		std::cerr << error << std::endl;
		exit(EXIT_FAILURE);
	}
}

/////////////////////////////////////////////////////////////////////// SHADERs

void createShaderProgram()
{


	std::string shaderPath("../src/");
	std::string vertexFile("vertex.vsh"), fragmentFile("fragment.fsh");
	std::string vertexShader = readFromFile(shaderPath + vertexFile);
	std::string fragmentShader = readFromFile(shaderPath + fragmentFile);

	PassThroughProgram = new ShaderProgram(vertexShader, fragmentShader);
	PassThroughProgram->createCompileLink();

	GLuint programID = PassThroughProgram->programName();
	UniformId = glGetUniformLocation(programID, "Matrix");

	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	delete PassThroughProgram;
	checkOpenGLError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs

//typedef struct {
//	GLfloat XYZW[4];
//	GLfloat RGBA[4];
//} Vertex;

const Vertex Vertices[] = 
{
	{{ 0.25f, 0.25f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1.0f }},
	{{ 0.75f, 0.25f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }},
	{{ 0.50f, 0.75f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f }}
};

const GLubyte Indices[] =
{
	0,1,2
};

void createBufferObjects()
{
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);

	glGenBuffers(2, VboId);

	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(VERTICES);
	glVertexAttribPointer(VERTICES, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(COLORS);
	glVertexAttribPointer(COLORS, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)sizeof(Vertices[0].xyzw));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);

	checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects()
{
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(COLORS);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(2, VboId);
	glDeleteVertexArrays(1, &VaoId);
	checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// SCENE

typedef GLfloat Matrix[16];

const Matrix Io = {
	1.0f,  0.0f,  0.0f,  0.0f,
	0.0f,  1.0f,  0.0f,  0.0f,
	0.0f,  0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
}; // Row Major (GLSL is Column Major)

const Matrix Mo = {
	1.0f,  0.0f,  0.0f, -1.0f,
	0.0f,  1.0f,  0.0f, -1.0f,
	0.0f,  0.0f,  1.0f,  0.0f,
	0.0f,  0.0f,  0.0f,  1.0f
}; // Row Major (GLSL is Column Major)

const Matrix4 I = Matrices::identity();
const Matrix4 M = Matrices::translate(-1.0f, -1.0f, 0.0f);
const Matrix4 TangramScale = Matrices::scale(.4, .4, .4);

SceneConfiguration *activeSceneConfig;

void drawScene()
{
	glBindVertexArray(VaoId);
	PassThroughProgram->use();

	//glUniformMatrix4fv(UniformId, 1, GL_FALSE, I.colMajorArray());
	//Models::BackPlaneModel.drawModel();

	//glUniformMatrix4fv(UniformId, 1, GL_FALSE, I.colMajorArray());
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

	//glUniformMatrix4fv(UniformId, 1, GL_FALSE, M.colMajorArray());
	//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);

	//glUniformMatrix4fv(UniformId, 1, GL_FALSE, I.colMajorArray());
	////Models::BigTriModel.drawModel();
	//Models::MedTriModel.drawModel();
	////Models::SmallTriModel.drawModel();
	//Models::SquareModel.drawModel();
	//Models::QuadModel.drawModel();

	//for each model in curr_config
	//get model matrix
	//send model matrix to shaders
	//draw model
	GLint matrixID = PassThroughProgram->getUniformId(Uniforms::MATRIX);
	for (std::string objName : Scenes::ObjectNames::ALL_NAMES) {
		SceneConfiguration::WorldObject wrlObj = activeSceneConfig->getWorldObject(objName);
		Matrix4 modelMatrix = wrlObj.getModelMatrix();
		modelMatrix = TangramScale * modelMatrix;
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, modelMatrix.colMajorArray());
		wrlObj.drawRenderModel();
	}

	PassThroughProgram->removeFromUse();
	glBindVertexArray(0);

	checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	destroyShaderProgram();
	destroyBufferObjects();
	Models::cleanupModels();
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();
}

void idle()
{
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	WinX = w;
	WinY = h;
	glViewport(0, 0, WinX, WinY);
}

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
    FrameCount = 0;
    glutTimerFunc(1000, timer, 0);
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupScenes()
{
	createBufferObjects();
	Models::setupModels();
	Scenes::setupTangramConfigs();
	activeSceneConfig = &Scenes::SquareTangramConfig;
}

void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0,timer,0);
}

void setupOpenGL() {
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit() ; 
	if (result != GLEW_OK) { 
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	} 
	GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[])
{
	glutInit(&argc, argv);
	
	glutInitContextVersion(3, 3);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	
	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if(WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[])
{
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	createShaderProgram();
	setupScenes();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();	
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////