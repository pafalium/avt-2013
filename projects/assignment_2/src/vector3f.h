//vector3f.h

#ifndef __VECTOR_3F_H__
#define __VECTOR_3F_H__

#include <GL/glew.h>
#include <vector>

class Vector3f {
	std::vector<GLfloat> m_data;

public:
	Vector3f(GLfloat x, GLfloat y, GLfloat z);
	
	Vector3f &operator *= (const GLfloat &scalar);
	friend const Vector3f operator*(const Vector3f &vec, const GLfloat &scalar);
	friend const Vector3f operator*(const GLfloat &scalar, const Vector3f &vec);

	GLfloat x();
	GLfloat y();
	GLfloat z();
};

#endif