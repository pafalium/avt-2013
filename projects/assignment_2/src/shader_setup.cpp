//shader_setup.cpp

#include "shader_setup.h"
#include "readFile.h"

namespace ShaderPrograms {

	ShaderProgram *PassThroughProgram;
	ShaderProgram *MonoChromeProgram;

	void setupPrograms()
	{
		std::string shaderPath("../src/");
		std::string vertexFile("vertex.vsh"), fragmentFile("fragment.fsh");
		std::string vertexShader = readFromFile(shaderPath + vertexFile);
		std::string fragmentShader = readFromFile(shaderPath + fragmentFile);

		PassThroughProgram = new ShaderProgram(vertexShader, fragmentShader);
		PassThroughProgram->createCompileLink();

		std::string monoChromeFragFile("monoChrom.fsh");
		std::string monoChromeFragShader = readFromFile(shaderPath + monoChromeFragFile);

		MonoChromeProgram = new ShaderProgram(vertexShader, monoChromeFragShader);
		MonoChromeProgram->createCompileLink();
	}
	void cleanupPrograms()
	{
		delete PassThroughProgram;
		delete MonoChromeProgram;
	}
}