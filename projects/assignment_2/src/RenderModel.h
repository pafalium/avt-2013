
#ifndef __RENDERMODEL_H__
#define __RENDERMODEL_H__

#include <GL/glew.h>
#include <vector>


struct Vertex {
	GLfloat xyzw[4];
	GLfloat rgba[4];
};

class RenderModel {
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