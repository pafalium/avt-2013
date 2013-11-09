///////////////////////////////////////////////////////////////////////
//
// Assignment 3 consists in the following:
//
// - Create the following changes to your scene, making it fully 3D:
//   - Extrude your TANs into the 3rd dimension. The TANs should have
//     slightly different "heights".
//   - The new faces of each TAN should share the same hue as the 
//     original top face color but have different levels of saturation 
//     and brightness (use an external app if needed).
//   - The shape is now built vertically (i.e. rather than horizontally
//     as in assignment 2) but still on top of the surface.
//   - When the TANs join to create your chosen shape, they should not 
//     be perfectly aligned in the new dimension.
//
// - Add the following functionality:
//   [DONE] - Create a View Matrix from (eye, center, up) parameters.
//   [DONE] - Create an Orthographic Projection Matrix from (left-right, 
//     bottom-top, near-far) parameters.
//   [DONE] - Create a Perspective Projection Matrix from (fovy, aspect,
//     nearZ, farZ) parameters.
//   - Implement rotations through quaternions.
//
// - Add the following dynamics to the application:
//   - Create a spherical 3D camera controlled by the mouse allowing to 
//     visualize the scene through all its angles.
//   - Change perspective from orthographic to perspective and back as
//     a response to pressing the key 'p'.
//   - The scene starts with the 7 TANs in their original square 
//     configuration, laying flat and horizontally on the surface.
//   - Each time the 't' key is pressed, one of the TANs will move from 
//     its resting place to its position in your chosen shape, now 
//     presented vertically.
//
// Further suggestions:
//   - Use mouse interaction to select which TAN will be next moved 
//     into place or moved back to its resting position.
//   - Create an edit mode in which the user can edit the shape in
//     real-time (e.g. position, rotation, color).
//   - Allow to load and save the TANGRAM shapes from external files.
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

#include "utils.h"

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

Matrix4 Proj1 = Matrices::orthoProj(-2, 2, -2, 2, -1, 5);
Matrix4 Proj2 = Matrices::perspectiveProj(deg2Rad(45.0), 4.0 / 3.0, 0, 5.0);
Vector3f EyePos = Vector3f(0, 0, 2);
Vector3f CenterPos = Vector3f(0, 0, 0);
Matrix4 TestView = Matrices::lookAt(EyePos, CenterPos, Vector3f(0, 1, 0));
const Matrix4 I = Matrices::identity();
const Matrix4 TangramScale = Matrices::scale(.4, .4, 1);

Toggler *toggler;

void drawScene()
{
	Matrix4 projView = Proj2 * TestView;

	ShaderPrograms::PassThroughProgram->use();
	GLint matrixID = ShaderPrograms::PassThroughProgram->getUniformId(Uniforms::MATRIX);
	//draw background plane
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, projView.colMajorArray());
	//glUniformMatrix4fv(matrixID, 1, GL_FALSE, I.colMajorArray());
	Models::BackPlaneModel.drawModel();
	ShaderPrograms::PassThroughProgram->removeFromUse();
	
	ShaderProgram *currProg = toggler->currentTangramShaderProgram();
	currProg->use();
	SceneConfiguration *currSceneConfig = toggler->currentSceneConfiguration();
	for (std::string objName : Scenes::ObjectNames::ALL_NAMES) {
		SceneConfiguration::WorldObject wrlObj = currSceneConfig->getWorldObject(objName);
		Matrix4 modelMatrix = wrlObj.getModelMatrix();
		//modelMatrix = TangramScale * modelMatrix;
		modelMatrix = projView * TangramScale * modelMatrix;//TODO remove test, implement matrix2shader passing
		glUniformMatrix4fv(matrixID, 1, GL_FALSE, modelMatrix.colMajorArray());
		wrlObj.drawRenderModel();
	}
	currProg->removeFromUse();

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
		//TODO remove tests
	case 'w':
		EyePos += Vector3f(0, .2, 0);
		break;
	case 's':
		EyePos -= Vector3f(0, .2, 0);
		break;
	case 'a':
		EyePos -= Vector3f(.2, 0, 0);
		break;
	case 'd':
		EyePos += Vector3f(.2, 0, 0);
		break;
	case 'y':
		CenterPos += Vector3f(0, .2, 0);
		break;
	case 'h':
		CenterPos -= Vector3f(0, .2, 0);
		break;
	case 'g':
		CenterPos -= Vector3f(.2, 0, 0);
		break;
	case 'j':
		CenterPos += Vector3f(.2, 0, 0);
		break;
	}
	switch (key) {
	case 'w':
	case 's':
	case 'a':
	case 'd':
	case 'y':
	case 'h':
	case 'g':
	case 'j':
		TestView = Matrices::lookAt(EyePos, CenterPos, Vector3f(0, 1, 0));
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