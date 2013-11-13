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
		//extern const std::string COLOR;
		extern const std::string NORMAL;
		extern const std::string TEXCOORD;
	}
	namespace Indexes {
		extern const GLuint POSITION;
		//extern const GLuint COLOR;
		extern const GLuint NORMAL;
		extern const GLuint TEXCOORD;
	}
}

namespace Uniforms {
	extern const std::string MATRIX;
}

namespace UniformBlocks {
	namespace Names {
		extern const std::string SHARED_MATRICES;
	}
	namespace BindPoints {
		extern const GLuint SHARED_MATRICES;
	}
}

extern std::set<VertexAttribChannel> VertexAttribChannels;

extern std::set<std::string> UniformNames;

#endif