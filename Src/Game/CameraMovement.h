#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include <Euler.h>
#include "Messages.h"
class CameraMovement :
	public BehaviourComponent
{
private:
	float horizontalLimit;
	float horizontaVel;
	float horizontalBackVel;
	float verticalLimit;
	float verticalVel;
	float aceleratedCameraOffset;
	float deceleratedCameraOffset;
	float cameraDefaulOffset;
	float cameraAceleratedVel;
	float cameraDeceletatedVel;

	float shakeDuration = 0.0f;
	float maxShakeDuration;
	float shakeAmount;

	bool shake = false;

	Ogre::Vector3 originalPos;
public:
	CameraMovement(GameObject* camera_, GameObject* _target, GameObject* _pivot);
	virtual ~CameraMovement();

	void setInitialValues(float _horizontaLimit, float _horizontalVel, float _horizontalBackVel, float _verticalLimit, float _verticalVel,
		float _aceleratedCameraOffset, float _deceleratedCameraOffset, float _cameraDefaultOffset, float _cameraAcceleratedVel, float _cameraDeceleratedVel,
		float _maxShakeDuration, float _shakeAmount);

	OIS::Keyboard* keyboard;
	GameObject* pivot;
	GameObject* target;

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
};