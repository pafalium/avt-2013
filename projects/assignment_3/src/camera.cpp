
#include "camera.h"
#include "utils.h"

//class Projection {
//	bool m_zoomChanged;
//protected:
//	// zoom parameter
//	float m_zoom;
//	bool zoomChanged();
//	void unsetZoomChanged();
//public:
//	Projection(float zoom);
//	virtual ~Projection();
//	virtual const Matrix4 &projMatrix() = 0;
//	void addToZoom(float increment);
//};

Projection::Projection(float zoom) :
m_zoom(zoom), m_zoomChanged(true)
{}

Projection::~Projection()
{}

void Projection::addToZoom(float increment)
{
	m_zoom += increment;
	if (m_zoom < 0.0f)
		m_zoom = 0.1f;
	m_zoomChanged = true;
}

bool Projection::zoomChanged()
{
	return m_zoomChanged;
}

void Projection::unsetZoomChanged()
{
	m_zoomChanged = false;
}

//class OrthoProjection : public Projection {
//	// original scale (zoom used as factor);
//	float m_scale;
//	// x/y
//	float m_aspectRatio;
//	// depth of ortho volume
//	float m_zLength; // equally for both sides
//	// to detect zoom changes without overloading addToZoom
//	float m_lastZoom;
//	// the last calculated matrix
//	Matrix4 m_lastProj;
//public:
//	OrthoProjection(float zoom, float scale, float aspect, float zLength);
//	const Matrix4 &projMatrix();
//};

OrthoProjection::OrthoProjection(float zoom, float scale, float aspect, float zLength)
:Projection(zoom), m_scale(scale), m_aspectRatio(aspect), m_zLength(zLength), m_lastProj(0)
{}

const Matrix4 &OrthoProjection::projMatrix()
{
	if (!zoomChanged())
		return m_lastProj;

	float zoomFactor = 1 / m_zoom;
	float halfHeight = m_scale * zoomFactor;
	float halfWidth = halfHeight * m_aspectRatio;
	float halfDepth = m_zLength / 2.0f;

	m_lastProj = Matrices::orthoProj(-halfWidth, halfWidth, -halfHeight, halfHeight, -halfDepth, halfDepth);
	unsetZoomChanged();

	return m_lastProj;
}

//class PerspectiveProjection : public Projection {
//	// y field of view (zoom used as factor) in degrees
//	float m_fovy;
//	// x/y
//	float m_aspectRatio;
//	// volume depth constraints
//	float m_zNear, m_zFar;
//	// to detect zoom changes
//	float m_lastZoom;
//	// last proj matrix
//	Matrix4 m_lastProj;
//public:
//	PerspectiveProjection(float zoom, float fovy, float aspect, float zNear, float zFar);
//	const Matrix4 &projMatrix();
//};

PerspectiveProjection::PerspectiveProjection(float zoom, float fovy, float aspect, float zNear, float zFar)
:Projection(zoom), m_fovy(fovy), m_aspectRatio(aspect), m_zNear(zNear), m_zFar(zFar), m_lastProj(0)
{}

const Matrix4 &PerspectiveProjection::projMatrix()
{
	if (!zoomChanged())
		return m_lastProj;

	float zoomFactor = 1 / m_zoom;
	float vertFov = deg2Rad(m_fovy * zoomFactor);

	unsetZoomChanged();
	m_lastProj = Matrices::perspectiveProj(vertFov, m_aspectRatio, m_zNear, m_zFar);

	return m_lastProj;
}

//struct Camera {
//	// Current polar coordinates of the camera in degrees.
//	float lat, lon, rad;
//	// Current view matrix based on the camera's current coordinates.
//	Matrix4 viewMat;
//	std::vector<Projection *> m_projections;
//	unsigned int m_currentProjection;
//	// Camera coordinates have changed?
//	bool coordsChanged;
//
//	// Create a camera with an initial projection and position(polar coordinates).
//	Camera(Projection *initProj, Vector3f initRadCoords);
//	// Destroy the camera and its projections
//	~Camera();
//	// Add a new projection mode to this camera.
//	void addProjection(Projection *proj);
//	// Select next projection in cameras projection modes.
//	void nextProjection();
//
//	const Matrix4 &viewMatrix();
//	const Matrix4 &projMatrix();
//
//	void addLongitude(float lon);
//	void addLatitude(float lat);
//	void addRadius(float rad);
//	// Pan the camera by the given vector.
//	void moveCenter(Vector3f delta);
//	// Changes zoom of the current projection by the desired amount.
//	void addZoom(float zoom);
//};

Camera::Camera(Projection *initProj, Vector3f initRadCoords, Vector3f center, Vector3f up)
:lat(initRadCoords.x()), lon(initRadCoords.y()), rad(initRadCoords.z()),
camCenter(center), camUp(up),
coordsChanged(true),viewMat(0),m_projections(),m_currentProjection(0)
{
	m_projections.push_back(initProj);
}

Camera::~Camera()
{
	for (Projection *proj : m_projections)
		delete proj;
}

void Camera::addProjection(Projection *proj)
{
	m_projections.push_back(proj);
}

void Camera::nextProjection()
{
	++m_currentProjection;
	m_currentProjection %= m_projections.size();
}

const Matrix4 &Camera::viewMatrix()
{
	if (!coordsChanged)
		return viewMat;

	float theta = deg2Rad(-lat+90.0f);
	float phi = deg2Rad(lon);
	float x, y, z;
	x = rad * sin(theta) * cos(phi);
	y = rad * sin(theta) * sin(phi);
	z = rad * cos(theta);

	coordsChanged = false;
	viewMat = Matrices::lookAt(Vector3f(x, y, z), camCenter, camUp);

	return viewMat;
}

const Matrix4 &Camera::projMatrix()
{
	return m_projections[m_currentProjection]->projMatrix();
}

void Camera::addLatitude(float inc)
{
	coordsChanged = true;
	lat += inc;
	if (lat > 90.0f)
		lat = 90.0f;
	if (lat < -90.0f)
		lat = -90.0f;
}

void Camera::addLongitude(float inc)
{
	coordsChanged = true;
	lon += inc;
	lon = fmod(lon, 360);
}

void Camera::addRadius(float inc)
{
	coordsChanged = true;
	rad += inc;
	if (rad < 0)
		rad = -rad;//FIXME mwaahahaha
}

void Camera::moveCenter(Vector3f delta)
{
	coordsChanged = true;
	camCenter += delta;
}

void moveCenterView(Vector3f viewDelta)
{
	//TODO
}

void Camera::addZoom(float inc)
{
	m_projections[m_currentProjection]->addToZoom(inc);
}