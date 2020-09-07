#pragma once
#include <BehaviourComponent.h>
#include <MainApp.h>
#include <Euler.h>
#include <string>
class ShipController : public BehaviourComponent
{
private:
	Euler euler;
	Ogre::Radian iniOrientation;
	OIS::Keyboard* keyboard;
	float pi = 3.14159265359;
	int health;
	bool isRolling = false;
	float rollingCooldown;
	float rollingTimer;
	bool rollRight = false;
	bool rollLeft = false;
	Real rollVel = pi / 2;
	Real rollBackVel = pi / 4;
	Real barrelVel = 5*pi;
	bool warningZone = false;
	std::string shipName;
	int warningZoneLength;
	int deadZoneLength;
public:
	ShipController(GameObject* gameObject, int _health, std::string _shipName, float _rollingCooldown, int _warningZoneLength, int _deadZoneLength);
	virtual ~ShipController();

	virtual void Update(float t);
	virtual void reciveMsg(Message* msg);
};