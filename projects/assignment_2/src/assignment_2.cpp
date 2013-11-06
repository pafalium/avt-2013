///////////////////////////////////////////////////////////////////////
//
// Assignment 2 consists in the following:
//
// - Rewrite the program using C++ classes for:
//   [DONE] - Matrix manipulation;
//   [DONE] - Shader manipulation;
//   [DONE] - Managing drawable entities.
//
// - Provide an UML diagram of your solution.
//
// - Add the following functionality:
//   [DONE] - Read shader code from external files;
//   [DONE] - Check shader compilation and linkage for error messages.
// 
// - Draw the following scene, minimizing the number of vertices on the GPU:
//   [DONE] - A set of 7 TANs (i.e. TANGRAM shapes) of different colors;
//   [DONE] - A flat surface on which the TANs will be placed (with an appropriate contrasting color).
//
// - Alternate between the following dispositions when the user presses the 't' key;
//   [DONE] - The 7 TANs in their original square configuration;
//   [DONE] - The 7 TANs put together to form a shape of your choice (6500 to choose from!);
//   [DONE] - The silhouette of the shape of your choice on the flat surface.
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


#include "ShaderProgram.h"
#include "RenderModel.h"
#include "shader_setup.h"
#include "matrices.h"
#include "model_setup.h"
#include "uniformsAttribs.h"
#include "toggling.h"

#define CAPTION "Hello New World"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

//ShaderProgram *PassThroughProgram = 0;
//ShaderProgram *MonoChromeProgram = 0;

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
	ShaderPrograms::setupPrograms();
	checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram()
{
	ShaderPrograms::cleanupPrograms();
	checkOpenGLError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// SCENE

const Matrix4 I = Matrices::identity();
const Matrix4 TangramScale = Matrices::scale(.4, .4, 1);

Toggler *toggler;

void drawScene()
{
	ShaderPrograms::PassThroughProgram->use();
	GLint matrixID = ShaderPrograms::PassThroughProgram->getUniformId(Uniforms::MATRIX);
	//draw background plane
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, I.colMajorArray());
	Models::BackPlaneModel.drawModel();
	ShaderPrograms::PassThroughProgram->removeFromUse();
	
	//tangramShaderProgram->use();
	ShaderProgram *currProg = toggler->currentTangramShaderProgram();
	currProg->use();
	//for each model in curr_config
	//get model matrix
	//send model matrix to shaders
	//draw model
	SceneConfiguration *currSceneConfig = toggler->currentSceneConfiguration();
	for (std::string objName : Scenes::ObjectNames::ALL_NAMES) {
		SceneConfiguration::WorldObject wrlObj = currSceneConfig->getWorldObject(objName);
		Matrix4 modelMatrix = wrlObj.getModelMatrix();
		modelMatrix = TangramScale * modelMatrix;
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, modelMatrix.colMajorArray());
		wrlObj.drawRenderModel();
	}
	currProg->removeFromUse();
	//tangramShaderProgram->removeFromUse();

	checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	destroyShaderProgram();
	Models::cleanupModels();
	//cleanup toggling
	delete toggler;
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

void lerpTestTimer(int val)
{
	Scenes::SqrFigTangramConfig.incrMSecsLoop(16);
	glutTimerFunc(16, lerpTestTimer, 0);
}

void keyPressed(unsigned char key, int x, int y)
{
	switch (key){
	case 't':
		toggler->toggle();
		break;
	}
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupScenes()
{
	Models::setupModels();
	Scenes::setupTangramConfigs();
}

void setupToggling()
{
	toggler = new Toggler(TogglerSquare::instance());
}

void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPressed);
	glutTimerFunc(0,timer,0);
	//lerp test
	glutTimerFunc(0, lerpTestTimer, 0);
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
	setupToggling();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();	
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////