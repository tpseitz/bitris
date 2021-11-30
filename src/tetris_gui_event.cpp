#include "tetris_gui.h"

olc::Key
  MOVE_LEFT   = olc::Key::A,  MOVE_RIGHT   = olc::Key::D,
  ROTATE_LEFT = olc::Key::O,  ROTATE_RIGHT = olc::Key::P,
  DROP_SOFT   = olc::Key::S,  DROP_HARD    = olc::Key::ENTER,
  PAUSE       = olc::Key::ESCAPE;


void TetrisGUI::_readPauseEvents() {
  if (GetKey(PAUSE).bReleased) pause = false;
  if (GetKey(olc::Key::F10).bReleased) running = false;
}


void TetrisGUI::_readTetrisEvents() {
  if (GetKey(PAUSE).bPressed) {
    pause = true;
    return;
  }

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


