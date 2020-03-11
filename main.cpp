#include "pch.h"
#include "Block.h"
#include "GameData.h"


GameData gameData{ 20, 19 };


void update() 
{
  TIMER_GUARD(update, 250);
  gameData.Update();
}

void on_key(S2D_Event e) 
{
  GameData::Key pressedKey = GameData::KeyFromString(e.key);

  if (pressedKey == GameData::Key::Esc)
    exit(0);

  switch (e.type) 
  {
  case S2D_KEY_HELD:
    case S2D_KEY_DOWN:
    {
      TIMER_GUARD(keyDown, 100);
      gameData.DoKeyPressed(pressedKey);
      break;
    }
  }
}

void render() 
{
  gameData.Render();
}

int main(int argc, char* args[]) 
{
  S2D_Window* window = S2D_CreateWindow(
    "FUN WITH STL", 1030, 1080, update, render, S2D_RESIZABLE
  );
  window->on_key = on_key;

  S2D_Music * mus = S2D_CreateMusic(R"(audio\background.ogg)");
  S2D_PlayMusic(mus, true);
  S2D_SetMusicVolume(1);

  S2D_HideCursor();
  S2D_Show(window);

  return 0;
}