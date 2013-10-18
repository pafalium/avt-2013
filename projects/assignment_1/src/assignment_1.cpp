///////////////////////////////////////////////////////////////////////
//
// Assignment 1 consists in the following:
//
// - Download the libraries: Glew and FreeGlut for your system.
// - Create a project to compile, link and run the code provided in this section in your favourite programming environment (classes will use VS2010).
// - Update your graphics drivers to their latest versions.
// - Verify what OpenGL contexts your computer can support, focusing on OpenGL 2.1, OpenGL 3.2, OpenGL 3.3.
//
// Further suggestions to verify your understanding of the concepts explored in lab 1:
//
// - Change the program so display is called at 60 FPS.
//
// (c)2013 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////
//
// Added preprocessor constants to control display calling policy.
// To run at a constant display rate define CONSTANT_FPS and 
// DISPLAY_MILISEC with the desired display interval.
//
// OpenGL context version: 4.2
//
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>
#include <string>

#include "GL/glew.h"
#include "GL/freeglut.h"

///////////////////////////////////////////////////////////////////////

//#define CONSTANT_FPS
//#define DISPLAY_MILISEC 1000/60

#define CAPTION "Hello Blank World"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Draw something
	glutSwapBuffers();
}

void idle()
{
#ifndef CONSTANT_FPS
	glutPostRedisplay();
#endif
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

#ifdef CONSTANT_FPS
void displayTimer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(DISPLAY_MILISEC, displayTimer, 0);
}
#endif

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() 
{
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(0,timer,0);

#ifdef CONSTANT_FPS
	glutTimerFunc(0, displayTimer, 0);
#endif
}

void setupOpenGL() {
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glEnable(GL_DEPTH_TEST);
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
	
	glutInitContextVersion(4, 2);
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
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();	
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////