//Quaternion.h

#pragma once

//TODO converter importar isto para a codebase
//TODO separar definição de matrizes de fabrico de matrizes

///////////////////////////////////////////////////////////////////////
//
// Using quaternions to rotate in 3D.
//
// (c) 2013 by Carlos Martinho
//
///////////////////////////////////////////////////////////////////////

#include <string>

#include "vector3f.h"

#define DEGREES_TO_RADIANS 0.01745329251994329547
#define RADIANS_TO_DEGREES 57.29577951308232185913


/////////////////////////////////////////////////////////////////////// QUATERNION

typedef struct {
	float t, x, y, z;
} Quaternion;

const float qThreshold = (float)1.0e-5;
const Quaternion qFromAngleAxis(float theta, Vector3f axis);
const void qToAngleAxix(const Quaternion& q, float& theta, Vector3f& axis);
const void qClean(Quaternion& q);
const float qQuadrance(const Quaternion& q);
const float qNorm(const Quaternion& q);
const Quaternion qNormalize(const Quaternion& q);
const Quaternion qConjugate(const Quaternion& q);
const Quaternion qInverse(const Quaternion& q);
const Quaternion qMultiply(const Quaternion& q, const float s);
const Quaternion qMultiply(const Quaternion& q0, const Quaternion& q1);
const Quaternion qLerp(const Quaternion& q0, const Quaternion& q1, float k);
const Quaternion qSlerp(const Quaternion& q0, const Quaternion& q1, float k);
const bool qEqual(const Quaternion& q0, const Quaternion& q1);
//const void qPrint(const std::string& s, const Quaternion& q);
//const void qPrintAngleAxis(const std::string& s, const Quaternion& q);

