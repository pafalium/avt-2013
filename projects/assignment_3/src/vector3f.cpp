
#include "Vector3f.h"
#include <cmath>

const GLfloat Vector3f::m_Threshold = 1.0e-5f;

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

Vector3f &Vector3f::operator -= (const Vector3f &other)
{
	m_data[0] -= other.x();
	m_data[1] -= other.y();
	m_data[2] -= other.z();

	return *this;
}
const Vector3f operator-(const Vector3f &vec1, const Vector3f &vec2)
{
	Vector3f result = vec1;
	result -= vec2;
	return result;
}

Vector3f &Vector3f::operator += (const Vector3f &other)
{
	m_data[0] += other.x();
	m_data[1] += other.y();
	m_data[2] += other.z();

	return *this;
}
const Vector3f operator+(const Vector3f &vec1, const Vector3f &vec2)
{
	Vector3f result = vec1;
	result += vec2;
	return result;
}

GLfloat Vector3f::x() const
{
	return m_data[0];
}
GLfloat Vector3f::y() const
{
	return m_data[1];
}
GLfloat Vector3f::z() const
{
	return m_data[2];
}

//norm
GLfloat Vector3f::norm() const
{
	GLfloat quadrance = x()*x() + y()*y() + z()*z();
	return std::sqrt(quadrance);
}
//normalize
void Vector3f::normalize()
{
	GLfloat norma = norm();
	if (norma < m_Threshold)
		return;
	GLfloat factor = 1.0f / norma;
	for (unsigned int i = 0; i < 3; ++i)
		m_data[i] *= factor;
}
//clean (zero fields close to zero)
void Vector3f::clean()
{
	for (unsigned int i = 0; i < 3;++i)
	if (m_data[i] < m_Threshold)
		m_data[i] = 0.0f;
}
//dot product
GLfloat Vector3f::dot(const Vector3f &other) const
{
	return x()*other.x() + y()*other.y() + z()*other.z();
}
//cross product
Vector3f Vector3f::cross(const Vector3f &other) const
{
	GLfloat crossX = -z() *other.y() + y()*other.z();
	GLfloat crossY = z()*other.x() - x()*other.z();
	GLfloat crossZ = -y()*other.x() + x()*other.y();

	return Vector3f(crossX, crossY, crossZ);
}