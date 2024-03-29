#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"
#include "SceneEnding.h"
#include "SceneLogo.h"
#include "SceneTitle.h"
#include "FadeToBlack.h"
#include "SceneManager.h"
#include "Pathfinding.h"
#include "ModuleFonts.h"
#include "GuiManager.h"

#include <memory>

#include "Defs.h"
#include "Log.h"

#include "Optick/include/optick.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;
	

	input = new Input(true);
	win = new Window(true);
	render = new Render(true);
	tex = new Textures(true);
	audio = new Audio(true);
	//L07 DONE 2: Add Physics module
	physics = new Physics(true);
	pathfinding = new PathFinding(true);
	sceneManager = new SceneManager(true);

	sceneLogo = new SceneLogo(true);
	sceneTitle = new SceneTitle(false);

	scene = new Scene(false);
	sceneEnding = new SceneEnding(true);
	entityManager = new EntityManager(false);
	map = new Map(false);
	fade = new FadeToBlack(true);
	fonts = new ModuleFonts(true);
	guiManager = new GuiManager(true);


	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);

	//L07 DONE 2: Add Physics module
	AddModule(physics);  //20 de memoria (hasta aqui) + modulos fade y render
	AddModule(pathfinding);
	AddModule(sceneManager);
	
	AddModule(sceneLogo); //7 de memoria x si solo  --> nse pq no se eliminan aunq he probado con breakpoint y se llama a delete texture y funciona normal
	AddModule(sceneTitle); //450 de memoria x si solo --> podr�a estar dejando 11mb de memory leaks
	
	
	AddModule(scene); //junto a entitiy manager: 112
	AddModule(sceneEnding);//3 de memoria x si solo
	AddModule(entityManager);
	AddModule(map); //5 con scene active

	// Render last to swap buffer
	AddModule(fade);
	AddModule(fonts);
	AddModule(guiManager);
	AddModule(render);
	
	
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	//aqui pone todos los modulos a active, creo que esto tendria q comentarlo y que el active
	//vaya en funcion de lo que le pase yo al constructor aqui
	module->Init();
				
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	timer = Timer();

	bool ret = false;

	// L01: DONE 3: Load config from XML
	ret = LoadConfig();

	if (ret == true)
	{
		title = configNode.child("app").child("title").child_value(); // L01: DONE 4: Read the title from the config file

		maxFrameDuration = configNode.child("app").child("frcap").attribute("value").as_int();

		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			// L01: DONE 5: Add a new argument to the Awake method to receive a pointer to an xml node.
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			pugi::xml_node node = configNode.child(item->data->name.GetString());
			ret = item->data->Awake(node);
			item = item->next;
		}
	}
	LOG("---------------- Time Awake: %f/n", timer.ReadMSec());

	return ret;
}

// Called before the first frame
bool App::Start()
{
	dt = 0;
	timer.Start();
	startupTime.Start();
	lastSecFrameTime.Start();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		if (item->data->active == true)
		{

			ret = item->data->Start();
			

		}
		item = item->next;
	}

	LOG("----------------- Time Start(): %f", timer.ReadMSec());

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if (ret == true)
		ret = PreUpdate();

	if (ret == true)
		ret = DoUpdate();

	if (ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
bool App::LoadConfig()
{
	bool ret = false;

	// L01: DONE 3: Load config.xml file using load_file() method from the xml_document class
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");

	// L01: DONE 3: Check result for loading errors
	if (parseResult) {
		ret = true;
		configNode = configFile.child("config");
	}
	else {
		LOG("Error in App::LoadConfig(): %s", parseResult.description());
	}

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	OPTICK_EVENT();
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	OPTICK_EVENT();
	// L03: DONE 1: This is a good place to call Load / Save methods
	if (loadGameRequested == true) LoadFromFile();
	if (saveGameRequested == true) SaveToFile();

	// Amount of frames since startup
	frameCount++;
	// Amount of time since game start (use a low resolution timer)
	secondsSinceStartup = startupTime.ReadSec();
	// Amount of ms took the last update
	dt = frameTime.ReadMSec();
	// Amount of frames during the last second
	lastSecFrameCount++;

	if (lastSecFrameTime.ReadMSec() > 1000) {
		lastSecFrameTime.Start();
		framesPerSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
		// Average FPS for the whole game life
		averageFps = (averageFps + framesPerSecond) / 2;
	}

	// L14: TODO 2: Use SDL_Delay to make sure you get your capped framerate
	// L14: TODO 3: Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected

	float delay = float(maxFrameDuration) - dt;

	PerfTimer delayTimer = PerfTimer();
	delayTimer.Start();
	if (maxFrameDuration > 0 && delay > 0) {
		SDL_Delay(delay);
		LOG("We waited for %f milliseconds and the real delay is % f", delay, delayTimer.ReadMs());
		dt = maxFrameDuration;
	}
	else {
		LOG("No wait");
	}

	float expectedFrames = (1.0 / (float)maxFrameDuration * 1000.0);

	// Shows the time measurements in the window title
	static char title[256];
	sprintf_s(title, 256, "FPS: %.2f / Av.FPS: %.2f / Last-frame MS: %.3f / Vsync: %s ",
		expectedFrames, averageFps, dt, render->isVsync ? "ON" : "OFF");

	//std::cout << "dt: " << dt << std::endl;

	app->win->SetTitle(title);
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	OPTICK_EVENT();
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	OPTICK_EVENT();
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt/1000);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	OPTICK_EVENT();
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for (item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if (pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if (index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// L02: DONE 1: Implement methods to request load / save and methods 
// for the real execution of load / save (to be implemented in TODO 5 and 7)
void App::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() 
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;
}


// L02: DONE 5: Implement the method LoadFromFile() to actually load a xml file
// then call all the modules to load themselves
bool App::LoadFromFile()
{
	bool ret = true;

	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (result == NULL)
	{
		LOG("Could not load xml file savegame.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL && ret == true)
		{
			ret = item->data->LoadState(gameStateFile.child("save_state").child(item->data->name.GetString()));
			item = item->next;
		}
	}

	loadGameRequested = false;

	return ret;
}

bool App::LoadFromFileCheckPastGame()
{
	bool ret = true;

	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (result == NULL)
	{
		LOG("Could not load xml file savegame.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		ListItem<Module*>* item;
		item = modules.start;

		while (item != NULL)
		{
			if (item->data->name != "window")
			{
				if (item->data->name == "entitymanager")
				{
					ret = item->data->LoadState(gameStateFile.child("save_state").child(item->data->name.GetString()));
				}

				item = item->next;
				continue;
			}
			ret = item->data->LoadState(gameStateFile.child("save_state").child(item->data->name.GetString()));
			break;
		}
	}

	loadGameRequested = false;

	return ret;
}

// L02: DONE 7: Implement the xml save method SaveToFile() for current state
// check https://pugixml.org/docs/quickstart.html#modify
bool App::SaveToFile() 
{
	bool ret = false;

	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node saveStateNode = saveDoc->append_child("save_state");

	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL)
	{
		ret = item->data->SaveState(saveStateNode.append_child(item->data->name.GetString()));
		item = item->next;
	}

	ret = saveDoc->save_file("save_game.xml");

	saveGameRequested = false;

	return ret;
}