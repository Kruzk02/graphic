#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

enum class KeyState : std::uint8_t
{
    None,
    Pressed,
    Held,
    Released
};

struct KeyEvent
{
    int key;
    int action;
};

class Input
{
  public:
    static void init();
    static void update();

    static void setKey(const KeyEvent &keyEvent);

    static auto isKeyPressed(int key) -> bool;
    static auto isKeyHeld(int key) -> bool;
    static auto isKeyReleased(int key) -> bool;

    static auto isActionPressed(const std::string &action) -> bool;
    static auto isActionHeld(const std::string &action) -> bool;

  private:
    static std::unordered_map<int, KeyState> keyStates;
    static std::unordered_map<std::string, int> actionMap;
};
