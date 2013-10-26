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
	Vector3f &operator -= (const Vector3f &other);
	friend const Vector3f operator-(const Vector3f &vec1, const Vector3f &vec2);
	Vector3f &operator += (const Vector3f &other);
	friend const Vector3f operator+(const Vector3f &vec1, const Vector3f &vec2);

	GLfloat x() const;
	GLfloat y() const;
	GLfloat z() const;
};

const Vector3f operator*(const Vector3f &vec, const GLfloat &scalar);
const Vector3f operator*(const GLfloat &scalar, const Vector3f &vec);
const Vector3f operator-(const Vector3f &vec1, const Vector3f &vec2);
const Vector3f operator+(const Vector3f &vec1, const Vector3f &vec2);

#endif