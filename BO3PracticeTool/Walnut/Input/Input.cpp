#include "Input.h"

#include "Walnut/Application.h"
#include "SDL.h"

namespace Walnut {

	bool Input::IsKeyDown(KeyCode keycode)
	{
		const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
		return keyboardState[SDL_GetScancodeFromKey((SDL_Keycode)keycode)] == 1;
	}

	bool Input::IsMouseButtonDown(MouseButton button)
	{
		int state = SDL_GetMouseState(NULL, NULL);
		return (state & SDL_BUTTON((int)button)) != 0;
	}

	glm::vec2 Input::GetMousePosition()
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
		return glm::vec2(x, y);
	}

	void Input::SetCursorMode(CursorMode mode)
	{
		SDL_SetRelativeMouseMode(static_cast<SDL_bool>(mode));
	}

}