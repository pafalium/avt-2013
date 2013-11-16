//animation.h

#pragma once

#include <map>
#include <set>

#include "scene.h"

//TODO don't forget to define the Animation's comparator.
//TODO don't forget to define all the comparators...

// Actually more like keyframe set
class Animation;

// AnimationManager updates it's current active animations until they
// finish.
// AnimationManager is responsible for it's animations' lifecycle.
class AnimationManager {
	// The Scene that is changed when the active animations are updated.
	Scene *m_affectedScene;
	// Active animations (that are currently used to update the scene).
	std::set<Animation *> m_activeAnimations;
	// All the animations currently stored in the manager.
	// Animations are uniquely identified by their name.
	std::map<std::string, Animation *> m_animations;
public:
	AnimationManager(Scene *scene);
	// Destroy the AnimationManager along with it's Animations.
	~AnimationManager();
	// Add an Animation to the manager.
	// There shouldn't already be an animation with the same name
	// in the manager. If there is it might be lost.
	void addAnimation(Animation *animation);
	// Activate the Animation with the given name.
	// If the Animation is already activated it is restarted.
	void activateAnimation(std::string animationName);
	// Update the manager's active animations.
	// Time is given in miliseconds.
	void update(int elapsedTime);
private:
	typedef std::map<std::string, Animation *>::iterator animationsIterator;
};

class ObjectKeyFrameSet;
struct KeyFrame;

class Animation {
	// The Animation unique name (within the AnimationManager).
	std::string m_name;
	// The keyframes for each object affected by this animation.
	std::map<std::string, ObjectKeyFrameSet *> m_objKeySets;
	// The animation current time.
	int m_currentTime;
	// Did the last update reach the end of the animation?
	bool m_hasFinished;
public:
	// Create an empty Animation with a name.
	Animation(std::string name);
	// Delete this Animation and all it's keyframesets.
	~Animation();
	// Add a keyframe for the provided sceneObject.
	// If the object is not already in this animation a new keyframe set 
	// is created for it.
	void addObjectKeyFrame(SceneObject *object, KeyFrame keyframe);
	// Advance this animation time by the given time.
	// Time is given in miliseconds. It is expected to be positive.
	void update(int elapsedTime);
	// Check if either the animation reached it's last keyframe or not.
	bool hasFinished() const;
	// Restart this Animation's time.
	void resetTime();
	// Apply the Animation's current state to the associated SceneObjects.
	void apply();

	// The name of this Animation.
	const std::string &name() const;
	// Apply the Animation current state to the given Scene.
	// Only objects in the Scene are modified (if some keyframeset refers to
	// non-existent SceneObject nothing is done).
	//void applyToScene(Scene *scene);
private:
	typedef std::map<std::string, ObjectKeyFrameSet *>::iterator objKeySetsIterator;
};

struct KeyFrame {
	KeyFrame(int time, Vector3f locs, Quaternion rots);
	int timestamp;
	Vector3f loc;
	Quaternion rot;
};
bool operator< (const KeyFrame &k1, const KeyFrame &k2);

class ObjectKeyFrameSet {
	// The affected SceneObject.
	SceneObject *m_affectedObject;
	// The key frame set. :P
	std::set<KeyFrame> m_keyframes;
	// Time of the current 'update'.
	int m_currentTime;
	// (interpolated) keyframe for the currentTime.
	KeyFrame m_currentKeyFrame;
	// Last keyframe time :P
	//TODO remove this time since it can be obtained using rbegin.
	int m_lastKeyFrameTime;
public:
	// Create a new ObjectKeyFrameSet for a SceneObject containing one Keyframe.
	// The KeyFrame is not necessarily the first Keyframe.
	ObjectKeyFrameSet(SceneObject *affectedObject, KeyFrame initFrame);
	// Add a keyframe to this set.
	// If there is already a keyframe with the same time the existing
	// keyframe is replaced.
	void addKeyFrame(KeyFrame keyframe);
	// Determine the intermediare attributes to be applyed to the SceneObject
	// considering that elapsedTime miliseconds have passed since last time.
	void update(int elapsedTime);
	// Reset keyframeset timeline.
	void resetTime();
	// Update SceneObject's attributes with those from this set at the given time.
	void apply();
	// Was the last keyframe reached?
	bool hasFinished();
private:
	typedef std::set<KeyFrame>::iterator keyframesIterator;
};

