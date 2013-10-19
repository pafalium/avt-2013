//uniformsAttribs.h

#ifndef __UNIFORMS_ATTRIBS_H__
#define __UNIFORMS_ATTRIBS_H__

#include <GL/glew.h>

#include <string>
#include <set>

struct VertexAttribChannel {
	std::string attribName;
	GLuint attribIndex;
	VertexAttribChannel(std::string name, GLuint index) 
		:attribName(name), attribIndex(index){}
	bool operator<(const VertexAttribChannel& other) const{
		return attribName < other.attribName;
	}
};

std::set<VertexAttribChannel> VertexAttribChannels =
{ VertexAttribChannel("in_Position", 0), VertexAttribChannel("in_Color", 1) };

std::set<std::string> UniformNames =
{};

#endif