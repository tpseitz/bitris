#ifndef TETRIS_H
#define TETRIS_H

extern const uint16_t tls[7];
extern const uint16_t lsb[5];
extern const uint8_t lg[30];

struct Tetris {
  uint32_t seed = 0xABCD, pts = 0, gr = 1;
  uint16_t well[22] {0}, prd = 5, tet;
  uint8_t tx = 6, ty = 0, lvl = 0, sd = 0, dl = 0, ti = 0, nxt[100] {0};
  bool dropSoft = false;

  Tetris(uint8_t, uint32_t=0xABCD);
  uint32_t rnd();
  const uint16_t getLine(const uint16_t, int, int, uint16_t=0);
  bool tstCol(uint16_t, int, int);
  void moveLeft();
  void moveRight();
  void rotateLeft();
  void rotateRight();
  void dropHard(uint8_t=0);
  void cleanWell(uint8_t=0);
  bool round();
};

#endif
