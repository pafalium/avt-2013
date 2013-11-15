//matrix4.cpp

#include "matrix4.h"
#include <cmath>

const GLfloat Matrix4::m_Threshold = 1.0e-5f;

/////////////////////////////////////////////// CONSTRUCTORS
Matrix4::Matrix4(GLfloat val)
: m_data(16, val)
{}

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
	int currentElem;
	GLfloat sum;
	for (currentElem = 0, sum = 0; currentElem < 4; ++currentElem){
		sum += this->cell(row, currentElem) * other.cell(currentElem, col);
	}
	return sum;
}
//////////////////////////////////////////////// OPERATIONS
void Matrix4::clean()
{
	for (unsigned int i = 0; i < m_data.size(); ++i) {
		if (fabs(m_data[i]) < m_Threshold)
			m_data[i] = 0.0f;
	}
}
/////////////////////////////////////////////// ACCESSORS
GLfloat &Matrix4::cellRef(int row, int col)
{
	return m_data[row + col * 4];
}
const GLfloat &Matrix4::cell(int row, int col) const
{
	return m_data[row + col * 4];
}
void Matrix4::setColumn(int col, const std::vector<GLfloat> &content)
{
	for (int row = 0; row < 4; ++row)
		cellRef(row, col) = content[row];
}
const GLfloat *Matrix4::colMajorArray() const
{
	return m_data.data();
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

