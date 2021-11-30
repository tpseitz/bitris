#include "tetris_gui.h"


void TetrisGUI::_drawWell() {
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


void TetrisGUI::_drawNext() {
  FillRect(0, 0, 4 * block_w, 4 * block_h, 0xff000000);
  uint16_t ln;
  for (int y = 0; y < 4; y++) {
    ln = tetris->getLine(tls[tetris->nxt[0]], y, 0);
    for (int x = 0; x < 4; x++)
      if (ln >> x & 1) DrawSprite(x * block_w, y * block_h,
        block[tetris->lvl % THEMES][tetris->nxt[0]]);
  }
}


void TetrisGUI::_drawTetris() {
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

