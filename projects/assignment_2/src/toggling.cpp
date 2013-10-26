//toggling.cpp

#include "toggling.h"

//class Toggler {
//	TogglerState *m_state;
//	friend class TogglerState;
//	void setState(TogglerState *state);
//public:
//	Toggler(TogglerState *initState);
//	void toggle();
//	SceneConfiguration *currentSceneConfiguration();
//	ShaderProgram *currentTangramShaderProgram();
//};

Toggler::Toggler(TogglerState *initState)
: m_state(initState)
{}

void Toggler::setState(TogglerState *state)
{
	m_state = state;
}

void Toggler::toggle()
{
	m_state->toggle(this);
}

SceneConfiguration *Toggler::currentSceneConfiguration()
{
	return m_state->currentSceneConfiguration();
}

ShaderProgram *Toggler::currentTangramShaderProgram()
{
	return m_state->currentTangramShaderProgram();
}

//class TogglerState {
//protected:
//	void setOwnerState(Toggler *owner, TogglerState *state);
//public:
//	virtual ~TogglerState();
//	virtual void toggle(Toggler *owner);
//	virtual SceneConfiguration *currentSceneConfiguration();
//	virtual ShaderProgram *currentTangramShaderProgram();
//};

void TogglerState::setOwnerState(Toggler *owner, TogglerState *state)
{
	owner->setState(state);
}

///////////////////////////////////////////////////////////////////// STATES

//class TogglerSquare : TogglerState {
//	//TODO initialize to 0
//	static TogglerSquare *Instance;
//public:
//	static TogglerSquare *instance();
//	void toggle(Toggler *owner);
//	SceneConfiguration *currentSceneConfiguration();
//	ShaderProgram *currentTangramShaderProgram();
//};

TogglerSquare *TogglerSquare::Instance = 0;

TogglerSquare *TogglerSquare::instance()
{
	if (Instance == 0)
		Instance = new TogglerSquare();

	return Instance;
}

void TogglerSquare::toggle(Toggler *owner)
{
	setOwnerState(owner, TogglerFigure::instance());
}

SceneConfiguration *TogglerSquare::currentSceneConfiguration()
{
	return &Scenes::SquareTangramConfig;
}

ShaderProgram *TogglerSquare::currentTangramShaderProgram()
{
	return ShaderPrograms::PassThroughProgram;
}

//class TogglerFigure : public TogglerState {
//	//TODO initialize to 0
//	static TogglerFigure *Instance;
//public:
//	static TogglerFigure *instance();
//	void toggle(Toggler *owner);
//	SceneConfiguration *currentSceneConfiguration();
//	ShaderProgram *currentTangramShaderProgram();
//};

TogglerFigure *TogglerFigure::Instance = 0;

TogglerFigure *TogglerFigure::instance()
{
	if (Instance == 0)
		Instance = new TogglerFigure();

	return Instance;
}

void TogglerFigure::toggle(Toggler *owner)
{
	setOwnerState(owner, TogglerSilhouette::instance());
}

SceneConfiguration *TogglerFigure::currentSceneConfiguration()
{
	return &Scenes::FigureTangramConfig;
}

ShaderProgram *TogglerFigure::currentTangramShaderProgram()
{
	return ShaderPrograms::PassThroughProgram;
}

//class TogglerSilhouette : public TogglerState {
//	//TODO initialize to 0
//	static TogglerSilhouette *Instance;
//public:
//	static TogglerSilhouette *instance();
//	void toggle(Toggler *owner);
//	SceneConfiguration *currentSceneConfiguration();
//	ShaderProgram *currentTangramShaderProgram();
//};

TogglerSilhouette *TogglerSilhouette::Instance = 0;

TogglerSilhouette *TogglerSilhouette::instance()
{
	if (Instance == 0)
		Instance = new TogglerSilhouette();

	return Instance;
}

void TogglerSilhouette::toggle(Toggler *owner)
{
	setOwnerState(owner, TogglerSquare::instance());
}

SceneConfiguration *TogglerSilhouette::currentSceneConfiguration()
{
	return &Scenes::FigureTangramConfig;
}

ShaderProgram *TogglerSilhouette::currentTangramShaderProgram()
{
	return ShaderPrograms::MonoChromeProgram;
}


//class TogglerInterpolate : public TogglerState {
//	static TogglerInterpolate *Instance;
//public:
//	static TogglerInterpolate *instance();
//	void toggle(Toggler *owner);
//	SceneConfiguration *currentSceneConfiguration();
//	ShaderProgram *currentTangramShaderProgram();
//};

TogglerInterpolate *TogglerInterpolate::Instance = 0;

TogglerInterpolate *TogglerInterpolate::instance()
{
	if (Instance == 0)
		Instance = new TogglerInterpolate();

	return Instance;
}

void TogglerInterpolate::toggle(Toggler *owner)
{
	setOwnerState(owner, TogglerFigure::instance());
}

SceneConfiguration *TogglerInterpolate::currentSceneConfiguration()
{
	return &Scenes::SqrFigTangramConfig;
}

ShaderProgram *TogglerInterpolate::currentTangramShaderProgram()
{
	return ShaderPrograms::PassThroughProgram;
}