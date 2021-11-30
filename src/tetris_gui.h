#ifndef TETRIS_GUI_H
#define TETRIS_GUI_H

#include <cstdint>
#include "olcPixelGameEngine.h"
#include "tetris.h"


#define THEMES 10


class TetrisGUI : public olc::PixelGameEngine {
  float fTime = 0, fWaitTime = 0;
  olc::Sprite *block[THEMES][7];
  int block_w = 8, block_h = 8;

  int key_repeat_after = 30;

  Tetris *tetris = NULL;
  bool running = true, gameon = false, pause = false;
  olc::Sprite* background = NULL;
  olc::Sprite* well = NULL;
  olc::Sprite* next = NULL;

  int move_left = 0, move_right = 0, rotate_left = 0, rotate_right = 0;

  bool _createGraphics();
  bool _initGame();
  void _readPauseEvents();
  void _readTetrisEvents();
  void _updateRound();
  void _drawWell();
  void _drawNext();
  void _drawTetris();

public:
  TetrisGUI();
  bool OnUserCreate() override;
  bool OnUserUpdate(float) override;
  bool OnUserDestroy() override;
};


#endif
