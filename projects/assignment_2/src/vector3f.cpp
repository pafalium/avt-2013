
#include "Vector3f.h"

Vector3f::Vector3f(GLfloat x, GLfloat y, GLfloat z)
:m_data(3)
{
	m_data[0] = x;
	m_data[1] = y;
	m_data[2] = z;
}

Vector3f &Vector3f::operator*=(const GLfloat &scalar)
{
	for (int i = 0; i < 3; ++i)
		m_data[i] *= scalar;

	return *this;
}

const Vector3f operator*(const Vector3f &vec, const GLfloat &scalar)
{
	Vector3f result(vec);
	return (result *= scalar);
}
const Vector3f operator*(const GLfloat &scalar, const Vector3f &vec)
{
	return vec * scalar;
}

GLfloat Vector3f::x()
{
	return m_data[0];
}
GLfloat Vector3f::y()
{
	return m_data[1];
}
GLfloat Vector3f::z()
{
	return m_data[2];
}