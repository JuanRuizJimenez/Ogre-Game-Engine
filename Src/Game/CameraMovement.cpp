#include "CameraMovement.h"
#include <GameObject.h>
CameraMovement::CameraMovement(GameObject * camera_, GameObject* _target, GameObject* _pivot) :BehaviourComponent(camera_),target(_target), pivot(_pivot)
{
	keyboard = MainApp::instance()->getKeyboard();
}

CameraMovement::~CameraMovement()
{
}

void CameraMovement::setInitialValues(float _horizontaLimit, float _horizontalVel, float _horizontalBackVel, float _verticalLimit, float _verticalVel, float _aceleratedCameraOffset, float _deceleratedCameraOffset, float _cameraDefaultOffset, float _cameraAcceleratedVel, float _cameraDeceleratedVel, float _maxShakeDuration, float _shakeAmount)
{
	 horizontalLimit = _horizontaLimit ;
	 horizontaVel = _horizontalVel;
	 horizontalBackVel = _horizontalBackVel;
	 verticalLimit = _verticalLimit;
	 verticalVel = _verticalVel;
	 aceleratedCameraOffset = _aceleratedCameraOffset;
	 deceleratedCameraOffset = _deceleratedCameraOffset;
	 cameraDefaulOffset = _cameraDefaultOffset;
	 cameraAceleratedVel = _cameraAcceleratedVel;
	 cameraDeceletatedVel = _cameraDeceleratedVel;

	 //Auxiliar para el contador del temblor
	 shakeDuration = 0.0f;
	 maxShakeDuration = _maxShakeDuration;
	
	 shakeAmount = _shakeAmount;
}




void CameraMovement::Update(float t)
{

	if (keyboard->isKeyDown(OIS::KC_A)) {
		if (gameObject->getPosition().x > -horizontalLimit) {
			gameObject->translate(Vec3(-horizontalLimit * horizontaVel * t, 0, 0));
		}
	}
	else if (keyboard->isKeyDown(OIS::KC_D)) {
		if (gameObject->getPosition().x < horizontalLimit) {
			gameObject->translate(Vec3(horizontalLimit* horizontaVel * t, 0, 0));
		}

	}
	else {
		if (gameObject->getPosition().x > 1) {
			gameObject->translate(Vec3(-horizontalBackVel * t, 0,0));
		}
		else if (gameObject->getPosition().x < -1) {
			gameObject->translate(Vec3(horizontalBackVel * t, 0,0));
		}
	}
	if (keyboard->isKeyDown(OIS::KC_W)) {
		if (gameObject->getPosition().y > -verticalLimit) {
			gameObject->translate(Vec3(0, -verticalLimit*verticalVel* t, 0));
		}
		if (pivot->getPosition().y < verticalLimit){
			pivot->translate(Vec3(0, verticalLimit*verticalVel * t, 0));
		}
	}
	else if (keyboard->isKeyDown(OIS::KC_S)){

		if (gameObject->getPosition().y < verticalLimit){
			gameObject->translate(Vec3(0, verticalLimit* verticalVel * t, 0));
		}
		if (pivot->getPosition().y > -verticalLimit) {
			pivot->translate(Vec3(0,-verticalLimit*verticalVel* t, 0));
		}

	}

	if (keyboard->isKeyDown(OIS::KC_SPACE)) {
		if (gameObject->getPosition().z < aceleratedCameraOffset) {
			gameObject->translate(Vec3(0, 0, cameraAceleratedVel * t));

		}
	}
	else if (keyboard->isKeyDown(OIS::KC_C)) {
		if (gameObject->getPosition().z > deceleratedCameraOffset) {
			gameObject->translate(Vec3(0, 0, -cameraDeceletatedVel * t));

		}
	}
	else
	{
		if (gameObject->getPosition().z > cameraDefaulOffset +1) {
			gameObject->translate(Vec3(0, 0, -cameraAceleratedVel * t));

		}
		else if (gameObject->getPosition().z < cameraDefaulOffset -1) {
			gameObject->translate(Vec3(0, 0, cameraDeceletatedVel * t));

		}
	}
	if (shake) {
		if (shakeDuration > 0)
		{
			int x = rand() % 2 -1.0;
			int y = rand() % 2 -1.0 ;
			int z = rand() % 2 - 1.0
				;
			Ogre::Vector3 newPos = gameObject->getPosition() + Ogre::Vector3(x, y, z);
			gameObject->setPosition(newPos);

			shakeDuration -= t;
		}
		else {
			shake = false;
			gameObject->setPosition(originalPos);
		}

	}
	Euler e;
	e.fromQuaternion(gameObject->getOrientation());
	e.mRoll = 0;
	gameObject->setOrientation(e);
	gameObject->lookAt(pivot->getGlobalPosition());

}

void CameraMovement::reciveMsg(Message * msg)
{
	if (msg->id == "GAME_OVER") {
		gameObject->setActive(false);
	}
	else if (msg->id == "QUITA_VIDA")
	{
		DownLifeMsg* dlm = static_cast<DownLifeMsg*>(msg);
		if (dlm->name == "PointerPlayer") {
			if (!shake) {
				//La camara tiembla para dar mas feedback
				shakeDuration = maxShakeDuration;
				shake = true;
				originalPos = gameObject->getPosition();
			}
		}
	}
}