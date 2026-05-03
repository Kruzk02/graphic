#include "input.hpp"
#include "GLFW/glfw3.h"
#include <string>
#include <unordered_map>

std::unordered_map<int, KeyState> Input::keyStates;
std::unordered_map<std::string, int> Input::actionMap;

void Input::init()
{
    actionMap["Quit"] = GLFW_KEY_ESCAPE;
    actionMap["MoveForward"] = GLFW_KEY_W;
    actionMap["MoveLeft"] = GLFW_KEY_A;
    actionMap["MoveRight"] = GLFW_KEY_D;
    actionMap["MoveBackward"] = GLFW_KEY_S;
}

void Input::update()
{
    for (auto &[key, state] : keyStates)
    {
        if (state == KeyState::Pressed)
            state = KeyState::Held;
        else if (state == KeyState::Released)
            state = KeyState::None;
    }
}

void Input::setKey(const KeyEvent &keyEvent)
{
    if (keyEvent.action == GLFW_PRESS)
    {
        keyStates[keyEvent.key] = KeyState::Pressed;
    }
    else if (keyEvent.action == GLFW_RELEASE)
    {
        keyStates[keyEvent.key] = KeyState::Released;
    }
}

auto Input::isKeyPressed(int key) -> bool
{
    return keyStates[key] == KeyState::Pressed;
}

auto Input::isKeyHeld(int key) -> bool
{
    return keyStates[key] == KeyState::Held;
}

auto Input::isKeyReleased(int key) -> bool
{
    return keyStates[key] == KeyState::Released;
}

auto Input::isActionPressed(const std::string &action) -> bool
{
    return isKeyPressed(actionMap[action]);
}

auto Input::isActionHeld(const std::string &action) -> bool
{
    return isKeyHeld(actionMap[action]);
}
