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
// MODIFIED FOR TANGRAM EXERCISE
//
///////////////////////////////////////////////////////////////////////
#include <iostream>
#include <sstream>
#include <string>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "loadShaders.h"
#include "model_setup.h"

///////////////////////////////////////////////////////////////////////

//#define CONSTANT_FPS
//#define DISPLAY_MILISEC 16

namespace FilePaths {

	const std::string SHADER_PATH("../src/");
	const std::string VSHADER_FILE("vertex.vsh");
	const std::string FSHADER_FILE("fragment.fsh");

	const std::string VSHADER_PATH(SHADER_PATH + VSHADER_FILE);
	const std::string FSHADER_PATH(SHADER_PATH + FSHADER_FILE);

}

#define CAPTION "Hello Blank World"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

GLuint ShaderProgramID;

// Added when following a tut
GLuint VertexArrayID;
// This will identify our vertex buffer
GLuint Vertexbuffer;

void drawTriangle()
{
	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, Vertexbuffer);
	//glBindVertexArray(VertexArrayID);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle

	glDisableVertexAttribArray(0);
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup()
{
	//destroy triangle
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteBuffers(1, &Vertexbuffer);
	//destroy shader program
	glDeleteProgram(ShaderProgramID);

	Models::cleanupModels();
}

void display()
{
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Draw something

	//drawTriangle();

	Models::SquareModel.drawModel();
	//Models::BigTriModel.drawModel();

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

void setupTriangle()
{
	//GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
	};

	// This will identify our vertex buffer
	//GLuint vertexbuffer;

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &Vertexbuffer);

	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, Vertexbuffer);

	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void setupObjects()
{
	setupTriangle();
	Models::setupModels();
}

void setupShaderProgram()
{
	ShaderProgramID = LoadShaders(FilePaths::VSHADER_PATH.c_str(), FilePaths::FSHADER_PATH.c_str());
	glUseProgram(ShaderProgramID);
}

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
	setupObjects();
	setupShaderProgram();
	setupCallbacks();
}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();	
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////