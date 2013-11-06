//uniformsAttribs.h

#ifndef __UNIFORMS_ATTRIBS_H__
#define __UNIFORMS_ATTRIBS_H__

#include <GL/glew.h>

#include <string>
#include <set>

struct VertexAttribChannel {
	std::string attribName;
	GLuint attribIndex;
	VertexAttribChannel(std::string name, GLuint index);
	bool operator<(const VertexAttribChannel& other) const;
};

namespace VertexAttribs {
	namespace Names {
		extern const std::string POSITION;
		extern const std::string COLOR;
	}
	namespace Indexes {
		extern const GLuint POSITION;
		extern const GLuint COLOR;
	}
}

namespace Uniforms {
	extern const std::string MATRIX;
}

extern std::set<VertexAttribChannel> VertexAttribChannels;

extern std::set<std::string> UniformNames;

#endif