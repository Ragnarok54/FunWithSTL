#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <Windows.h>

#include <simple2d.h>


#define TIMER_GUARD(a, t)                    \
static DWORD lastTick##a = ::GetTickCount(); \
if (GetTickCount() - lastTick##a < t)        \
  return;                                    \
lastTick##a = ::GetTickCount();


using namespace std;

template<typename T>
vector<T> rangeint(T from, T to)
{
  vector<T> ret(to - from + 1);
  iota(begin(ret), end(ret), from);
  return ret;
}

class Block
{
private:
  S2D_Image * mImg;
  int mX = 0, mY = 0;
  bool mIsStatic = false;

public:

  Block(int aX, int aY, bool aIsStatic = true)
    : mIsStatic(aIsStatic), mX(aX), mY(aY)
  {
    string imagePath = string("img\\square0.bmp");
    mImg = S2D_CreateImage(imagePath.c_str());
  }

  void SetColor(int r, int g, int b)
  {
    mImg->color.r = r;
    mImg->color.g = g;
    mImg->color.b = b;
    mImg->color.a = 1.0;
  }

  static int GetSize() { return 51; }

  void Render()
  {
    mImg->x = mX; 
    mImg->y = mY;
    S2D_DrawImage(mImg);
  }

  bool IsStatic() const
  {
    return mIsStatic;
  }

  void MakeStatic()
  {
    mIsStatic = true;
  }

  void UpdatePos(float aDXunits, float aDYunits)
  {
    mX += GetSize() * aDXunits;
    mY += GetSize() * aDYunits;
  }
};

class Game
{
private:
  int mBlocksHeight, mBlocksWidth;

  void Init()
  {
    for (int i : rangeint(0, mBlocksHeight - 1))
      mBlocks.emplace_back(0, i * Block::GetSize());
    for (int i : rangeint(0, mBlocksWidth))
      mBlocks.emplace_back(i * Block::GetSize(), mBlocksHeight * Block::GetSize());
    for (int i : rangeint(0, mBlocksHeight - 1))
      mBlocks.emplace_back(mBlocksWidth * Block::GetSize(), i * Block::GetSize());
  }

  int GetLimitY() const 
  {
    return Block::GetSize() * mBlocksHeight;
  }

public :
  vector<Block> mBlocks;

  enum class Key
  {
    Up = 0,
    Right,
    Down,
    Left,
    Space,
    Esc,
    None
  };

  enum class Shape
  {
    Line = 0,
    Square,
    Z,
    L
  };

  Game(int aBlocksHeight, int aBlocksWidth)
    : mBlocksHeight(aBlocksHeight), mBlocksWidth(aBlocksWidth)
  {
    Init();
  }

  void Add(Shape aShape)
  {
    auto [r, g, b] = make_tuple(rand() % 100 / 100, 
                                rand() % 100 / 100, 
                                rand() % 100 / 100);


  }

  void DoKeyPressed(Key aKey)
  {
    int dx = 0, dy = 0;

    if (aKey == Key::Space)
    {
      mBlocks.emplace_back(mBlocksWidth / 2 * Block::GetSize(), 0, false);
    }
    else if (aKey == Key::Left)
    {
      dx = -1;
    }
    else if (aKey == Key::Right)
    {
      dx = 0.5;
    }

    for (auto & block : mBlocks)
    {
      if (block.IsStatic())
        continue;

      block.UpdatePos(dx, dy);
    }
  }

  void Update()
  {
    for (auto& block : mBlocks)
    {
      if (block.IsStatic())
        continue;

      block.UpdatePos(0, 0.1);
    }
  }
};

Game world{ 20, 19 };
DWORD lastTickCount = 0;

void update() 
{
  TIMER_GUARD(update, 1);
  world.Update();
}

void on_key(S2D_Event e) 
{
  Game::Key pressedKey = Game::Key::None;
  string pressedKeyStr = e.key;
  if (pressedKeyStr == "Left")
    pressedKey = Game::Key::Left;
  else if (pressedKeyStr == "Right")
    pressedKey = Game::Key::Right;
  else if (pressedKeyStr == "Down")
    pressedKey = Game::Key::Down;
  else if (pressedKeyStr == "Up")
    pressedKey = Game::Key::Up;
  else if (pressedKeyStr == "Space")
    pressedKey = Game::Key::Space;
  else if (pressedKeyStr == "Escape")
    exit(0);
  else
    return; // some other key, not interesting for us

  switch (e.type) 
  {
    case S2D_KEY_DOWN:
    {
      TIMER_GUARD(keyDown, 250);
      // Key was pressed
      world.DoKeyPressed(pressedKey);
      break;
    }

    case S2D_KEY_HELD:
    {
      TIMER_GUARD(keyHeld, 250);
      world.DoKeyPressed(pressedKey);
      // Key is being held down
      break;
    }

    case S2D_KEY_UP:
    {
      // Key was released
      break;
    }
  }
}

void render() 
{
  for (auto& block : world.mBlocks)
    block.Render();
}

int main(int argc, char* args[]) 
{
  S2D_Window* window = S2D_CreateWindow(
    "FUN WITH STL", 1030, 1080, update, render, S2D_RESIZABLE
  );
  window->on_key = on_key;

  S2D_Music * mus = S2D_CreateMusic(R"(audio\background.ogg)");
  S2D_PlayMusic(mus, true);  // play on a loop
  S2D_SetMusicVolume(1);  // set volume 50%


  S2D_HideCursor();
  S2D_Show(window);

  return 0;
}