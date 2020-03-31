#pragma once
#include <random>
#include <unordered_set>

// Hash function for sorting Block type
template <>
struct hash<Block>
{
    size_t operator()(Block const& x) const noexcept
    {
        return ( (51 + std::hash<int>()(x.GetX())) * 51 + std::hash<int>()(x.GetY()) );
    }
};

class GameData
{
private:
  int mBlocksHeight, mBlocksWidth;
  bool mAlive{ true };
  int score = 0;

  void Init()
  {
    auto [r, g, b] = make_tuple(0.5f, 0.5f, 0.5f);

    // #STL homework
    for (int i : range::numeric(0, mBlocksHeight - 1))
      mWallBlocks.emplace_back(0, i * Block::GetSize(), r, g, b);
    for (int i : range::numeric(0, mBlocksWidth))
      mWallBlocks.emplace_back(i * Block::GetSize(), mBlocksHeight * Block::GetSize(), r, g, b);
    for (int i : range::numeric(0, mBlocksHeight - 1))
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
    // #STL homework rand() is a bad way to randomize, find a better modern C++ one
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(0, 10000);
    auto [r, g, b] = make_tuple(dist(mt) % 100 / 100.0f,
                                dist(mt) % 100 / 100.0f,
                                dist(mt) % 100 / 100.0f);

    const int startX = (int)(mBlocksWidth / 2) * Block::GetSize();

    mMovingBlocks.clear();

    switch (aShape)
    {
    case Shape::Line:
    {
      for (int i : range::numeric(-2, 1))
        mMovingBlocks.emplace_back(startX + i * Block::GetSize(), 0, r, g, b);
      break;
    }
    case Shape::Square:
    {
      mMovingBlocks.emplace_back(startX - 1 * Block::GetSize(), 0, r, g, b);
      mMovingBlocks.emplace_back(startX - 1 * Block::GetSize(), 1 * Block::GetSize(), r, g, b);
      mMovingBlocks.emplace_back(startX, 0, r, g, b);
      mMovingBlocks.emplace_back(startX, 1 * Block::GetSize(), r, g, b);
      break;
    }
    case Shape::L:
    {
      mMovingBlocks.emplace_back(startX, 0, r, g, b);
      mMovingBlocks.emplace_back(startX, 1 * Block::GetSize(), r, g, b);
      mMovingBlocks.emplace_back(startX + Block::GetSize(), 1 * Block::GetSize(), r, g, b);
      mMovingBlocks.emplace_back(startX, -Block::GetSize(), r, g, b);
      break;
    }
    case Shape::J:
    {
      mMovingBlocks.emplace_back(startX, 0, r, g, b);
      mMovingBlocks.emplace_back(startX, 1 * Block::GetSize(), r, g, b);
      mMovingBlocks.emplace_back(startX , - Block::GetSize(), r, g, b);
      mMovingBlocks.emplace_back(startX -Block::GetSize(), -Block::GetSize(), r, g, b);
      break;
    }
    case Shape::Z:
    {
      // #STL homework replace with emplace_back
      mMovingBlocks.emplace_back(Block(startX, 0, r, g, b));
      mMovingBlocks.emplace_back(Block(startX, -Block::GetSize(), r, g, b));
      mMovingBlocks.emplace_back(Block(startX + Block::GetSize(), 0, r, g, b));
      mMovingBlocks.emplace_back(Block(startX + Block::GetSize(), Block::GetSize(), r, g, b));

      break;
    }
    }
  }

  bool HasObstacleAt(int x, int y)
  {
    // #STL we could use unordered_set
    // homework research & try how to write hash function for an unordered_set
    
    // Insert all the blocks into the unordered set
    unordered_set<Block> walls(begin(mFrozenBlocks), end(mFrozenBlocks));
    walls.insert(begin(mWallBlocks), end(mWallBlocks));
    
    Block *t = new Block(x, y);

    if (walls.find(*t) != end(walls))
        return true;
    return false;
    
    /* Original function
    for (auto val : walls) {
        cout << val.GetX() << " " << val.GetY()<<endl;
    }
    auto foundInWall = find_if(begin(mWallBlocks), end(mWallBlocks), [x, y](Block & block)
      {
        return block.GetX() == x && block.GetY() == y;
      });
    if (foundInWall != end(mWallBlocks))
      return true;
    

    for (auto& block : mFrozenBlocks)
    {
      if (block.GetX() == x && block.GetY() == y)
        return true;
    }
    return false;
    */
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
      // #STL homework rotate shape clockwise
        bool CanRotate = true;
        int CenterX = 0,CenterY = 0;

        // Compute the origin point for Shape rotation
        for (auto& block : mMovingBlocks) {
            CenterX += block.GetX();
            CenterY += block.GetY();
        }
        CenterX /= (mMovingBlocks.size() * Block::GetSize()) + 1 ;
        CenterY /= (mMovingBlocks.size() * Block::GetSize());

        // Check if rotation can be acomplished
        for (auto& block : mMovingBlocks) {
            if (HasObstacleAt(-block.GetY() + (CenterX + 1) * Block::GetSize() + CenterY * Block::GetSize(),
                block.GetX() - CenterX * Block::GetSize() + CenterY * Block::GetSize()) == true)
                CanRotate = false;
        }
        // Rotate shape if possible
        if (CanRotate) {
            for (auto& block : mMovingBlocks) {
                block.SetPos(-block.GetY() + (CenterX + 1) * Block::GetSize() + CenterY * Block::GetSize(),
                              block.GetX() - CenterX * Block::GetSize() + CenterY * Block::GetSize());
            }
        }
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
    //int minY = 999;

    // proceed to freeze the moving blocks
    // moving => frozen

    auto minBlock = *min_element(begin(mMovingBlocks), end(mMovingBlocks),
      [](Block& lhs, Block& rhs) { return lhs.GetY() < rhs.GetY(); });

    mFrozenBlocks.insert(end(mFrozenBlocks), begin(mMovingBlocks), end(mMovingBlocks));

    mMovingBlocks.clear();

    if (minBlock.GetY() <= 2)
      mAlive = false;
  }

  void Update()
  {
      if (!IsAlive())
          return;

      // #STL homework we need to destroy blocks that line up on a X coordinate
      vector<int> BlockAmount(mBlocksHeight + 1);
    
    // Compute the amount of blocks per row
    for (auto block : mFrozenBlocks) {
        BlockAmount[block.GetY() / Block::GetSize()]++;
    }
    
    // Check if we have a line
    for (int row = 0; row <= mBlocksHeight * Block::GetSize(); row += Block::GetSize()) {
        if (BlockAmount[row / Block::GetSize()] == mBlocksWidth - 1) {
            // Remove the line
            mFrozenBlocks.erase(remove_if(begin(mFrozenBlocks), end(mFrozenBlocks),
                                [row](Block block) {return row == block.GetY(); }),
                                end(mFrozenBlocks));
    
            // Shift down the blocks above it
            for (auto & block : mFrozenBlocks) {
                if(block.GetY() <= row)
                block.UpdatePos(0, 1);
            }
            // Get 10 points for clearing a line
            score += 10;
        }
        BlockAmount[row / Block::GetSize()] = 0;
    }

    if (!TryMove(0, 1))
      FreezeMovingBlocks();

    if (mMovingBlocks.empty()) {
        Add((Shape)(rand() % 5 + 1));
        // Get a point for every block
        ++score;
    }
  }

  void PrintGameOver()
  {
    S2D_Text* txt = S2D_CreateText("fonts\\HARNGTON.ttf", "GAME OVER", 60);
    txt->x = 127;
    txt->y = 340;
    S2D_DrawText(txt);
  }

  // Function to print score
  void PrintScore()
  {
      S2D_Text* txt = S2D_CreateText("fonts\\HARNGTON.ttf", "", 60);
      S2D_SetText(txt, "Score: %d", score);
      txt->x = 227;
      txt->y = 440;
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
      PrintScore();
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
