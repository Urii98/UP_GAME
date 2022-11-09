#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Module.h"

class SceneManager : public Module
{
public:

	SceneManager(bool isActive);

	// Destructor
	virtual ~SceneManager();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void Menu();

private:
	enum Scenes
	{
		NONE,
		LOGO,
		TITLE,
		LEVEL1,
		LEVEL2,
		ENDING,
	} currentScene;

};

#endif // __SCENE_MANAGER_H__