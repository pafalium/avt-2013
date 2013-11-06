#ifndef __SHADER_PROGRAM_H__
#define __SHADER_PROGRAM_H__

#include <GL\glew.h>

#include <string>
#include <map>
#include "outputStreams.h"
#include "Shader.h"


class ShaderProgram
{
	GLuint m_programName;
	Shader m_vertexShader;
	Shader m_fragmentShader;
	std::map<std::string, GLint> m_uniformIds;
public:
	// Creates a new program and its shaders (no OpenGL calls).
	ShaderProgram(const std::string &vertexSource, const std::string &fragmentSource);
	// Detach shaders, destroy shaders.
	~ShaderProgram();
	// Create, compile and link program.
	// After calling this mehtod the program is/should be ready for execution.
	// TODO uniform ids are not being gathered.
	void createCompileLink();
	// Returns this program's OpenGL name.
	GLuint programName();
	// Sets the current OpenGL shader program to this.
	void use();
	// Clear OpenGL program binding.
	void removeFromUse();
	// Get a uniform id by uniform name.
	GLint getUniformId(const std::string &uniformName);
private:
	void displayShaderCompileLog(const std::string &message, const Shader &shader);
	void displayProgramLinkingLog(const std::string &message);
	std::string linkingInfoLog();
	GLint linkProgram();
};


#endif