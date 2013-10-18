#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

#include <GL\glew.h>

#include <string>
#include "outputStreams.h"
#include "Shader.h"

class ShaderProgram
{
	GLuint m_programName;
	Shader m_vertexShader;
	Shader m_fragmentShader;
public:
	// Creates a new program and its shaders.
	ShaderProgram(const std::string &vertexSource, const std::string &fragmentSource);
	// Detach shaders, destroy shaders.
	~ShaderProgram();

	//TODO
	// Sets the current OpenGL shader program to this.
	void use();
	//TODO
	// Clear OpenGL program binding.
	// 
	void removeFromUse();
private:
	void displayShaderCompileLog(const std::string &message, const Shader &shader);
	void displayProgramLinkingLog(const std::string &message);
	std::string linkingInfoLog();
	GLint linkProgram();
};


#endif