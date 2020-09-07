#include "EnemyShoot.h"
#include <ParticleManager.h>


//duplicado de shotBehaviour.cpp
void OnEnemyBulletCollision(GameObject* one, GameObject* other, std::vector<btManifoldPoint*> contactPoints)
{
	//si tiene rigidbody
	if (other->getRigidBody() != nullptr  && other->getRigidBody()->isActive() && other->isActive() && one->isActive()) {
		one->setActive(false);

		Ogre::Vector3 pos = one->getPosition();
		MainApp::instance()->getParticleManager()->createParticle(pos, "EnemyBulletCollision", 1.0f);
	}

	//si es un objeto con comportamiento y con rigidbody activo procesa el choque	
	if (other->getBComponents().size() > 0 && other->getRigidBody()!=nullptr && other->getRigidBody()->isActive()) {
		//Si la bala choca manda mensaje a toda la escena con el nombre de la entidad en este caso sería el player

		int dmg = static_cast<BulletBehaviour*>(one->getBComponents()[0])->getDamage();
		DownLifeMsg Msg(dmg, other->getName());
		//Si la bala choca con una entdiad manda mensaje a toda la escena con el nombre del enemy
		other->getBComponents()[0]->sendSceneMsg(&Msg);
		
	}
}

EnemyShoot::EnemyShoot(GameObject* go, enemyType type_, GameObject* target_, int _damage, int _range, std::string bulletMesh, float shootCd, float _bLifeTime)
	: BehaviourComponent(go), type(type_), target(target_), bulletMeshName(bulletMesh), damage(_damage), range(_range), ShootCd(shootCd), bLifeTime(_bLifeTime)
{
}

EnemyShoot::~EnemyShoot()
{
	target = nullptr;
	while (!bullets.empty())
		bullets.pop_back();
}

bool EnemyShoot::inRange() {
	float distance =gameObject->getGlobalPosition().getVector().distance(target->getGlobalPosition().getVector());
	return range >= distance;
}

void EnemyShoot::Update(float t) {
	auxTemp += t;
	if (auxTemp >= ShootCd) {
		if (inRange()) {
			auxTemp = 0;
			shoot();
			ISound* aux = SoundManager::instance()->PlaySound2D("ShootEnemy.wav");
			aux->setVolume(0.7);
		}
	}
}

void EnemyShoot::reciveMsg(Message * msg)
{
}

void EnemyShoot::shoot() {
	switch (type)
	{
	case groundTurret:
		getBullet(0);
		getBullet(1);
		break;
	case Flyer:
		getBullet();
		break;
	default:
		break;
	}
}

void EnemyShoot::getBullet(int id) {
	int i = 0;
	while (i < bullets.size() && bullets[i]->isActive())
		i++;

	if (i >= bullets.size()) {
		GameObject* nBullet = new GameObject();
		string name = gameObject->getName() + "Bullet" + to_string(i);
		nBullet->createEntity(bulletMeshName, name, MainApp::instance()->getCurrentScene());
		nBullet->setScale(Vec3(1, 1, 3.5));

		string rBulletName = "Rb" + name;
		RigidBody* rBullet = new RigidBody(nBullet, 1, rBulletName, true);
		rBullet->setCollisionCallback(OnEnemyBulletCollision);
		MainApp::instance()->getCurrentScene()->addComponent(rBullet);
		situateBullet(nBullet, id);

		bullets.push_back(nBullet);
		MainApp::instance()->getCurrentScene()->addGameObject(nBullet);
	}
	else { 
		bullets[i]->setActive(true);
		situateBullet(bullets[i], id, true, i); 
	}
}

void EnemyShoot::situateBullet(GameObject*b, int id, bool created, int i) {
	Vec3 dir = gameObject->getDirection();
	b->setDirection(dir);
	int vel = 0;
	switch (type)
	{
	case groundTurret:
		switch (id)
		{
		case 0:
			b->setPosition(MainApp::instance()->getCurrentScene()->getGameObject(gameObject->getName() + "PivotT1")->getGlobalPosition());
			break;
		case 1:
			b->setPosition(MainApp::instance()->getCurrentScene()->getGameObject(gameObject->getName() + "PivotT2")->getGlobalPosition());
			break;
		default:
			break;
		}
		vel = 15;
		break;
	case Flyer:
		b->setPosition(MainApp::instance()->getCurrentScene()->getGameObject(gameObject->getName() + "PivotF1")->getGlobalPosition());
		vel = 30;
		break;
	default:
		break;
	}
	if (created && i >= 0)
		bulletComponents[i]->resetValues(b->getGlobalPosition(), dir);
	else {
		BulletBehaviour* bh = new BulletBehaviour(b, b->getGlobalPosition(), gameObject->getDirection(), damage, bLifeTime);
		bh->setVel(vel);
		MainApp::instance()->getCurrentScene()->addComponent(bh);
		bulletComponents.push_back(bh);
	}
}
