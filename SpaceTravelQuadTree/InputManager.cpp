#include "InputManager.h"

void InputManager::keyInput(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	CommandKey command;
	switch (key) {
	case GLFW_KEY_ESCAPE:
		command = CommandKey::ESCAPE;
		break;
	case GLFW_KEY_SPACE:
		command = CommandKey::SPACE;
		break;
	case GLFW_KEY_LEFT:
		command = CommandKey::LEFT;
		break;
	case GLFW_KEY_RIGHT:
		command = CommandKey::RIGHT;
		break;
	case GLFW_KEY_UP:
		command = CommandKey::UP;
		break;
	case GLFW_KEY_DOWN:
		command = CommandKey::DOWN;
		break;
	default:
		command = CommandKey::INVALID;
		break;
	}
	
	if (action == GLFW_REPEAT)
	{
		getInstance().arrayInputState[command] = ActionState::PRESSED;
	}
	else
		getInstance().arrayInputState[command] = action;
}
	
void InputManager::init()
{
	glfwSetKeyCallback(RenderManager::getInstance().getWindow(), keyInput);
}



void InputManager::setInputState(CommandKey key, ActionState s) 
{
	arrayInputState[(int)key] = s;
}

InputManager::InputManager()
{
	for (int i = 0; i < NUMBEROFAVAILABLECOMMANDS; i++) {
		arrayInputState[i] = -1;
	}
}


InputManager::~InputManager()
{
}

