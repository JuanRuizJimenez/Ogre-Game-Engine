#include <BehaviourComponent.h>
#include <MainApp.h>
#include <GUIManager.h>

class TargetController : public BehaviourComponent 
{
public:
	TargetController(GameObject* target, Ogre::Camera* camera_, string sightPath, string secondSightPath, string GUIName, int w_, int h_);
	virtual ~TargetController();
	virtual void Update(float t);
	virtual void reciveMsg(Message* msg) {};
private:
	string sightName;
	string secondSightName;
	MyGUI::ImageBox* sight;
	MyGUI::ImageBox* secondSight;
	Ogre::Camera* camera;
	Ogre::Vector2 newPos;
	OIS::Keyboard* keyboard;
	int screenW;
	int screenH;
	
	int sightState = 0;//0 normal 1 disparando
	int w;
	int h;
	bool getScreenspaceCoords(Ogre::SceneNode* object, Ogre::Vector2& result);
};
