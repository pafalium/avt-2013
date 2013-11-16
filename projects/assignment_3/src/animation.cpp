//animation.cpp

#include "animation.h"

//////////////////////////////////////////////////////////// ANIMATION_MANAGER
AnimationManager::AnimationManager(Scene *scene)
: m_affectedScene(scene), m_activeAnimations(), m_animations()
{}

AnimationManager::~AnimationManager()
{
	for (animationsIterator it = m_animations.begin(); it != m_animations.end(); ++it) {
		delete it->second;
	}
}

void AnimationManager::addAnimation(Animation *animation)
{
	m_animations[animation->name()] = animation;
}

void AnimationManager::activateAnimation(std::string animationName)
{
	animationsIterator it = m_animations.find(animationName);
	if (it == m_animations.end()) {
		//TODO report animation doesn't exist.
		return;
	}
	Animation *anim = it->second;
	m_activeAnimations.insert(anim);
	anim->resetTime();
}

void AnimationManager::update(int elapsedTime)
{
	std::vector<Animation *> finishedAnims;
	for (Animation *anim : m_activeAnimations) {
		anim->update(elapsedTime);
		anim->apply();
		if (anim->hasFinished()) finishedAnims.push_back(anim);
	}
	for (Animation *finishedAnim : finishedAnims) {
		m_activeAnimations.erase(finishedAnim);
	}
}

/////////////////////////////////////////////////////////// ANIMATION
Animation::Animation(std::string name)
: m_name(name), m_objKeySets(), m_currentTime(0), m_hasFinished(false)
{}

Animation::~Animation()
{
	for (objKeySetsIterator it = m_objKeySets.begin();
		it != m_objKeySets.end();
		++it)
	{
		delete it->second;
	}
}

void Animation::addObjectKeyFrame(SceneObject *object, KeyFrame keyframe)
{
	objKeySetsIterator it = m_objKeySets.find(object->name());
	// Object not already in the animation?
	if (it == m_objKeySets.end()) {
		// create set with keyframeset with the keyframe
		ObjectKeyFrameSet *keySet = new ObjectKeyFrameSet(object, keyframe);
		// add keyframeset to the animations keyframeset
		m_objKeySets.insert(std::pair<std::string, ObjectKeyFrameSet *>(object->name(), keySet));
	} else {
		// add the keyframe to the set
		ObjectKeyFrameSet *keySet = it->second;
		keySet->addKeyFrame(keyframe);
	}
}

void Animation::update(int elapsedTime)
{
	bool allFinished = true;
	for (objKeySetsIterator it = m_objKeySets.begin();
		it != m_objKeySets.end();
		++it)
	{
		it->second->update(elapsedTime);
		if (!it->second->hasFinished())
			allFinished = false;
	}
	m_currentTime += elapsedTime;

	m_hasFinished = allFinished;
}

bool Animation::hasFinished() const
{
	return m_hasFinished;
}

void Animation::resetTime()
{
	m_hasFinished = false;
	m_currentTime = 0;
	for (objKeySetsIterator it = m_objKeySets.begin();
		it != m_objKeySets.end();
		++it)
	{
		it->second->resetTime();
	}
}

void Animation::apply()
{
	for (objKeySetsIterator it = m_objKeySets.begin();
		it != m_objKeySets.end();
		++it)
	{
		it->second->apply();
	}
}

const std::string &Animation::name() const
{
	return m_name;
}

//////////////////////////////////////////////////// OBJECT_KEYFRAME_SET
ObjectKeyFrameSet::ObjectKeyFrameSet(SceneObject *affectedObject, KeyFrame initFrame)
: m_affectedObject(affectedObject), m_keyframes(), m_currentTime(0), 
m_lastKeyFrameTime(initFrame.timestamp), m_currentKeyFrame(-1, Vector3f(0, 0, 0), Quaternion())
{
	addKeyFrame(initFrame);
}

void ObjectKeyFrameSet::addKeyFrame(KeyFrame keyframe)
{
	if (m_lastKeyFrameTime < keyframe.timestamp)
		m_lastKeyFrameTime = keyframe.timestamp;
	// if keyframe exists remove it
	keyframesIterator it = m_keyframes.find(keyframe);
	if (it != m_keyframes.end())
		m_keyframes.erase(it);
	// insert keyframe in the set
	m_keyframes.insert(keyframe);
}

void ObjectKeyFrameSet::update(int elapsedTime)
{
	m_currentTime += elapsedTime;
	KeyFrame firstKey = *m_keyframes.begin();
	if (firstKey.timestamp >= m_currentTime) {
		m_currentKeyFrame = firstKey;
		return;
	}
	if (m_lastKeyFrameTime <= m_currentTime) {
		m_currentKeyFrame = *m_keyframes.rbegin();
		return;
	}

	keyframesIterator before, after;
	after = m_keyframes.begin();
	++after;
	before = m_keyframes.begin();
	for (; m_currentTime < before->timestamp /*&& m_currentTime < after->timestamp*/;
		++after, ++before)
		; // empty statement on purpose
	float timeDiff = after->timestamp - before->timestamp;
	float lerpFactor = (m_currentTime-before->timestamp) / timeDiff;
	m_currentKeyFrame.timestamp = before->timestamp;
	m_currentKeyFrame.loc = vec3fLerp(before->loc, after->loc, lerpFactor);
	m_currentKeyFrame.rot = qLerp(before->rot, after->rot, lerpFactor);
}

void ObjectKeyFrameSet::resetTime()
{
	m_currentTime = 0.0f;
}

void ObjectKeyFrameSet::apply()
{
	m_affectedObject->location(m_currentKeyFrame.loc);
	m_affectedObject->rotation(m_currentKeyFrame.rot);
}

bool ObjectKeyFrameSet::hasFinished()
{
	return m_currentTime >= m_lastKeyFrameTime;
}


////////////////////////////////////////////////// KEYFRAME
KeyFrame::KeyFrame(int time, Vector3f locs, Quaternion rots)
:timestamp(time), loc(locs), rot(rots)
{}

bool operator < (const KeyFrame &k1, const KeyFrame &k2)
{
	return k1.timestamp < k2.timestamp;
}