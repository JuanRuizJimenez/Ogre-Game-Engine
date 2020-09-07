#include "ShipController.h"
#include <GameObject.h>
#include "GameSceneManager.h"
#include "ParticleManager.h"
#include "Messages.h"

//duplicado de shotBehaviour.cpp
void OnShipEnvironmentCollision(GameObject* one, GameObject* other, std::vector<btManifoldPoint*> contactPoints)
{
	//si la nave choca contra un edificio
	//Lo hemos hecho mirando si choca contra un gameobject que no tiene ningún componente de comportamiento
	//Lo ideal sería haber definido una variable para los rigidbody que defina su "layer de colisión"
	//como Enemy, Player, Environment
	//Y con esto comprobar con qué ha chocado
	if (other->getRigidBody() != nullptr  && other->getRigidBody()->isActive() && other->isActive() && one->isActive() && other->getBComponents().size() == 0) {
		Ogre::Vector3 pos = one->getPosition();
		if (one->getBComponents().size() > 0) {
			GameOverMsg msg;
			one->getBComponents()[0]->sendSceneMsg(&msg);
		}
	}
}

ShipController::ShipController(GameObject* gameObject, int _health, std::string _shipName, float _rollingCooldown, int _warningZoneLength, int _deadZoneLength) 
	:BehaviourComponent(gameObject), health(_health), shipName(_shipName), rollingCooldown(_rollingCooldown) , warningZoneLength(_warningZoneLength), deadZoneLength(_deadZoneLength)
{
	keyboard = MainApp::instance()->getKeyboard();
	gameObject->setOrientation(euler);
	rollingTimer = rollingCooldown;
	gameObject->getRigidBody()->setCollisionCallback(OnShipEnvironmentCollision);
}


ShipController::~ShipController()
{
}

void ShipController::Update(float t)
{
	if (keyboard->isKeyDown(OIS::KC_A)) {
		if (euler.mRoll < Radian(1.0)) {
			euler.rotate(Radian(0), Radian(0),Radian(rollVel)*t);
			gameObject->setOrientation(euler);
		}
	}
	else if (keyboard->isKeyDown(OIS::KC_D)) {
		if (euler.mRoll > Radian(-1.0)) {
			euler.rotate(Radian(0), Radian(0), -Radian(rollVel)*t);
			gameObject->setOrientation(euler);
		}
	}
	else 
	{
		if (euler.mRoll > Radian(0.02) && !isRolling)
		{
			euler.rotate(Radian(0), Radian(0), -Radian(rollBackVel)*t);
			gameObject->setOrientation(euler);
		}
		else if (euler.mRoll < Radian(-0.02) && !isRolling)
		{
			euler.rotate(Radian(0), Radian(0), Radian(rollBackVel)*t);
			gameObject->setOrientation(euler);
		}
	}
	rollingTimer -= t;
	if (keyboard->isKeyDown(OIS::KC_E)) {
		if (!isRolling && rollingTimer<0) {
			isRolling = true;
			Message msg = BarrelRollStart();
			sendSceneMsg(&msg);
			rollRight = true;
			iniOrientation = euler.mRoll;
			gameObject->getRigidBody()->setActive(false);
			rollingTimer = rollingCooldown;
		}
	}
	if (keyboard->isKeyDown(OIS::KC_Q) && rollingTimer < 0) {
		if (!isRolling&& rollingTimer < 0) {
			isRolling = true;
			Message msg = BarrelRollStart();
			sendSceneMsg(&msg);
			rollLeft = true;
			iniOrientation = euler.mRoll;
			gameObject->getRigidBody()->setActive(false);
			rollingTimer = rollingCooldown;
		}
	}
	if (isRolling) {
		if (rollRight && euler.mRoll > (-Radian(2 * pi) + iniOrientation) + Radian(2 * pi)*t) {
			euler.rotate(Radian(0), Radian(0), -Radian(barrelVel)*t);
			gameObject->setOrientation(euler);
		}
		else if (rollLeft && euler.mRoll < (Radian(2 * pi) + iniOrientation)- Radian((2 * pi)*t)) {
			euler.rotate(Radian(0), Radian(0), Radian(barrelVel)*t);
			gameObject->setOrientation(euler);
		}
		else {
			isRolling = false;
			Message msg = BarrelRollEnd();
			sendSceneMsg(&msg);
			rollLeft = false;
			rollRight = false;
			gameObject->getRigidBody()->setActive(true);
			euler.mRoll = iniOrientation;
		}
	}

	Vec3 pos = gameObject->getGlobalPosition();
	//Si se sale de la zona de combate manda un mensaje
	if ((pos.x > warningZoneLength || pos.x < -warningZoneLength || pos.z> warningZoneLength || pos.z<-warningZoneLength || pos.y>warningZoneLength) && !warningZone) {
		EnterWarningZone msg;
		sendSceneMsg(&msg);
		warningZone = true;
	}
	else if (!(pos.x > warningZoneLength || pos.x < -warningZoneLength || pos.z> warningZoneLength || pos.z < -warningZoneLength || pos.y>warningZoneLength) && warningZone) {
		//Si vuelve a la zona de combate manda un mensaje
		ExitWarningZone msg;
		sendSceneMsg(&msg);
		warningZone = false;
	}
	if ((pos.x > deadZoneLength || pos.x < -deadZoneLength || pos.z> deadZoneLength || pos.z < -deadZoneLength || pos.y>deadZoneLength || pos.y<=0)) {
		//Si se sale del mapa manda mensaje de que ha muerto
		GameOverMsg msg;
		sendSceneMsg(&msg);
		ExitWarningZone msg2;
		sendSceneMsg(&msg2);
	}
}

void ShipController::reciveMsg(Message * msg)
{
	if (msg->id == "QUITA_VIDA")
	{
		//Si recibe el mensaje  de que ha sufrido daño entonces resta su vida ,
		//Si la vida baja de 0 entonces manda el mensaje de que ha muerto
		DownLifeMsg* dlm = static_cast<DownLifeMsg*>(msg);
		if (dlm->name == "PointerPlayer") {
			health -= dlm->power;
			ISound* aux = SoundManager::instance()->PlaySound2D("HurtPlayer.wav");
			UpdateHealthBarMsg uhb(health);
			sendSceneMsg(&uhb);
			if (health <= 0) {
				GameOverMsg msg;
				sendSceneMsg(&msg);
			}
		}
	}
	else if (msg->id == "GAME_OVER") {
		gameObject->setActive(false);
		MainApp::instance()->getParticleManager()->createParticle(gameObject->getGlobalPosition().getVector(), shipName +"BulletCollision", 6.0f);
	}
}
