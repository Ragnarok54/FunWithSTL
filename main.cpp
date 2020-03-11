#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <simple2d.h>

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
  int mX = 0, mY = 0, mType = 0;

public:

  Block(int aType, int aX, int aY)
    : mType(aType), mX(aX), mY(aY)
  {
    string imagePath = string("img\\square") + (char)('0' + mType) + ".bmp";
    S2D_Image* img = S2D_CreateImage(imagePath.c_str());
  }

  static int GetSize() { return 51; }

  void Render()
  {
    mImg->x = mX; 
    mImg->y = mY;
    S2D_DrawImage(mImg);
  }
};

class Game
{
private:
  int mBlocksHeight, mBlocksWidth;

  void Init()
  {
    for (int i : rangeint(0, mBlocksHeight))
      mBlocks.emplace_back(0, 0, i * Block::GetSize());
  }

public :
  vector<Block> mBlocks;

  Game(int aBlocksHeight, int aBlocksWidth)
    : mBlocksHeight(aBlocksHeight), mBlocksWidth(aBlocksWidth)
  {
  }
};

Game world{ 15, 18 };

void update() 
{ 
  /* update your application state */ 
}

void render() 
{

  for (auto& block : world.mBlocks)
  {
    block.Render();
  } 
}

int main(int argc, char* args[]) 
{

  S2D_Window* window = S2D_CreateWindow(
    "FUN WITH STL", 1024, 768, update, render, S2D_RESIZABLE
  );

  S2D_Show(window);

  return 0;
}