#pragma once

class GameData
{
private:
  int mBlocksHeight, mBlocksWidth;
  bool mAlive{ true };

  void Init()
  {
    auto [r, g, b] = make_tuple(0.5f, 0.5f, 0.5f);

    // #STL homework
    for (int i : rangeint(0, mBlocksHeight - 1))
      mWallBlocks.emplace_back(0, i * Block::GetSize(), r, g, b);
    for (int i : rangeint(0, mBlocksWidth))
      mWallBlocks.emplace_back(i * Block::GetSize(), mBlocksHeight * Block::GetSize(), r, g, b);
    for (int i : rangeint(0, mBlocksHeight - 1))
      mWallBlocks.emplace_back(mBlocksWidth * Block::GetSize(), i * Block::GetSize(), r, g, b);
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

  vector<Block> mWallBlocks;

  vector<Block> mFrozenBlocks;

  vector<Block> mMovingBlocks;

  GameData(int aBlocksHeight, int aBlocksWidth)
    : mBlocksHeight(aBlocksHeight), mBlocksWidth(aBlocksWidth)
  {
    Init();
  }

  bool IsAlive() const
  {
    return mAlive;
  }

  void Add(Shape aShape)
  {
    // #STL homework rand() is a bad way to randomize, find a better one
    auto [r, g, b] = make_tuple(rand() % 100 / 100.0f,
      rand() % 100 / 100.0f,
      rand() % 100 / 100.0f);

    const int startX = (int)(mBlocksWidth / 2) * Block::GetSize();

    mMovingBlocks.clear();

    switch (aShape)
    {
    case Shape::Line:
    {
      mMovingBlocks.push_back(Block(startX - 2 * Block::GetSize(), 0, r, g, b));
      mMovingBlocks.push_back(Block(startX - 1 * Block::GetSize(), 0, r, g, b));
      mMovingBlocks.push_back(Block(startX, 0, r, g, b));
      mMovingBlocks.push_back(Block(startX + 1 * Block::GetSize(), 0, r, g, b));
      break;
    }
    case Shape::Square:
    {
      mMovingBlocks.push_back(Block(startX - 1 * Block::GetSize(), 0, r, g, b));
      mMovingBlocks.push_back(Block(startX - 1 * Block::GetSize(), 1 * Block::GetSize(), r, g, b));
      mMovingBlocks.push_back(Block(startX, 0, r, g, b));
      mMovingBlocks.push_back(Block(startX, 1 * Block::GetSize(), r, g, b));
      break;
    }
    case Shape::L:
    {
      mMovingBlocks.push_back(Block(startX, 0, r, g, b));
      mMovingBlocks.push_back(Block(startX, 1 * Block::GetSize(), r, g, b));
      mMovingBlocks.push_back(Block(startX + Block::GetSize(), 1 * Block::GetSize(), r, g, b));
      mMovingBlocks.push_back(Block(startX, -Block::GetSize(), r, g, b));
      break;
    }
    case Shape::J:
    {
      mMovingBlocks.push_back(Block(startX, 0, r, g, b));
      mMovingBlocks.push_back(Block(startX, 1 * Block::GetSize(), r, g, b));
      mMovingBlocks.push_back(Block(startX + Block::GetSize(), -1 * Block::GetSize(), r, g, b));
      mMovingBlocks.push_back(Block(startX, -Block::GetSize(), r, g, b));
      break;
    }
    case Shape::Z:
    {
      mMovingBlocks.push_back(Block(startX, 0, r, g, b));
      mMovingBlocks.push_back(Block(startX, -Block::GetSize(), r, g, b));
      mMovingBlocks.push_back(Block(startX + Block::GetSize(), 0, r, g, b));
      mMovingBlocks.push_back(Block(startX + Block::GetSize(), Block::GetSize(), r, g, b));

      break;
    }
    }
  }

  bool HasObstacleAt(int x, int y)
  {
    // #STL
    for (auto& block : mWallBlocks)
    {
      if (block.GetX() == x && block.GetY() == y)
        return true;
    }
    for (auto& block : mFrozenBlocks)
    {
      if (block.GetX() == x && block.GetY() == y)
        return true;
    }

    return false;
  }

  bool TryMove(int dx, int dy)
  {
    auto blockClones = mMovingBlocks;
    for (auto& block : blockClones)
    {
      block.UpdatePos(dx, dy);
      if (HasObstacleAt(block.GetX(), block.GetY()))
        return false;
    }   
   
    mMovingBlocks = blockClones;
    return true;
  }

  void DoKeyPressed(Key aKey)
  {
    int dx = 0, dy = 0;

    if (aKey == Key::Space)
    {
      // #STL homework rotate shape
    }
    else if (aKey == Key::Left)
    {
      dx = -1;
    }
    else if (aKey == Key::Right)
    {
      dx = 1;
    }
    else if (aKey == Key::Down)
    {
      TryMove(0, 1);
    }

    TryMove(dx, dy);
  }

  void FreezeMovingBlocks()
  {
    // #STL 
    
    // min value on Y to see if game has ended
    int minY = 999;

    // proceed to freeze the moving blocks
    // moving => static
    for (auto& block : mMovingBlocks)
    {
      mFrozenBlocks.push_back(block);
      if (block.GetY() < minY)
        minY = block.GetY();
    }
    mMovingBlocks.clear();

    if (minY <= 2)
      mAlive = false;
  }

  void Update()
  {
    if (!IsAlive())
      return;

    // #STL we need to destroy blocks that line up on a X coordinate

    if (!TryMove(0, 1))
      FreezeMovingBlocks();

    if (mMovingBlocks.empty())
      Add((Shape)(rand() % 4 + 1));
  }

  void PrintGameOver()
  {
    S2D_Text* txt = S2D_CreateText("fonts\\HARNGTON.ttf", "GAME OVER", 60);
    txt->x = 127;
    txt->y = 340;
    S2D_DrawText(txt);
  }

  void Render()
  {
    if (IsAlive())
    {
      for (auto& block : mWallBlocks)
        block.Render();
      for (auto& block : mMovingBlocks)
        block.Render();
      for (auto& block : mFrozenBlocks)
        block.Render();
    }
    else
    {
      PrintGameOver();

      // homework can you devise a scoring system?
    }
  }

  static Key KeyFromString(string pressedKeyStr)
  {
    GameData::Key pressedKey = GameData::Key::None;
    if (pressedKeyStr == "Left")
      pressedKey = GameData::Key::Left;
    else if (pressedKeyStr == "Right")
      pressedKey = GameData::Key::Right;
    else if (pressedKeyStr == "Down")
      pressedKey = GameData::Key::Down;
    else if (pressedKeyStr == "Up")
      pressedKey = GameData::Key::Up;
    else if (pressedKeyStr == "Space")
      pressedKey = GameData::Key::Space;
    else if (pressedKeyStr == "Escape")
      pressedKey = GameData::Key::Esc;
    return pressedKey;
  }
};
#pragma once
