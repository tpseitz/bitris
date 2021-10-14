#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#include "tetris.cpp"

const int THEMES = 10;
const float FRAME_SPEED = 0.02;
const olc::Pixel COLOR[THEMES][2] = {
  { 0xfff85800, 0xfffcbc3c }, { 0xff00a800, 0xff18f8b8 },
  { 0xffcc00d8, 0xfff878f8 }, { 0xfff85800, 0xff54d858 },
  { 0xff5800e4, 0xff98f858 }, { 0xff98f858, 0xfffc8868 },
  { 0xff0038f8, 0xff7c7c7c }, { 0xfffc4468, 0xff2000a8 },
  { 0xfff85800, 0xff0038f8 }, { 0xff0038f8, 0xff44a0fc }};

olc::Key
  MOVE_LEFT   = olc::Key::A,  MOVE_RIGHT   = olc::Key::D,
  ROTATE_LEFT = olc::Key::O,  ROTATE_RIGHT = olc::Key::P,
  DROP_SOFT   = olc::Key::S,  DROP_HARD    = olc::Key::ENTER,
  PAUSE       = olc::Key::ESCAPE;

int key_repeat_after = 10;

olc::Sprite* _createBlock(olc::Pixel center, olc::Pixel border) {
  olc::Sprite* img = new olc::Sprite(8, 8);
  for (int j = 0; j < 64; j++) img->SetPixel(j % 8, j / 8, center);
  if (border.n) for (int j = 0; j < 8; j++) {
    img->SetPixel(0, j, border);
    img->SetPixel(j, 0, border);
    img->SetPixel(8 - 2, j, border);
    img->SetPixel(j, 8 - 2, border); }
  for (int j = 0; j < 8; j++) {
    img->SetPixel(8 - 1, j, 0xff000000);
    img->SetPixel(j, 8 - 1, 0xff000000); }
  img->SetPixel(0, 0, 0xffffffff);
  img->SetPixel(1, 1, 0xffffffff);
  img->SetPixel(2, 1, 0xffffffff);
  img->SetPixel(1, 2, 0xffffffff);
  return img;
}

class Example : public olc::PixelGameEngine {
  float fTime = 0;
  olc::Sprite *block[THEMES][7];
  int block_w = 8, block_h = 8;

  Tetris *tetris = NULL;
  bool gameon = false, pause = false;
  olc::Sprite* background = NULL;
  olc::Sprite* well = NULL;
  olc::Sprite* next = NULL;

  bool _createGraphics() {
    for (int i = 0; i < THEMES; i++) {
      olc::Sprite* b1 = _createBlock(0xffffffff, COLOR[i][0]);
      olc::Sprite* b2 = _createBlock(COLOR[i][0], 0);
      olc::Sprite* b3 = _createBlock(COLOR[i][1], 0);
      block[i][0] = b1; block[i][1] = b2; block[i][2] = b3; block[i][3] = b1;
      block[i][4] = b2; block[i][5] = b3; block[i][6] = b1; }
    return true;
  }

  bool _initGame() {
    background = new olc::Sprite("background.png");
    well = new olc::Sprite(10 * block_w, 20 * block_h);
    next = new olc::Sprite(4 * block_w, 4 * block_h);

    return true;
  }

  int move_left = 0, move_right = 0, rotate_left = 0, rotate_right = 0;
  void _readTetrisKeys() {
    if      (GetKey(DROP_SOFT).bPressed)  tetris->dropSoft = true;
    else if (GetKey(DROP_SOFT).bReleased) tetris->dropSoft = false;

    if (GetKey(DROP_HARD).bPressed) tetris->dropHard();

    if (GetKey(MOVE_LEFT).bPressed) tetris->moveLeft();
    if (GetKey(MOVE_LEFT).bHeld) move_left++;
    else move_left = 0;

    if (GetKey(MOVE_RIGHT).bPressed) tetris->moveRight();
    if (GetKey(MOVE_RIGHT).bHeld) move_right++;
    else move_right = 0;

    if (GetKey(ROTATE_LEFT).bPressed) tetris->rotateLeft();
    if (GetKey(ROTATE_LEFT).bHeld) rotate_left++;
    else rotate_left = 0;

    if (GetKey(ROTATE_RIGHT).bPressed) tetris->rotateRight();
    if (GetKey(ROTATE_RIGHT).bHeld) rotate_right++;
    else rotate_right = 0;
  }

  void _updateRound() {
    if (move_left    > key_repeat_after) tetris->moveLeft();
    if (move_right   > key_repeat_after) tetris->moveRight();
    if (rotate_left  > key_repeat_after) tetris->rotateLeft();
    if (rotate_right > key_repeat_after) tetris->rotateRight();

    gameon = tetris->round();
  }

  void _drawWell() {
    for (int y = 0; y < 20; y++) {
      uint16_t ln = tetris->well[y + 1] >> 3;
      for (int x = 0; x < 10; x++)
        if (ln >> x & 1)
          DrawSprite(x * block_w, y * block_h, block[tetris->lvl % THEMES][0]);
        else
          FillRect(x * block_w, y * block_h, block_w, block_h, 0xff000000);
    }

    for (int y = 0; y < 4; y++) {
      uint16_t ln = tetris->getLine(tetris->tet, y, 0);
      for (int x = 0; x < 4; x++)
        if (ln >> x & 1) DrawSprite(
          (tetris->tx + x - 3) * block_w,
          (tetris->ty + y - 1) * block_h,
          block[tetris->lvl % THEMES][tetris->ti]);
    }
  }

  void _drawNext() {
    FillRect(0, 0, 4 * block_w, 4 * block_h, 0xff000000);
    uint16_t ln;
    for (int y = 0; y < 4; y++) {
      ln = tetris->getLine(tls[tetris->nxt[0]], y, 0);
      for (int x = 0; x < 4; x++)
        if (ln >> x & 1) DrawSprite(x * block_w, y * block_h,
          block[tetris->lvl % THEMES][tetris->nxt[0]]);
    }
  }

  void _drawTetris() {
    if (background) DrawSprite(0, 0, background);
    DrawRect(94, 38, 10 * block_w + 4, 20 * block_h + 4);
    DrawRect(206, 114, 4 * block_w + 4, 4 * block_h + 4);

    olc::Sprite* prev = GetDrawTarget();
    SetDrawTarget(well);
    _drawWell();
    SetDrawTarget(next);
    _drawNext();
    SetDrawTarget(prev);

    char buf[16];
    DrawSprite(96, 40, well);
    DrawSprite(192, 104, next);
    sprintf(buf, "%06d", tetris->pts);
    FillRect(192, 56, 48, 8, olc::BLACK);
    DrawString(192, 56, buf, olc::WHITE);
    sprintf(buf, "%02d", tetris->lvl);
    FillRect(208, 160, 16, 8, olc::BLACK);
    DrawString(208, 160,buf , olc::WHITE);
    sprintf(buf, "%03d", tetris->dl);
    FillRect(152, 16, 24, 8, olc::BLACK);
    DrawString(152, 16, buf, olc::WHITE);
  }

public:
  Example() { sAppName = "Tetris"; }

  bool OnUserCreate() override {
    _createGraphics();
    _initGame();
    return true;
  }

  bool OnUserUpdate(float fElapsedTime) override {
    if (pause && GetKey(PAUSE).bReleased) pause = false;
    else if (tetris && gameon) {
      if (GetKey(PAUSE).bPressed) pause = true;
      _readTetrisKeys();
      _updateRound();
      _drawTetris();
    } else if (tetris) {
      if (GetKey(olc::Key::ENTER).bPressed) {
        delete tetris;
        tetris = NULL;
        return false; //XXX
      } else _drawTetris();
    } else {
      tetris = new Tetris(9);
      gameon = true;
    }

    return true;
  }

  bool OnUserDestroy() override {
    if (background) delete background;
    if (tetris) delete tetris;
    if (well) delete well;
    return true;
  }
};

int main() {
  Example game;
  if (game.Construct(256, 224, 2, 2)) game.Start();
  return 0;
}

