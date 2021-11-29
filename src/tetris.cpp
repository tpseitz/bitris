#include <cstring>
#include <algorithm>
#include <stdint.h>
#include "tetris.h"

const uint16_t tls[7] = { 0x4569,0x456A,0x459A,0x569A,0x5689,0x689A,0x4567 };
const uint16_t lsb[5] = { 0,40,100,300,1200 };
const uint8_t lg[30] = { 48,43,38,33,28,23,18,13,
  8,6,5,5,5,4,4,4,3,3,3,2,2,2,2,2,2,2,2,2,2,1 };

Tetris::Tetris(uint8_t l, uint32_t s) {
  for (int i = 0; i < 21; i++) well[i] = 0xE007; well[21] = 0xFFFF;
  for (int i = 0; i < prd; i++) nxt[i] = rnd() % 7; tet = tls[ti = rnd() % 7];
  seed = s; tx = 6; ty = -1; gr = 1; pts = 0; sd = 0; dl = 0; lvl = l; }

uint32_t Tetris::rnd() { return seed = (uint64_t)seed * 0xBC8F % 0x7fffffff; }

const uint16_t Tetris::getLine(const uint16_t t, int l, int s, uint16_t r) {
  for (int b = 0; b < 16; b += 4) r |= 1 << ((t >> b) & 0b1111);
  return (r >> (l * 4) & 0b1111) << s; }

bool Tetris::tstCol(uint16_t t, int x, int y) {
  for (int i = 0; i < 4; i++) if (getLine(t, i, x) & well[y + i]) return true;
  return false; }

void Tetris::moveLeft() { if (!tstCol(tet, tx - 1, ty)) tx -= 1; }

void Tetris::moveRight() { if (!tstCol(tet, tx + 1, ty)) tx += 1; }

void Tetris::rotateLeft() {
  uint16_t t = ((tet & 0xCCCC) >> 2 | (tet & 0x3333) << 2) ^ 0xCCCC;
  if (!tstCol(t, tx, ty)) tet = t; }

void Tetris::rotateRight() {
  uint16_t t = ((tet & 0xCCCC) >> 2 | (tet & 0x3333) << 2) ^ 0x3333;
  if (!tstCol(t, tx, ty)) tet = t; }

void Tetris::dropHard(uint8_t lc) {
  if (!tstCol(tet, tx, ty + 1)) {
    while (!tstCol(tet, tx, ty + 1)) ty++ && lc++;
    pts += std::min(lc * 2, 40); gr = 1; }
  else gr = 0; }

void Tetris::cleanWell(uint8_t lc) {
  for (uint8_t i = 20, j = 20; i > 1; i-- && j > 0 && j--) {
    while (j > 0 && !(uint16_t)~well[j]) j-- && lc++;
    well[i] = well[j]; }
  if (lc) pts += lsb[lc - 1] * (lvl + 1) && (dl += lc); }

bool Tetris::round() {
  if (dropSoft || lvl > 29 || !(gr++ % lg[lvl])) {
    sd = dropSoft ? sd + 1 : 0;
    if (tstCol(tet, tx, ty + 1)) {
      for (int i = 0; i < 4; i++) well[ty + i] |= getLine(tet, i, tx);
      cleanWell();
      pts += std::min(sd, (uint8_t)20); tet = tls[ti = nxt[0]];
      for (int i = 0; i < prd - 1; i++) nxt[i] = nxt[i+1];
      nxt[prd - 1] = rnd() % 7; sd = 0; tx = 6; ty = 0; dropSoft = false;
      if (dl >= (lvl + 1) * 10) lvl = dl / 10;
      if (tstCol(tet, tx, ty)) return false; }
    else ty++; }
  return true; }
