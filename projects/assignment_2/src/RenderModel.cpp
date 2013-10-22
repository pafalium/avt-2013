
#include "RenderModel.h"
#include "uniformsAttribs.h"
#include <iostream>

#include <map>

std::map<std::string, size_t> VertAttrOffsets;

RenderModel::RenderModel(const std::vector<Vertex> &verts, const std::vector<GLuint> &inds) :
m_vertices(verts), m_indexes(inds), m_vaoName(0), m_eboName(0), m_vboName(0)
{}
void RenderModel::setupModel()
{
	//create vertexArray
	glGenVertexArrays(1, &m_vaoName);
	glBindVertexArray(m_vaoName);

	//create buffer (pos,col)
	glGenBuffers(1, &m_vboName);
	glBindBuffer(GL_ARRAY_BUFFER, m_vboName);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);

	
	//set attrib pointers
	glEnableVertexAttribArray(VertexAttribs::Indexes::POSITION);
	glVertexAttribPointer(VertexAttribs::Indexes::POSITION, 4, GL_FLOAT, GL_FALSE, 
		sizeof(Vertex), reinterpret_cast<void *>(offsetof(Vertex, xyzw)));
	glEnableVertexAttribArray(VertexAttribs::Indexes::COLOR);
	glVertexAttribPointer(VertexAttribs::Indexes::COLOR, 4, GL_FLOAT, GL_FALSE, 
		sizeof(Vertex), reinterpret_cast<void *>((offsetof(Vertex, rgba))));

	//create index buffer
	glGenBuffers(1, &m_eboName);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboName);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*m_indexes.size(), m_indexes.data(), GL_STATIC_DRAW);

	//unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(VertexAttribs::Indexes::POSITION);
	glDisableVertexAttribArray(VertexAttribs::Indexes::COLOR);
}
void RenderModel::drawModel() const
{
	//prepare
	glBindVertexArray(m_vaoName);

	//draw
	glDrawElements(GL_TRIANGLES, m_indexes.size(), GL_UNSIGNED_INT, 0);

	//unbind
	glBindVertexArray(0);
}
void RenderModel::cleanupModel()
{
	glDeleteVertexArrays(1, &m_vaoName);
	glDeleteBuffers(1, &m_vboName);
	glDeleteBuffers(1, &m_eboName);
}

void RenderModel::vaoName(GLuint vao)
{
	m_vaoName = vao;
}
GLuint RenderModel::vaoName()
{
	return m_vaoName;
}
size_t RenderModel::vertexNum()
{
	return m_vertices.size();
}