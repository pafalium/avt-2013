///////////////////////////////////////////////////////////////////////
//
// Assignment 3 consists in the following:
//
// - Create the following changes to your scene, making it fully 3D:
//   [DONE] - Extrude your TANs into the 3rd dimension. The TANs should have
//     slightly different "heights".
//   [NORMALS][DONE]- The new faces of each TAN should share the same hue as the 
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
//   [DONE] - Create a spherical 3D camera controlled by the mouse allowing to 
//     visualize the scene through all its angles.
//   [DONE] - Change perspective from orthographic to perspective and back as
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

#include "matrices.h"
#include "utils.h"

#include "ShaderProgram.h"
#include "RenderModel.h"

#include "uniformsAttribs.h"
#include "shader_setup.h"
#include "model_setup.h"
#include "toggling.h"
#include "camera.h"
#include "camera_setup.h"


#define CAPTION "Hello New World"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

/////////////////////////////////////////////////////////////////////// ERRORS



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
const Vector3f ObjectsColor(0.0f, 1.0f, 1.0f);
const Vector3f BackColor(.8f, .8f, 0.0f);

Toggler *toggler;

void drawScene()
{
	//ShaderPrograms::sendSharedMatBufMatrix(0, TestView);
	//ShaderPrograms::sendSharedMatBufMatrix(1, Proj2);
	ShaderPrograms::sendSharedMatBufMatrix(0, MyCamera->viewMatrix());
	ShaderPrograms::sendSharedMatBufMatrix(1, MyCamera->projMatrix());

	//draw background plane
	ShaderPrograms::PassThroughProgram->use();
	ShaderPrograms::PassThroughProgram->sendUniformMat4(Uniforms::MATRIX, I);
	ShaderPrograms::PassThroughProgram->sendUniformVec3(Uniforms::COLOR, BackColor);
	Models::BackPlaneModel->drawModel();
	ShaderPrograms::PassThroughProgram->removeFromUse();
	
	ShaderProgram *currProg = toggler->currentTangramShaderProgram();
	currProg->use();
	SceneConfiguration *currSceneConfig = toggler->currentSceneConfiguration();
	for (std::string objName : Scenes::ObjectNames::ALL_NAMES) {
		SceneConfiguration::WorldObject wrlObj = currSceneConfig->getWorldObject(objName);
		Matrix4 modelMatrix = wrlObj.getModelMatrix();
		//modelMatrix = TangramScale * modelMatrix;
		currProg->sendUniformMat4(Uniforms::MATRIX, modelMatrix);
		currProg->sendUniformVec3(Uniforms::COLOR, ObjectsColor);
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
	cleanupCamera();
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
	case 'q':
		MyCamera->addRadius(-0.1f);
		break;
	case 'a':
		MyCamera->addRadius(0.1f);
		break;
	case 'p':
		MyCamera->nextProjection();
		break;
	}
}

int LastMousePosX, LastMousePosY;

void mouse(int button, int state, int x, int y)
{
	LastMousePosX = x;
	LastMousePosY = y;
}

void mouseMotion(int x, int y)
{
	
	MyCamera->addLongitude(-x + LastMousePosX);
	MyCamera->addLatitude(y - LastMousePosY);
	LastMousePosX = x;
	LastMousePosY = y;
}

void mouseWheel(int wheel, int dir, int x, int y)
{
	float incr = dir / 10.0f;
	MyCamera->addZoom(incr);
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
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);
	glutMouseWheelFunc(mouseWheel);
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
	glDepthRange(-1.0, 1.0);
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
	setupCamera();
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