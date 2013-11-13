//camera_setup
#pragma once

#include "camera.h"

const Vector3f CameraInitCoords(0.0f, 0.0f, 2.0f);
Camera *MyCamera;

void setupCamera()
{
	PerspectiveProjection *persproj = new PerspectiveProjection(1.0f, 90.0f, 4.0f / 3.0f, 0.0f, 10.0f);
	OrthoProjection *orthoproj = new OrthoProjection(1.0f, 2.0f, 4.0f / 3.0f, 10.0f);

	MyCamera = new Camera(persproj, CameraInitCoords, Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0, 0, 1));
	MyCamera->addProjection(orthoproj);

	MyCamera->nextProjection();
}

void cleanupCamera()
{
	delete MyCamera;
}