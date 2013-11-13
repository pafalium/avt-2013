//shader_setup.cpp

#include <GL/glew.h>

#include "shader_setup.h"
#include "uniformsAttribs.h"
#include "utils.h"

namespace ShaderPrograms {

	ShaderProgram *PassThroughProgram;
	ShaderProgram *MonoChromeProgram;

	GLuint SharedMatBufID = 0;
	const unsigned int Mat4Size = 16 * sizeof(GLfloat);
	const unsigned int SharedMatBufSize = 2 * Mat4Size;

	void setupSharedMatBuf()
	{
		glGenBuffers(1, &SharedMatBufID);
		glBindBuffer(GL_UNIFORM_BUFFER, SharedMatBufID);
		glBufferData(GL_UNIFORM_BUFFER, SharedMatBufSize, 0, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, UniformBlocks::BindPoints::SHARED_MATRICES,SharedMatBufID);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		checkOpenGLError("Error creating uniform buffer object.");
	}

	void cleanupSharedMatBuf()
	{
		glDeleteBuffers(1, &SharedMatBufID);
	}

	void sendSharedMatBufMatrix(unsigned int index, const Matrix4& mat)
	{
		glBindBuffer(GL_UNIFORM_BUFFER, SharedMatBufID);
		glBufferSubData(GL_UNIFORM_BUFFER, Mat4Size*index, Mat4Size, mat.colMajorArray());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void setupPrograms()
	{
		setupSharedMatBuf();

		std::string shaderPath("../src/");
		std::string vertexFile("vertex.vsh"), fragmentFile("fragment.fsh");
		std::string vertexShader = readFromFile(shaderPath + vertexFile);
		std::string fragmentShader = readFromFile(shaderPath + fragmentFile);

		PassThroughProgram = new ShaderProgram();
		PassThroughProgram->addShader(GL_VERTEX_SHADER, vertexShader);
		PassThroughProgram->addShader(GL_FRAGMENT_SHADER, fragmentShader);
		PassThroughProgram->addAttrib(VertexAttribs::Names::POSITION, VertexAttribs::Indexes::POSITION);
		//PassThroughProgram->addAttrib(VertexAttribs::Names::COLOR, VertexAttribs::Indexes::COLOR);
		PassThroughProgram->addAttrib(VertexAttribs::Names::NORMAL, VertexAttribs::Indexes::NORMAL);
		PassThroughProgram->addAttrib(VertexAttribs::Names::TEXCOORD, VertexAttribs::Indexes::TEXCOORD);
		PassThroughProgram->addUniform(Uniforms::MATRIX);
		PassThroughProgram->addUniformBlock(UniformBlocks::Names::SHARED_MATRICES, 
			UniformBlocks::BindPoints::SHARED_MATRICES, SharedMatBufID);
		PassThroughProgram->createCompileLink();

		checkOpenGLError("Error creating pass through porgram.");

		std::string monoChromeFragFile("monoChrom.fsh");
		std::string monoChromeFragShader = readFromFile(shaderPath + monoChromeFragFile);

		MonoChromeProgram = new ShaderProgram();
		MonoChromeProgram->addShader(GL_VERTEX_SHADER, vertexShader);
		MonoChromeProgram->addShader(GL_FRAGMENT_SHADER, fragmentShader);
		MonoChromeProgram->addAttrib(VertexAttribs::Names::POSITION, VertexAttribs::Indexes::POSITION);
		//MonoChromeProgram->addAttrib(VertexAttribs::Names::COLOR, VertexAttribs::Indexes::COLOR);
		MonoChromeProgram->addAttrib(VertexAttribs::Names::NORMAL, VertexAttribs::Indexes::NORMAL);
		MonoChromeProgram->addAttrib(VertexAttribs::Names::TEXCOORD, VertexAttribs::Indexes::TEXCOORD);
		MonoChromeProgram->addUniform(Uniforms::MATRIX);
		MonoChromeProgram->addUniformBlock(UniformBlocks::Names::SHARED_MATRICES,
			UniformBlocks::BindPoints::SHARED_MATRICES, SharedMatBufID);
		MonoChromeProgram->createCompileLink();

		checkOpenGLError("Error creating mono chrome program.");
	}
	void cleanupPrograms()
	{
		delete PassThroughProgram;
		delete MonoChromeProgram;

		cleanupSharedMatBuf();
	}
}