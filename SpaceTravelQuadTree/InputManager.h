#pragma once
#include "RenderManager.h"

#define NUMBEROFAVAILABLECOMMANDS 7

class InputManager
{
private:
	InputManager();
	InputManager(InputManager const&) {};
	void operator=(InputManager const&) {};
	int arrayInputState[NUMBEROFAVAILABLECOMMANDS];
	
public:
	static InputManager& getInstance()
	{
		static InputManager instance;
		return instance;
	}
	~InputManager();

	enum ActionState{
		RELEASED = 0,
		PRESSED = 1,
		INACTIVE = -1
	};
	enum CommandKey{
		ESCAPE,
		SPACE,
		RIGHT,
		LEFT,
		DOWN,
		UP,
		INVALID
	};

	int getInputState(CommandKey key){ return arrayInputState[key]; }
	void setInputState(CommandKey key, ActionState s);
	void init();

	//CHANGE THIS-> Vita PORT!
	static void keyInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	void pollEvents(){ glfwPollEvents(); }
};

