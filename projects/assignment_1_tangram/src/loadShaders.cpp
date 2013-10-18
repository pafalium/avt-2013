
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include "loadShaders.h"

int max(int n1, int n2)
{
	return n1 > n2 ? n1 : n2;
}

std::string readSourceFromFile(const std::string &file_path)
{
	std::ostringstream shaderCodeStream;
	std::ifstream shaderStream(file_path, std::ios::in);
	if (shaderStream.is_open())
	{
		std::string Line;
		while (getline(shaderStream, Line))
			shaderCodeStream << Line << std::endl;
		shaderStream.close();
	}

	return shaderCodeStream.str();
}

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string vShaderSource = readSourceFromFile(vertex_file_path);

	// Read the Fragment Shader code from the file
	std::string fShaderSource = readSourceFromFile(fragment_file_path);

	// Output read files... (NOT COOL WITH BIG SHADER FILES)
	std::cout << "Fragment code:" << std::endl;
	std::cout << vShaderSource << std::endl;
	std::cout << "Vertex code:" << std::endl;
	std::cout << fShaderSource << std::endl;

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	std::cout << "Compiling shader : " << vertex_file_path << std::endl;
	char const * VertexSourcePointer = vShaderSource.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	char *VertexShaderErrorMessage = new char[max(InfoLogLength, int(1))];
	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	std::cout << std::string(VertexShaderErrorMessage) << std::endl;
	delete[] VertexShaderErrorMessage;

	// Compile Fragment Shader
	std::cout << "Compiling shader : " << fragment_file_path << std::endl;
	char const * FragmentSourcePointer = fShaderSource.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	char *FragmentShaderErrorMessage = new char[max(InfoLogLength,int(1))];
	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	std::cout << std::string(FragmentShaderErrorMessage) << std::endl;
	delete[] FragmentShaderErrorMessage;

	// Link the program
	std::cout << "Linking program" << std::endl;
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	char *ProgramErrorMessage = new char[max(InfoLogLength, int(1))];
	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	std::cout << std::string(ProgramErrorMessage) << std::endl;
	delete[] ProgramErrorMessage;

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}