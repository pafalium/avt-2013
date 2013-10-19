//matrices.cpp

#include "matrices.h"
#include <cmath>

/////////////////////////////////////////////// CONSTRUCTORS
Matrix4::Matrix4(std::vector<GLfloat> elems, bool isElemsRowMajor)
: m_data(16)
{
	for (int i = 0; i < 16; ++i)
		m_data[i] = elems[i];

	if (isElemsRowMajor)
		transpose();
}
Matrix4::Matrix4()
:m_data(16, (GLfloat)0)
{}
/////////////////////////////////////////////// OPERATORS
const Matrix4 Matrix4::operator*(const Matrix4 &other) const
{
	Matrix4 result;
	int row, col;
	for (row = 0; row < 4; ++row)
	for (col = 0; col < 4; ++col){
		result.cellRef(row, col) = rowColMultiply(row, other, col);
	}
	return result;
}
Matrix4 &Matrix4::operator*=(const Matrix4 &other)
{
	m_data = (*this * other).m_data;
	
	return *this;
}
/////////////////////////////////////////////// AUXILIARIES
void Matrix4::transpose(){
	std::vector<GLfloat> oldElems(m_data);

	for (int oldrow = 0; oldrow < 4; ++oldrow)
	{
		for (int oldcol = 0; oldcol < 4; ++oldcol)
		{
			m_data[oldcol * 4 + oldrow] = oldElems[oldrow * 4 + oldcol];
		}
	}
}
GLfloat Matrix4::rowColMultiply(int row, const Matrix4 &other, int col) const
{
	int currentElem, sum;
	for (currentElem = 0, sum = 0; currentElem < 4; ++currentElem){
		sum += this->cell(row, currentElem) * other.cell(currentElem, col);
	}
	return sum;
}
/////////////////////////////////////////////// ACCESSORS
GLfloat &Matrix4::cellRef(int row, int col)
{
	return m_data[row * 4 + col];
}
const GLfloat &Matrix4::cell(int row, int col) const
{
	return m_data[row * 4 + col];
}
void Matrix4::setColumn(int col, const std::vector<GLfloat> &content)
{
	for (int row = 0; row < 4; ++row)
		cellRef(row, col) = content[row];
}
/////////////////////////////////////////////// FRIENDS
std::ostream &operator<<(std::ostream &o, const Matrix4 &mat)
{
	//foreach row, fetch elements
	for (int row = 0; row < 4; ++row){
		for (int col = 0; col < 4; ++col){
			o << mat.cell(row, col) << " ";
		}
		o << std::endl;
	}
	return o;
}

/////////////////////////////////////////////// MATRIX GENERATORS
namespace Matrices {
	Matrix4 identity()
	{
		std::vector<GLfloat> content =
		{ 1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };
		return Matrix4(content);
	}
	Matrix4 translate(GLfloat dx, GLfloat dy, GLfloat dz) 
	{
		Matrix4 mat = identity();
		std::vector<GLfloat> transVec = { dx, dy, dz, 1.0f };
		mat.setColumn(3, transVec);

		return mat;
	}
	Matrix4 scale(GLfloat sx, GLfloat sy, GLfloat sz)
	{
		std::vector<GLfloat> content =
		{ sx, 0, 0, 0,
		0, sy, 0, 0,
		0, 0, sz, 0,
		0, 0, 0, 1 };
		return Matrix4(content);
	}
	Matrix4 axisRotate(GLfloat x, GLfloat y, GLfloat z, GLfloat theta)
	{
		GLfloat cosThe = cos(theta), sinThe = sin(theta);
		GLfloat oneSubCos = 1 - cosThe;
		std::vector<GLfloat> content =
		{ cosThe + x*x*oneSubCos, x*y*oneSubCos - z*sinThe, x*z*oneSubCos + y*sinThe, 0,
		x*y*oneSubCos + z*sinThe, cosThe + y*y*oneSubCos, y*z*oneSubCos - x*sinThe, 0,
		x*z*oneSubCos - y*sinThe, y*z*oneSubCos + x*sinThe, cosThe + z*z*oneSubCos, 0,
		0, 0, 0, 1};

		return Matrix4(content, true);
	}
}