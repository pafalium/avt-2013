//matrices.cpp

#include "matrices.h"
#include <cmath>


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
	Matrix4 translate(Vector3f delta)
	{
		return translate(delta.x(), delta.y(), delta.z());
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
	Matrix4 lookAt(const Vector3f &eye, const Vector3f &center, const Vector3f &up)
	{
		Vector3f view = center - eye;
		view.normalize();
		Vector3f side = view.cross(up);
		side.normalize();
		Vector3f upNorm = side.cross(view);

		std::vector<GLfloat> rotData =
		{ side.x(), upNorm.x(), -view.x(), 0,
		side.y(), upNorm.y(), -view.y(), 0,
		side.z(), upNorm.z(), -view.z(), 0,
		0, 0, 0, 1 };
		Matrix4 rot(rotData);

		Matrix4 trans = translate(Vector3f(0, 0, 0) - eye);

		return rot * trans;
	}
	Matrix4 lookAtInverse(const Vector3f &eye, const Vector3f &center, const Vector3f &up)
	{
		Vector3f view = center - eye;
		view.normalize();
		Vector3f side = view.cross(up);
		side.normalize();
		Vector3f upNorm = side.cross(view);

		std::vector<GLfloat> rotinvData =
		{ side.x(), side.y(), side.z(), 0,
		upNorm.x(), upNorm.y(), upNorm.z(), 0,
		-view.x(), -view.y(), -view.z(), 0,
		0, 0, 0, 1 };
		Matrix4 rotinv(rotinvData);

		Matrix4 transInv = translate(eye);

		return transInv * rotinv;
	}
	Matrix4 orthoProj(GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f)
	{
		GLfloat rml = 1.0 / (r - l);
		GLfloat tmb = 1.0 / (t - b);
		GLfloat fmn = 1.0 / (f - n);

		GLfloat tx = -(r + l)*rml;
		GLfloat ty = -(t + b)*tmb;
		GLfloat tz = -(f + n)*fmn;

		std::vector<GLfloat> ortData =
		{
			2*rml, 0, 0, 0,
			0, 2*tmb, 0, 0,
			0, 0, -2*fmn, 0,
			tx, ty, tz, 1
		};
		return Matrix4(ortData);
	}
	Matrix4 perspectiveProj(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
	{
		GLfloat d = 1.0f / std::tan(fovy / 2.0f);
		GLfloat nmf = 1.0f / (zNear - zFar);
		
		std::vector<GLfloat> perspData =
		{
			d / aspect, 0, 0, 0,
			0, d, 0, 0,
			0, 0, (zFar + zNear)*nmf, -1.0f,
			0, 0, (2 * zFar*zNear)*nmf, 0
		};
		return Matrix4(perspData);
	}
	Matrix4 qGLMatrix(const Quaternion& q) {
		Quaternion qn = qNormalize(q);

		float xx = qn.x * qn.x;
		float xy = qn.x * qn.y;
		float xz = qn.x * qn.z;
		float xt = qn.x * qn.t;
		float yy = qn.y * qn.y;
		float yz = qn.y * qn.z;
		float yt = qn.y * qn.t;
		float zz = qn.z * qn.z;
		float zt = qn.z * qn.t;

		std::vector<GLfloat> matData(16, 0);
		matData[0] = 1.0f - 2.0f * (yy + zz);
		matData[1] = 2.0f * (xy + zt);
		matData[2] = 2.0f * (xz - yt);
		matData[3] = 0.0f;

		matData[4] = 2.0f * (xy - zt);
		matData[5] = 1.0f - 2.0f * (xx + zz);
		matData[6] = 2.0f * (yz + xt);
		matData[7] = 0.0f;

		matData[8] = 2.0f * (xz + yt);
		matData[9] = 2.0f * (yz - xt);
		matData[10] = 1.0f - 2.0f * (xx + yy);
		matData[11] = 0.0f;

		matData[12] = 0.0f;
		matData[13] = 0.0f;
		matData[14] = 0.0f;
		matData[15] = 1.0f;

		Matrix4 matrix(matData);
		matrix.clean();
		return matrix;
	}
}

const Vector3f operator *(const Matrix4 &mat, const Vector3f &vec)
{
	float x = vec.x() * mat.cell(0, 0) + vec.y()*mat.cell(0, 1) + vec.z()*mat.cell(0, 2);
	float y = vec.x() * mat.cell(1, 0) + vec.y()*mat.cell(1, 1) + vec.z()*mat.cell(1, 2);
	float z = vec.x() * mat.cell(2, 0) + vec.y()*mat.cell(2, 1) + vec.z()*mat.cell(2, 2);

	return Vector3f(x, y, z);
}