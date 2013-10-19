
#ifndef __MATRICES_H__
#define __MATRICES_H__

#include <GL/glew.h>
#include <vector>
#include <iostream>

// Representation of a 4x4 matrix.
// Will be represented as column major internaly.
// Can accept row major representation on construction.
class Matrix4 {
	std::vector<GLfloat> m_data;

public:
	Matrix4(std::vector<GLfloat> elems, bool isElemsRowMajor=false);

	//this method will NOT verify that indexes are within bounds.
	const GLfloat &cell(int row, int col) const;
	//set a column to a given vector of 4 GLfloats
	void setColumn(int col, const std::vector<GLfloat> &content);

	Matrix4 &operator*=(const Matrix4 &other);
	const Matrix4 operator*(const Matrix4 &other) const;

	// Print matrix at row/line pattern.
	friend std::ostream &operator<< (std::ostream &o, const Matrix4 &mat);
private:
	//zero initialize constructor
	Matrix4();
	void transpose();
	GLfloat rowColMultiply(int row, const Matrix4 &other, int col) const;
	GLfloat &cellRef(int row, int col);
};

namespace Matrices {
	// Axis-angle rotation matrix. Rodrigues formula.
	// theta given in radians
	Matrix4 axisRotate(GLfloat x, GLfloat y, GLfloat z, GLfloat theta);
	Matrix4 translate(GLfloat dx, GLfloat dy, GLfloat dz);
	Matrix4 scale(GLfloat sx, GLfloat sy, GLfloat sz);
	Matrix4 identity();
	//Matrix4 lookAt();
}


#endif