//matrix4.h

#pragma once

#include <GL\glew.h>
#include <vector>
#include <iostream>

// Representation of a 4x4 matrix.
// Will be represented as column major internaly.
// Can accept row major representation on construction.
class Matrix4 {
	static const GLfloat m_Threshold;

	std::vector<GLfloat> m_data;

public:
	Matrix4(GLfloat val);
	Matrix4(std::vector<GLfloat> elems, bool isElemsRowMajor = false);

	//this method will NOT verify that indexes are within bounds.
	const GLfloat &cell(int row, int col) const;
	//set a column to a given vector of 4 GLfloats
	void setColumn(int col, const std::vector<GLfloat> &content);
	// Get column major array
	const GLfloat *colMajorArray() const;

	Matrix4 &operator*=(const Matrix4 &other);
	const Matrix4 operator*(const Matrix4 &other) const;

	void clean();

	// Print matrix at row/line pattern.
	friend std::ostream &operator<< (std::ostream &o, const Matrix4 &mat);
private:
	//zero initialize constructor
	Matrix4();
	void transpose();
	GLfloat rowColMultiply(int row, const Matrix4 &other, int col) const;
	GLfloat &cellRef(int row, int col);
};