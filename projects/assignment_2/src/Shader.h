
#ifndef __SHADER_H__
#define __SHADER_H__

#include <string>
#include <GL\glew.h>

class Shader
{
public:
	// Issues the creation of a new shader object to OpenGL.
	Shader(GLenum shaderType, const std::string &shaderSource);
	// Issues destruction of the OpenGL shader object associated with this.
	~Shader();
	// Returns OpenGL shader name of this.
	GLuint shaderName();
	// Issues GLSL source compilation.
	// returns GL_COMPILE_STATUS for this shader.
	GLint compile();
	// Fetches the compilation log info for this shader.
	// If the log is empty then returns an empty string.
	std::string compileLogInfo() const;
private:
	GLuint m_shaderName;
	GLenum m_shaderType;
	std::string m_shaderSource;
};

#endif