/*
 * Text mode Tetris
 * ================
 *
 * Default keys are `z` for moving left, `c` moving right, `,` for rotating
 * left, `.` for rotating right and enter for hard drop. You can change keys by
 * tampering the source code. Also starting level, zero by default, can be
 * changed this way.
 *
 * Basic structure is for main method to start game routine that starts game
 * timing drawing thread and wait for user key presses. Curses -library is
 * used to disable std::cin buffering so that key events can be received when
 * they are made.
 *
 * For compiling you will need curses -library. If you want to modify this
 * program or use it as an example surely "the source code will document
 * itself". It can be compiled with GNU C++ compiler with command
 *
 * `g++ -otetris tetris_tui.cpp -lpthread -lcurses -std=c++17`
 *
 */
#include <iostream>
#include <chrono>
#include <thread>
#include <curses.h>

#include "tetris.cpp"

void drawWell(Tetris *t) {
  std::cout << (char)27 << "[H";
  uint16_t ln[13] = {0};
  for (int y = 1; y < 22; y++) {
    uint16_t l = y - 4 <= t->ty && t->ty <= y
      ? t->getLine(t->tet, y - t->ty, t->tx) : 0;
    for (int x = 2; x < 14; x++)
      if ((t->well[y] >> x) & 1) ln[x - 2] = 0x5D5B;
      else if ((l >> x) & 1) ln[x - 2] = 0x2928;
      else ln[x - 2] = 0x2020;
    std::cout << (char)13 << (char*)ln << std::endl; }
  std::cout << (char)13 << "Level: " << (int)t->lvl << " | Lines: "
    << (int)t->dl << " | Score: " << t->pts << (char)10 << (char)13;
  uint16_t nxt = tls[t->nxt[0]], tl = 0;
  for (int i = 0; i < 16; i++) {
    if (!(i % 4)) {
      tl = t->getLine(nxt, i/4, 0);
      std::cout << (char)10 << (char)13; }
    std::cout << ((tl >> i % 4) & 1 ? "()" : "  "); }
  std::cout << std::endl; }

bool gameon = true;
void timed(Tetris *tetris) {
  drawWell(tetris);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  while (gameon = tetris->round()) {
    drawWell(tetris);
    std::this_thread::sleep_for(std::chrono::milliseconds(20)); }
  drawWell(tetris);
  gameon = false; }

void runGame(int level) {
  initscr(); cbreak(); noecho();
  Tetris *tetris = new Tetris(level);
  std::thread game (timed, tetris);
  while (gameon) {
    char c = getch();
    if (c == 'z') tetris->moveLeft();
    if (c == 'c') tetris->moveRight();
    if (c == ',') tetris->rotateLeft();
    if (c == '.') tetris->rotateRight();
    if (c == '\n') tetris->dropHard(); }
  endwin();
  game.join();
  std::cout << "Level: " << (int)tetris->lvl << std::endl << "Lines: "
    << (int)tetris->dl << std::endl << "Score: " << tetris->pts << std::endl;
  delete tetris; }

int main() { runGame(0); }
