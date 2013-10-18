//matrices_tests.cpp

#include <iostream>
#include "matrices.h"
#include "matrices_tests.h"

static const GLfloat PI = 3.14159265359;

void testIdentity(){
	Matrix4 I = Matrices::identity();

	std::cout << I << std::endl;
}

void testTranslate() {
	Matrix4 Trans = Matrices::translate(5, 5, 5);

	std::cout << Trans << std::endl;
}

void testScale() {
	Matrix4 Sca = Matrices::scale(.5, 2, -.5);

	std::cout << Sca << std::endl;
}

void testRotate() {
	Matrix4 rotX90 = Matrices::axisRotate(1, 0, 0, PI / 2.0);
	Matrix4 rotY90 = Matrices::axisRotate(0, 1, 0, PI / 2.0);
	Matrix4 rotZ90 = Matrices::axisRotate(0, 0, 1, PI / 2.0);

	std::cout << "X rot 90" << std::endl;
	std::cout << rotX90 << std::endl;
	std::cout << "Y rot 90" << std::endl;
	std::cout << rotY90 << std::endl;
	std::cout << "Z rot 90" << std::endl;
	std::cout << rotZ90 << std::endl;
}

void testIdentityMultiply() {
	Matrix4 I = Matrices::identity();

	std::cout << "I x I =" << std::endl;
	std::cout << (I * I) << std::endl;

	Matrix4 T = Matrices::translate(1, 0, -2);
	std::cout << "I x T =" << std::endl;
	std::cout << I*T << std::endl;

	std::cout << "T x I =" << std::endl;
	std::cout << T*I << std::endl;
}

void testRotateTranslate() {
	Matrix4 Rot = Matrices::axisRotate(0, 0, 1, PI);
	Matrix4 Tra = Matrices::translate(1, 0, -2);

	std::cout << "Rot:" << std::endl;
	std::cout << Rot << std::endl;

	std::cout << "Tra:" << std::endl;
	std::cout << Tra << std::endl;

	std::cout << "Rot x Tra:" << std::endl;
	std::cout << Rot * Tra << std::endl;

	std::cout << "Tra x Rot" << std::endl;
	std::cout << Tra * Rot << std::endl;
}

void testMatrixMultiply() {
	testIdentityMultiply();

	testRotateTranslate();
}

void testMatrices() {
	std::cout << "Testing Identity" << std::endl;
	testIdentity();
	std::cout << "Testing Translate" << std::endl;
	testTranslate();
	std::cout << "Testing Scale" << std::endl;
	testScale();
	std::cout << "Testing Rotate" << std::endl;
	testRotate();
	std::cout << "Testing Identity Multiply" << std::endl;
	testIdentityMultiply();
	std::cout << "Testing Rotate Translate Multiplication" << std::endl;
	testRotateTranslate();
}