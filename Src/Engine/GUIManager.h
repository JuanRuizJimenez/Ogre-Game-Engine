#pragma once

#include <MyGUI.h>
#include <MyGUI_OgrePlatform.h>
#include "Scene.h"
//Clase que se encarga de crear objetos GUI y añadirlos a la escena
class GUIManager
{
private:
	static GUIManager* instance_;
	MyGUI::Gui* mGUI;
	MyGUI::OgrePlatform* mPlatform;
public:
	GUIManager();
	~GUIManager();
	void initScene(Scene* escena);
	void resetGUI();
	void createTextbox(string text, int x, int y, int w, int h, string skinName, string name, int fontHeight);
	MyGUI::ImageBox* createImage(string fileName, int x, int y, int w, int h, string skinName, string name, string anchor="UP");
	MyGUI::TextBox* getTextBox(string name);
	MyGUI::ImageBox* getImage(string name);
	void destroyWidget(string name);
	void destroyWidget(MyGUI::Widget* name);
	static GUIManager* instance() {
		if (instance_ == nullptr)
			instance_ = new GUIManager();
		return instance_;
	}
};

