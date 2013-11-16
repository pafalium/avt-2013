
#pragma once

#include <vector>
#include "matrices.h"
#include "Vector3f.h"

class Projection {
	bool m_zoomChanged;
protected:
	// zoom parameter
	float m_zoom;
	bool zoomChanged();
	void unsetZoomChanged();
public:
	Projection(float zoom);
	virtual ~Projection();
	virtual const Matrix4 &projMatrix() = 0;
	void addToZoom(float increment);
};

class OrthoProjection : public Projection {
	// original scale (zoom used as factor);
	float m_scale;
	// x/y
	float m_aspectRatio;
	// depth of ortho volume
	float m_zLength; // equally for both sides
	// the last calculated matrix
	Matrix4 m_lastProj;
public:
	OrthoProjection(float zoom, float scale, float aspect, float zLength);
	const Matrix4 &projMatrix();
};

class PerspectiveProjection : public Projection {
	// y field of view (zoom used as factor) in degrees
	float m_fovy;
	// x/y
	float m_aspectRatio;
	// volume depth constraints
	float m_zNear, m_zFar;
	// last calculated matrix
	Matrix4 m_lastProj;
public:
	PerspectiveProjection(float zoom, float fovy, float aspect, float zNear, float zFar);
	const Matrix4 &projMatrix();
};

struct Camera {
	// Current polar coordinates of the camera. Angles in degrees.
	float lat, lon, rad;
	// Cartesian coordinates of the center which the camera is rotating around.
	// Cartesian coordinates of the camera's up vector.
	Vector3f camCenter, camUp;
	// Current view matrix based on the camera's current coordinates.
	Matrix4 viewMat;
	std::vector<Projection *> m_projections;
	unsigned int m_currentProjection;
	// Camera coordinates have changed?
	bool coordsChanged;

	// Create a camera with an initial projection and position(polar coordinates).
	Camera(Projection *initProj, Vector3f initRadCoords, Vector3f center, Vector3f up);
	// Destroy the camera and its projections
	~Camera();
	// Add a new projection mode to this camera.
	void addProjection(Projection *proj);
	// Select next projection in cameras projection modes.
	void nextProjection();

	const Matrix4 &viewMatrix();
	const Matrix4 &projMatrix();

	// Longitude cycles in -360,360
	void addLongitude(float lon);
	// Latitude caped at -90,90.
	void addLatitude(float lat);
	void addRadius(float rad);
	// Pan the camera by the given vec3.
	void moveCenter(const Vector3f &delta);
	// Pan the camera by the given vec3 which is in view coordinates.
	void moveCenterView(const Vector3f &viewDelta);
	// Calculate the current camera position in world coordinates.
	Vector3f currentPosition();
	// Changes zoom of the current projection by the desired amount.
	void addZoom(float zoom);
};