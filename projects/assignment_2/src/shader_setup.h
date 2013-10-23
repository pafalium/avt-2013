//shader_setup.h

#ifndef __SHADER_SETUP_H__
#define __SHADER_SETUP_H__

#include "ShaderProgram.h"

namespace ShaderPrograms {

	extern ShaderProgram *PassThroughProgram;
	extern ShaderProgram *MonoChromeProgram;

	void setupPrograms();
	void cleanupPrograms();
}
#endif