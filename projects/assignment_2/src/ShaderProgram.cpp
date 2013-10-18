#include "ShaderProgram.h"

#include <iostream>

#include "uniformsAttribs.h"

ShaderProgram::ShaderProgram(const std::string &vertexSource, const std::string &fragmentSource)
: m_vertexShader(GL_VERTEX_SHADER, vertexSource), 
m_fragmentShader(GL_FRAGMENT_SHADER, fragmentSource)
{
	m_programName = glCreateProgram();

	// attach shaders
	glAttachShader(m_programName, m_vertexShader.shaderName());
	glAttachShader(m_programName, m_fragmentShader.shaderName());

	bool shaderCompilationFailed = false;
	// compile vertex shader
	GLint vertexCompStatus = m_vertexShader.compile();
	if (vertexCompStatus == GL_FALSE){
		shaderCompilationFailed = true;
		displayShaderCompileLog("Vertex Shader compilation failed", m_vertexShader);
	}
	// compile fragment shader
	GLint fragmentCompStatus = m_fragmentShader.compile();
	if (fragmentCompStatus == GL_FALSE){
		shaderCompilationFailed = true;
		displayShaderCompileLog("Fragment Shader compilation failed", m_fragmentShader);
	}
	// fail if some compilation failed
	OutStreams::ShaderLog << "ERROR: Shader compilation failed" << std::endl;

	// "bind" vertex attribute channels and get uniform ids
	for (VertexAttribChannel vac : VertexAttribChannels){
		glBindAttribLocation(m_programName, vac.attribIndex, vac.attribName.c_str());
	}
	// link program and check for errors
	GLint linkResult = linkProgram();
	if (linkResult == GL_FALSE) {
		displayProgramLinkingLog("Shader Program linking failed");
	}

	// get uniform ids
	//TODO
}


ShaderProgram::~ShaderProgram()
{
	glUseProgram(0);
	glDetachShader(m_programName, m_vertexShader.shaderName());
	glDetachShader(m_programName, m_fragmentShader.shaderName());

	glDeleteProgram(m_programName);
}

GLint ShaderProgram::linkProgram()
{
	// Link program
	glLinkProgram(m_programName);
	// Get GL_LINK_STATUS
	GLint linkResult = GL_FALSE;
	glGetProgramiv(m_programName, GL_LINK_STATUS, &linkResult);
	return linkResult;
}

std::string ShaderProgram::linkingInfoLog()
{
	// Get log info size
	GLint logInfoSize;
	glGetProgramiv(m_programName, GL_INFO_LOG_LENGTH, &logInfoSize);
	// Get shader compilation info log string
	if (logInfoSize == 0)
		return std::string();
	// FIXME using a char array for interfacing with glGetProgramInfoLog
	char *logInfoArray = new char[logInfoSize];
	glGetProgramInfoLog(m_programName, logInfoSize, 0, logInfoArray);
	std::string logInfoString(logInfoArray);
	delete[] logInfoArray;
	return logInfoString;
}

void ShaderProgram::displayProgramLinkingLog(const std::string &message)
{
	OutStreams::ShaderLog << message << std::endl;
	OutStreams::ShaderLog << "Printing info log:" << std::endl;
	OutStreams::ShaderLog << linkingInfoLog() << std::endl;
}


void ShaderProgram::displayShaderCompileLog(const std::string &message, const Shader &shader)
{
	OutStreams::ShaderLog << message << std::endl;
	OutStreams::ShaderLog << "Printing info log:" << std::endl;
	OutStreams::ShaderLog << shader.compileLogInfo() << std::endl;
}