#include "TargetController.h"

TargetController::TargetController(GameObject* target, Ogre::Camera* camera_, string sightPath, string secondSightPath, string GUIName, int w_, int h_): BehaviourComponent(target), camera(camera_), w(w_), h(h_)
{
	newPos = Ogre::Vector2(0, 0);
	sight = GUIManager::instance()->createImage(sightPath, newPos.x, newPos.y, w, h, "ImageBox", GUIName);
	gameObject->getScene()->addGUIObject(sight);
	keyboard = MainApp::instance()->getKeyboard();
	sightName = sightPath;
	secondSightName = secondSightPath;
	screenW = MainApp::instance()->getRenderWindow()->getWidth();
	screenH = MainApp::instance()->getRenderWindow()->getHeight();
}

TargetController::~TargetController()
{
}

void TargetController::Update(float t)
{
	if (getScreenspaceCoords(gameObject->getSceneNode(), newPos)) {
		sight->setVisible(true);
		sight->setPosition(newPos.x, newPos.y);
	}
	else sight->setVisible(false);

	//Auxiliar para determinar si el estado de disparo ha cambiado
	int newState = 0;
	if (keyboard->isKeyDown(OIS::KC_L)) {
		newState = 1;
	}
	else newState = 0;

	//Solo si el estado ha cambiado entonces cambia de sprite
	if (newState != sightState) {
		sightState = newState;
		if (sightState == 0) {
			sight->setImageTexture(sightName);
		}
		else sight->setImageTexture(secondSightName);
	}
}

bool TargetController::getScreenspaceCoords(Ogre::SceneNode* object, Ogre::Vector2& result)
{


	// Get the center point of the object's bounding box
	Ogre::Vector3 point = object->_getDerivedPosition();

	// Is the camera facing that point? If not, return false
	Ogre::Plane cameraPlane = Plane(Ogre::Vector3(camera->getDerivedOrientation().zAxis()), camera->getDerivedPosition());
	if (cameraPlane.getSide(point) != Plane::NEGATIVE_SIDE)
		return false;

	// Transform the 3D point into screen space
	point = camera->getProjectionMatrix() * (camera->getViewMatrix() * point);

	// Transform from coordinate space [-1, 1] to [0, 1] and update in-value
	result.x = screenW*((point.x / 2) + 0.5f)-(w/2);
	result.y = screenH*(-(point.y / 2) + 0.5f)-(h/2);

	return true;
}
