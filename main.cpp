#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <simple2d.h>

using namespace std;

class Block
{
private:
  S2D_Image * mImg;
  int mX = 0, mY = 0, mType = 0;

public:

  Block(int type)
  {
    string imagePath = string("img\\square") + (char)('0' + type) + ".bmp";
    S2D_Image* img = S2D_CreateImage(imagePath.c_str());
  }

  void Render()
  {
    mImg->x = mX; 
    mImg->y = mY;
    S2D_DrawImage(mImg);
  }
};

class Game
{
public :
  
};

void update() { /* update your application state */ }

void render() {

  for (int i = 0; i < 15; ++i)
  {
    S2D_Image* img = S2D_CreateImage(R"(img\square0.bmp)");
    img->y = i * 51;
    S2D_DrawImage(img);
  }
  /*S2D_DrawTriangle(
    320, 50, 1, 0, 0, 1,
    540, 430, 0, 1, 0, 1,
    100, 430, 0, 0, 1, 1
  ); */
}

int main(int argc, char* args[]) {

  S2D_Window* window = S2D_CreateWindow(
    "FUN WITH STL", 1024, 768, update, render, S2D_RESIZABLE
  );

  S2D_Show(window);

  return 0;
}