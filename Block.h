#pragma once

class Block
{
private:
  S2D_Image* mImg;
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

  int GetX() const { return mX;  }

  int GetY() const { return mY;  }
};
