
#ifndef __RENDERMODEL_H__
#define __RENDERMODEL_H__

#include <GL/glew.h>
#include <vector>

#include "renderInfo.h"

struct RenderModel {
	GLuint m_vaoName, m_eboName, m_vboName;
	std::vector<Vertex> m_vertices;
	std::vector<GLuint> m_indexes;
public:
	RenderModel(const std::vector<Vertex> &verts, const std::vector<GLuint> &inds);
	void setupModel();
	void cleanupModel();
	void drawModel();
	size_t vertexNum();
	void vaoName(GLuint vao);
	GLuint vaoName();
private:
	//	bool checkIndexConsistent();
};

#endif