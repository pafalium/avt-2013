//quaternion.cpp

#include "quaternion.h"

const Quaternion qFromAngleAxis(float theta, Vector3f axis)
{
	Vector3f axisn = axis;
	axisn.normalize();

	Quaternion q;
	float a = theta * (float)DEGREES_TO_RADIANS;
	q.t = cos(a / 2.0f);
	float s = sin(a / 2.0f);
	q.x = axisn.x() * s;
	q.y = axisn.y() * s;
	q.z = axisn.z() * s;

	qClean(q);
	return qNormalize(q);
	//return q;
}

const void qToAngleAxis(const Quaternion& q, float& theta, Vector3f& axis)
{
	Quaternion qn = qNormalize(q);
	theta = 2.0f * acos(qn.t) * (float)RADIANS_TO_DEGREES;
	float s = sqrt(1.0f - qn.t*qn.t);
	if (s < qThreshold) {
		axis = Vector3f(1.0f, 0.0f, 0.0f);
	}
	else {
		float sinv = 1.0f / s;
		axis = Vector3f(qn.x*sinv, qn.y*sinv, qn.z*sinv);
	}
}

const void qClean(Quaternion& q)
{
	if (fabs(q.t) < qThreshold) q.t = 0.0f;
	if (fabs(q.x) < qThreshold) q.x = 0.0f;
	if (fabs(q.y) < qThreshold) q.y = 0.0f;
	if (fabs(q.z) < qThreshold) q.z = 0.0f;
}

const float qQuadrance(const Quaternion& q)
{
	return q.t*q.t + q.x*q.x + q.y*q.y + q.z*q.z;
}

const float qNorm(const Quaternion& q)
{
	return sqrt(qQuadrance(q));
}

const Quaternion qNormalize(const Quaternion& q)
{
	float s = 1 / qNorm(q);
	return qMultiply(q, s);
}

const Quaternion qConjugate(const Quaternion& q)
{
	Quaternion qconj = { q.t, -q.x, -q.y, -q.z };
	return qconj;
}

const Quaternion qInverse(const Quaternion& q)
{
	return qMultiply(qConjugate(q), 1.0f / qQuadrance(q));
}

const Quaternion qAdd(const Quaternion& q0, const Quaternion& q1)
{
	Quaternion q;
	q.t = q0.t + q1.t;
	q.x = q0.x + q1.x;
	q.y = q0.y + q1.y;
	q.z = q0.z + q1.z;
	return q;
}

const Quaternion qMultiply(const Quaternion& q, const float s)
{
	Quaternion sq;
	sq.t = s * q.t;
	sq.x = s * q.x;
	sq.y = s * q.y;
	sq.z = s * q.z;
	return sq;
}

const Quaternion qMultiply(const Quaternion& q0, const Quaternion& q1)
{
	Quaternion q;
	q.t = q0.t * q1.t - q0.x * q1.x - q0.y * q1.y - q0.z * q1.z;
	q.x = q0.t * q1.x + q0.x * q1.t + q0.y * q1.z - q0.z * q1.y;
	q.y = q0.t * q1.y + q0.y * q1.t + q0.z * q1.x - q0.x * q1.z;
	q.z = q0.t * q1.z + q0.z * q1.t + q0.x * q1.y - q0.y * q1.x;
	return q;
}

const Quaternion qLerp(const Quaternion& q0, const Quaternion& q1, float k)
{
	float cos_angle = q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t;
	float k0 = 1.0f - k;
	float k1 = (cos_angle > 0) ? k : -k;
	return qAdd(qMultiply(q0, k0), qMultiply(q1, k1));
}

const Quaternion qSlerp(const Quaternion& q0, const Quaternion& q1, float k)
{
	float angle = acos(q0.x*q1.x + q0.y*q1.y + q0.z*q1.z + q0.t*q1.t);
	float k0 = sin((1 - k)*angle) / sin(angle);
	float k1 = sin(k*angle) / sin(angle);
	return qAdd(qMultiply(q0, k0), qMultiply(q1, k1));
}

const bool qEqual(const Quaternion& q0, const Quaternion& q1)
{
	return (fabs(q0.t - q1.t) < qThreshold && fabs(q0.x - q1.x) < qThreshold &&
		fabs(q0.y - q1.y) < qThreshold && fabs(q0.z - q1.z) < qThreshold);
}

//const void qPrint(const std::string& s, const Quaternion& q)
//{
//	std::cout << s << " = (" << q.t << ", " << q.x << ", " << q.y << ", " << q.z << ")" << std::endl;
//}

//const void qPrintAngleAxis(const std::string& s, const Quaternion& q)
//{
//	std::cout << s << " = ";
//
//	float thetaf;
//	Vector axis_f;
//	qToAngleAxis(q, thetaf, axis_f);
//	std::cout << "angle = " << thetaf << " ";
//	vPrint("axis", axis_f);
//}