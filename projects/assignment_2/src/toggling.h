//toggling.h

#ifndef __TOGGLING_H__
#define __TOGGLING_H__

#include "sceneConfigurations.h"
#include "ShaderProgram.h"
#include "model_setup.h"
#include "shader_setup.h"


class TogglerState;

class Toggler {
	TogglerState *m_state;
	friend class TogglerState;
	void setState(TogglerState *state);
public:
	Toggler(TogglerState *initState);
	void toggle();
	SceneConfiguration *currentSceneConfiguration();
	ShaderProgram *currentTangramShaderProgram();
};


class TogglerState {
protected:
	void setOwnerState(Toggler *owner, TogglerState *state);
public:
	virtual void toggle(Toggler *owner)=0;
	virtual SceneConfiguration *currentSceneConfiguration()=0;
	virtual ShaderProgram *currentTangramShaderProgram()=0;
};

class TogglerSquare : public TogglerState {
	//TODO initialize to 0
	static TogglerSquare *Instance;
public:
	static TogglerSquare *instance();
	void toggle(Toggler *owner);
	SceneConfiguration *currentSceneConfiguration();
	ShaderProgram *currentTangramShaderProgram();
};

class TogglerFigure : public TogglerState {
	//TODO initialize to 0
	static TogglerFigure *Instance;
public:
	static TogglerFigure *instance();
	void toggle(Toggler *owner);
	SceneConfiguration *currentSceneConfiguration();
	ShaderProgram *currentTangramShaderProgram();
};

class TogglerSilhouette : public TogglerState {
	//TODO initialize to 0
	static TogglerSilhouette *Instance;
public:
	static TogglerSilhouette *instance();
	void toggle(Toggler *owner);
	SceneConfiguration *currentSceneConfiguration();
	ShaderProgram *currentTangramShaderProgram();
};

#endif