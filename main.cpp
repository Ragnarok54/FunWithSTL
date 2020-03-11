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

public:
  
  void SetColor(float r, float g, float b)
  {
    mImg->color.r = r;
    mImg->color.g = g;
    mImg->color.b = b;
    mImg->color.a = 1.0;
  }

  Block(int aX, int aY, float aR = 0, float aG = 0, float aB = 0)
    : mX(aX), mY(aY)
  {
    string imagePath = string("img\\square0.bmp");
    mImg = S2D_CreateImage(imagePath.c_str());

    SetColor(aR, aG, aB);
  }

  static int GetSize() { return 51; }

  void Render()
  {
    mImg->x = mX; 
    mImg->y = mY;
    S2D_DrawImage(mImg);
  }

  void UpdatePos(float aDXunits, float aDYunits)
  {
    mX += (int)(GetSize() * aDXunits);
    mY += (int)(GetSize() * aDYunits);
  }
};

class Game
{
private:
  int mBlocksHeight, mBlocksWidth;

  void Init()
  {
    auto [r, g, b] = make_tuple(0.5f, 0.5f, 0.5f);

    for (int i : rangeint(0, mBlocksHeight - 1))
      mBlocks.emplace_back(0, i * Block::GetSize(), r, g, b);
    for (int i : rangeint(0, mBlocksWidth))
      mBlocks.emplace_back(i * Block::GetSize(), mBlocksHeight * Block::GetSize(), r, g, b);
    for (int i : rangeint(0, mBlocksHeight - 1))
      mBlocks.emplace_back(mBlocksWidth * Block::GetSize(), i * Block::GetSize(), r, g, b);
  }

  int GetLimitY() const 
  {
    return Block::GetSize() * mBlocksHeight;
  }

public:

  enum class Key
  {
    None = 0,
    Up,
    Right,
    Down,
    Left,
    Space,
    Esc
  };

  enum class Shape
  {
    None = 0,
    Square,
    Line,
    L,
    J,
    Z,
  };

  vector<Block> mBlocks;
  vector<Block> mMovingShape;
  Shape         mMovingShapeType{ Shape::None };

  Game(int aBlocksHeight, int aBlocksWidth)
    : mBlocksHeight(aBlocksHeight), mBlocksWidth(aBlocksWidth)
  {
    Init();
  }

  void Add(Shape aShape)
  {
    auto [r, g, b] = make_tuple(rand() % 100 / 100.0f, 
                                rand() % 100 / 100.0f, 
                                rand() % 100 / 100.0f);

    const int startX = (int)(mBlocksWidth / 2) * Block::GetSize();

    mMovingShape.clear();

    switch (aShape)
    {
    case Shape::Line:
      {
        mMovingShape.emplace_back(startX - 2 * Block::GetSize(), 0, r, g, b);
        mMovingShape.emplace_back(startX - 1 * Block::GetSize(), 0, r, g, b);
        mMovingShape.emplace_back(startX, 0, r, g, b);
        mMovingShape.emplace_back(startX + 1 * Block::GetSize(), 0, r, g, b);
        break;
      }
      case Shape::Square:
      {
        mMovingShape.emplace_back(startX - 1 * Block::GetSize(), 0, r, g, b);
        mMovingShape.emplace_back(startX - 1 * Block::GetSize(), 1 * Block::GetSize(), r, g, b);
        mMovingShape.emplace_back(startX, 0, r, g, b);
        mMovingShape.emplace_back(startX, 1 * Block::GetSize(), r, g, b);
        break;
      }
      case Shape::L:
      {
        mMovingShape.emplace_back(startX, 0, r, g, b);
        mMovingShape.emplace_back(startX, 1 * Block::GetSize(), r, g, b);
        mMovingShape.emplace_back(startX + Block::GetSize(), 1 * Block::GetSize(), r, g, b);
        mMovingShape.emplace_back(startX, -Block::GetSize(), r, g, b);
        break;
      }
      case Shape::J:
      {
        mMovingShape.emplace_back(startX, 0, r, g, b);
        mMovingShape.emplace_back(startX, 1 * Block::GetSize(), r, g, b);
        mMovingShape.emplace_back(startX + Block::GetSize(), -1 * Block::GetSize(),  r, g, b);
        mMovingShape.emplace_back(startX, -Block::GetSize(), r, g, b);
        break;
      }
      case Shape::Z:
      {
        mMovingShape.emplace_back(startX, 0, r, g, b);
        mMovingShape.emplace_back(startX, -Block::GetSize(), r, g, b);
        mMovingShape.emplace_back(startX + Block::GetSize(), 0, r, g, b);
        mMovingShape.emplace_back(startX + Block::GetSize(), Block::GetSize(), r, g, b);
        
        break;
      }
    }
  }

  void DoKeyPressed(Key aKey)
  {
    int dx = 0, dy = 0;

    if (aKey == Key::Space)
    {
      //mBlocks.emplace_back(mBlocksWidth / 2 * Block::GetSize(), 0, false);
      Add((Shape)(rand() % 4 + 1));
    }
    else if (aKey == Key::Left)
    {
      dx = -1;
    }
    else if (aKey == Key::Right)
    {
      dx = 1;
    }

    for (auto & block : mMovingShape)
    {
      block.UpdatePos(dx, dy);
    }
  }

  void Update()
  {
    for (auto& block : mMovingShape)
    {
      block.UpdatePos(0, 0.1);
    }
  }

  void Render()
  {
    for (auto& block : mBlocks)
      block.Render();
    for (auto& block : mMovingShape)
      block.Render();
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
  world.Render();
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