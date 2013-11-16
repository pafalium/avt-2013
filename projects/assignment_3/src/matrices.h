
#ifndef __MATRICES_H__
#define __MATRICES_H__

#include <GL/glew.h>
#include <iostream>

#include "matrix4.h"
#include "vector3f.h"
#include "quaternion.h"

namespace Matrices {
	// Axis-angle rotation matrix. Rodrigues formula.
	// theta given in radians
	Matrix4 axisRotate(GLfloat x, GLfloat y, GLfloat z, GLfloat theta);
	Matrix4 translate(GLfloat dx, GLfloat dy, GLfloat dz);
	Matrix4 translate(Vector3f delta);
	Matrix4 scale(GLfloat sx, GLfloat sy, GLfloat sz);
	Matrix4 identity();
	Matrix4 lookAt(const Vector3f &eye, const Vector3f &center, const Vector3f &up);
	Matrix4 lookAtInverse(const Vector3f &eye, const Vector3f &center, const Vector3f &up);
	Matrix4 orthoProj(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f);
	//Perspective projection matrix. fovy is in radians.
	Matrix4 perspectiveProj(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);
	Matrix4 qGLMatrix(const Quaternion& q);
}

// multiply the given vector by the upper right 3x3 matrix of the given matrix
const Vector3f operator *(const Matrix4 &mat, const Vector3f &vec);


#endif