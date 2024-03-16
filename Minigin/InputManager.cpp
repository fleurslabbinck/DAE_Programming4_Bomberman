#include "InputManager.h"

int dae::InputManager::controllerCount{};

void dae::InputManager::Init(int nrControllers, int nrGamepads)
{
	for (int i{}; i < nrControllers; ++i)
	{
		m_controllers.push_back(Controller());
	}

	for (int i{}; i < nrGamepads; ++i)
	{
		const Gamepad gamepad{ Gamepad(i) };
		m_gamepads.push_back(gamepad);
	}
}

void dae::InputManager::BindCommand(dae::GameObject* gameObject)
{
	m_controllers[controllerCount].Bind(gameObject);
	++controllerCount;
}

bool dae::InputManager::ProcessInput()
{
	UpdateGamepadInput();
	UpdateKeyboardInput();
	ExecuteCommands();

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return true;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
		}
		// etc...
	}

	return false;
}

void dae::InputManager::UpdateGamepadInput()
{
	for (Gamepad& gamepad : m_gamepads)
	{
		if (gamepad.UpdateButtons())
		{
			if (gamepad.IsDownThisFrame(XINPUT_GAMEPAD_DPAD_LEFT)) m_controllers[gamepad.GetPlayerIdx()].moveLeft.Enable();
			if (gamepad.IsUpThisFrame(XINPUT_GAMEPAD_DPAD_LEFT)) m_controllers[gamepad.GetPlayerIdx()].moveLeft.Disable();

			if (gamepad.IsDownThisFrame(XINPUT_GAMEPAD_DPAD_RIGHT)) m_controllers[gamepad.GetPlayerIdx()].moveRight.Enable();
			if (gamepad.IsUpThisFrame(XINPUT_GAMEPAD_DPAD_RIGHT)) m_controllers[gamepad.GetPlayerIdx()].moveRight.Disable();

			if (gamepad.IsDownThisFrame(XINPUT_GAMEPAD_DPAD_DOWN)) m_controllers[gamepad.GetPlayerIdx()].moveDown.Enable();
			if (gamepad.IsUpThisFrame(XINPUT_GAMEPAD_DPAD_DOWN)) m_controllers[gamepad.GetPlayerIdx()].moveDown.Disable();

			if (gamepad.IsDownThisFrame(XINPUT_GAMEPAD_DPAD_UP)) m_controllers[gamepad.GetPlayerIdx()].moveUp.Enable();
			if (gamepad.IsUpThisFrame(XINPUT_GAMEPAD_DPAD_UP)) m_controllers[gamepad.GetPlayerIdx()].moveUp.Disable();
		}
	}
}

void dae::InputManager::UpdateKeyboardInput()
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

	size_t idx{};

	if (m_gamepads.empty()) idx = 0;
	else idx = m_gamepads.size();

	for (idx; idx < m_controllers.size(); ++idx)
	{
		if (currentKeyStates[SDL_SCANCODE_A]) m_controllers[idx].moveLeft.Execute();
		if (currentKeyStates[SDL_SCANCODE_D]) m_controllers[idx].moveRight.Execute();
		if (currentKeyStates[SDL_SCANCODE_S]) m_controllers[idx].moveDown.Execute();
		if (currentKeyStates[SDL_SCANCODE_W]) m_controllers[idx].moveUp.Execute();
	}
}

void dae::InputManager::ExecuteCommands()
{
	for (Controller& controller : m_controllers) controller.Execute();
}