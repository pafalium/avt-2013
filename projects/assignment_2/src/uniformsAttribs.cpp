//uniformsAttribs.cpp

#include "uniformsAttribs.h"

VertexAttribChannel::VertexAttribChannel(std::string name, GLuint index)
:attribName(name), attribIndex(index){}

bool VertexAttribChannel::operator<(const VertexAttribChannel& other) const{
	return attribName < other.attribName;
}

namespace VertexAttribs {
	namespace Names {
		const std::string POSITION = "in_Position";
		const std::string COLOR = "in_Color";
	}
	namespace Indexes {
		const GLuint POSITION = 0;
		const GLuint COLOR = 1;
	}
}

namespace Uniforms {
	const std::string MATRIX = "Matrix";
}

std::set<VertexAttribChannel> VertexAttribChannels =
{
	VertexAttribChannel(VertexAttribs::Names::POSITION, VertexAttribs::Indexes::POSITION),
	VertexAttribChannel(VertexAttribs::Names::POSITION, VertexAttribs::Indexes::COLOR)
};

std::set<std::string> UniformNames = {Uniforms::MATRIX};