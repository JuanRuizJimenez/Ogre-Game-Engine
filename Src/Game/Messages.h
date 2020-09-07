#pragma once
#include <MainApp.h>
#include <BasicComponent.h>

struct TiltCameraBackMsg : public Message
{
	Radian radian;

	TiltCameraBackMsg(string id, Radian negativeRadian) :Message(id), radian(negativeRadian) {};
};
struct InitSceneMsg : public Message {
	InitSceneMsg(string id = "INIT_SCENE") : Message(id) {};
};
struct EnterWarningZone : public Message {
	EnterWarningZone(string id = "ENTER_WARNING_ZONE") : Message(id) {};
};
struct ExitWarningZone : public Message {
	ExitWarningZone(string id = "EXIT_WARNING_ZONE") : Message(id) {};
};
struct DownLifeMsg : public Message
{
	int power;
	string name;

	DownLifeMsg(int p, string n, string id = "QUITA_VIDA") : Message(id), power(p), name(n) {}
};
struct EnemyDeadMsg : public Message {
	EnemyDeadMsg(string id = "ENEMY_DEAD") : Message(id) {};
};
struct OverloadedMsg : public Message {
	bool isOverloaded;
	OverloadedMsg(bool is, string id = "OVERLOADED") : Message(id), isOverloaded(is) {}
};
struct UpdateHealthBarMsg : public Message {
	int currentHealth;
	UpdateHealthBarMsg(int _currentHealth, string id = "UPDATE_HEALTHBAR") : Message(id), currentHealth(_currentHealth) {};
};
struct UpdateOverloadBarMsg : public Message {
	float currentOverload;
	UpdateOverloadBarMsg(float _currentOverload, string id = "UPDATE_OVERLOADBAR") : Message(id), currentOverload(_currentOverload) {};
};
struct BulletCollisionMsg : public Message {
	Ogre::Vector3 pos;
	BulletCollisionMsg(Ogre::Vector3 position,string id = "BULLET_COLLISION") : Message(id), pos(position) {}
};
struct GameOverMsg : public Message
{
	GameOverMsg(string id = "GAME_OVER") : Message(id) {}
};
struct MissionAccomplishedMsg : public Message {
	MissionAccomplishedMsg(string id = "MISSION_ACCOMPLISHED") : Message(id){}
};
struct VictoryMsg : public Message {
	VictoryMsg(string id = "VICTORY") : Message(id) {}
};
struct NextLevelMsg : public Message
{
	NextLevelMsg(string id = "NEXT_LEVEL") : Message(id) {}
};
struct BarrelRollStart : public Message
{
	BarrelRollStart(string id = "BARREL_START") : Message(id) {}
};
struct BarrelRollEnd : public Message
{
	BarrelRollEnd(string id = "BARREL_END") : Message(id) {}
};