//shader_setup.h

#ifndef __SHADER_SETUP_H__
#define __SHADER_SETUP_H__

#include "ShaderProgram.h"

namespace ShaderPrograms {

	extern ShaderProgram *PassThroughProgram;
	extern ShaderProgram *MonoChromeProgram;

	extern GLuint SharedMatBufID;

	void sendSharedMatBufMatrix(unsigned int index, const Matrix4& mat);

	void setupSharedMatBuf();
	void cleanupSharedMatBuf();

	void setupPrograms();
	void cleanupPrograms();
}
#endif