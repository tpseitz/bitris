#include <thread>
#include <chrono>
#include "olcPixelGameEngine.h"
#include "tetris_gui.h"

using namespace std::chrono_literals;

const float FRAME_SPEED = 0.02;
const int WIDTH = 256, HEIGHT = 224;
const olc::Pixel COLOR[THEMES][2] = {
  { 0xfff85800, 0xfffcbc3c }, { 0xff00a800, 0xff18f8b8 },
  { 0xffcc00d8, 0xfff878f8 }, { 0xfff85800, 0xff54d858 },
  { 0xff5800e4, 0xff98f858 }, { 0xff98f858, 0xfffc8868 },
  { 0xff0038f8, 0xff7c7c7c }, { 0xfffc4468, 0xff2000a8 },
  { 0xfff85800, 0xff0038f8 }, { 0xff0038f8, 0xff44a0fc }};


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


bool TetrisGUI::_createGraphics() {
  for (int i = 0; i < THEMES; i++) {
    olc::Sprite* b1 = _createBlock(0xffffffff, COLOR[i][0]);
    olc::Sprite* b2 = _createBlock(COLOR[i][0], 0);
    olc::Sprite* b3 = _createBlock(COLOR[i][1], 0);
    block[i][0] = b1; block[i][1] = b2; block[i][2] = b3; block[i][3] = b1;
    block[i][4] = b2; block[i][5] = b3; block[i][6] = b1; }
  return true;
}


bool TetrisGUI::_initGame() {
  background = new olc::Sprite("background.png");
  well = new olc::Sprite(10 * block_w, 20 * block_h);
  next = new olc::Sprite(4 * block_w, 4 * block_h);

  state = STATE_MENU;

  return true;
}


void TetrisGUI::_updateRound() {
  if (move_left    > key_repeat_after) tetris->moveLeft();
  if (move_right   > key_repeat_after) tetris->moveRight();
  if (rotate_left  > key_repeat_after) tetris->rotateLeft();
  if (rotate_right > key_repeat_after) tetris->rotateRight();

  bool gameon = tetris->round();

  if (!gameon) state = STATE_GAMEOVER;
}


TetrisGUI::TetrisGUI() {
  sAppName = "Tetris";
}


bool TetrisGUI::OnUserCreate() {
  running = true;
  _createGraphics();
  _initGame();
  return running;
}


void TetrisGUI::_newGame(int level) {
  tetris = new Tetris(level);
  fWaitTime = 1.0;
  state = STATE_GET_READY;
}


void TetrisGUI::_destroyGame() {
  if (tetris) delete tetris;
  tetris = NULL;
  state = STATE_MENU;
}


bool TetrisGUI::OnUserUpdate(float fElapsedTime) {
  std::chrono::milliseconds wait { int((FRAME_SPEED - fElapsedTime) * 1000.0) };
  if (FRAME_SPEED > fElapsedTime) std::this_thread::sleep_for(wait);

  switch (state) {
    case STATE_ERROR:
      if (GetKey(olc::Key::ESCAPE).bPressed) running = false;
      break;
    case STATE_MENU:
      _readMenuEvents();
      _drawMenu();
      break;
    case STATE_OPTIONS:
      break;
    case STATE_GET_READY:
      fWaitTime -= fElapsedTime;
      if (fWaitTime <= 0) state = STATE_GAMEON;
      _drawTetris();
      break;
    case STATE_PAUSE:
      _readPauseEvents();
      _drawTetris();
      break;
    case STATE_GAMEON:
      _readTetrisEvents();
      _updateRound();
      _drawTetris();
      break;
    case STATE_GAMEOVER:
      fWaitTime -= fElapsedTime;
      if (fWaitTime <= 0 and (
          GetKey(olc::Key::ESCAPE).bPressed
          or GetKey(olc::Key::ENTER).bPressed)) {
        state = STATE_MENU;
      }
      _drawTetris();
      break;
    default:
      state = STATE_ERROR;
  }

  return running;
}


bool TetrisGUI::OnUserDestroy() {
  if (background) delete background;
  if (tetris) delete tetris;
  if (well) delete well;
  return true;
}


int main() {
  TetrisGUI game;
  if (game.Construct(256, 224, 2, 2)) game.Start();
  return 0;
}

