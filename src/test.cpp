#include <iostream>

struct Point { uint8_t x; uint8_t y; };
void drawTet(uint16_t t) {
  Point pt[4];
  for (int i = 0; i < 16; i += 4)
    pt[i / 4] = { (uint8_t)(t >> i & 0b11), (uint8_t)(t >> (i + 2) & 0b11) };
  for (Point p: pt) std::cout << (int)p.x << ',' << (int)p.y << "  ";
  std::cout << std::endl;
  for (int x = 0, y = 0; y < 4; ++x == 4 && ++y && (x = 0)) {
    char c = ' ';
    for (Point p: pt) if (p.x == x && p.y == y) c = 'O';
    std::cout << c;
    if (x == 3) std::cout << std::endl;
//    std::cout << x << ',' << y << std::endl;
  }
  std::cout << std::endl;
}

//uint16_t tetrominos[] = { 0x9A85,0xD956,0x6A9D,0xA695,0x59AE,0xEA65,0xD951 };
//uint16_t tetrominos[] = { 0x5916,0x456A,0xA954,0x9A56,0x6598,0x7659,0x4567 };
uint16_t tetrominos[] = { 0x4659,0x456A,0xA954,0x9A56,0x6598,0x89A6,0x4567 };

uint16_t rotateRight(uint16_t t) {
  return ((t & 0xCCCC) >> 2 | (t & 0x3333) << 2) ^ 0x3333;
}

uint16_t rotateLeft(uint16_t t) {
  return ((t & 0xCCCC) >> 2 | (t & 0x3333) << 2) ^ 0xCCCC;
}

//uint8_t getLine(uint16_t t, uint16_t l) {
//  uint16_t r = 0;
//  for (int b = 0; b < 16; b += 4) {
//    std::cout << ((t >> (b)) & 0b11) << ','
//       << ((t >> (b + 2)) & 0b11) << ' ' << l << std::endl;
//    if (((t >> (b + 2)) & 0b11) == l) r |= 1 << ((t >> b) & 0b11);
//      std::cout << ((t >> (b)) & 0b11) << std::endl;
//  }
//  return r;
//  for (int b = 0; b < 16; b += 4) r |= 1 << ((t >> b) & 0b1111);
//  return (r >> (l * 4) & 0b1111) << 4;
//}

uint8_t getLine(uint16_t t, uint16_t l) {
  int s = 0;
  uint16_t r = 0;
  for (int b = 0; b < 16; b += 4) r |= 1 << ((t >> b) & 0b1111);
  return (r >> (l * 4) & 0b1111) << (s + 4); }

const uint64_t dt[]={ 0xEA659A85D951A695,0x00006A9D59AED956,0x04B0012C00640028,
  0x0D12171C21262B30,0x0404040505050608,0x0202020202030303,0x0000010202020202};
const uint16_t *tet = (uint16_t*)(&dt), *lsb = (uint16_t*)(&dt) + 7;
const uint8_t *lg = (uint8_t*)(&dt) + 24;

#include "draw_data.cpp"

int main() {
//  for (uint16_t t: tetrominos)
//    drawHex(rotateRight(rotateRight(t)));
  uint16_t t = tetrominos[2];
//  t = 0b0100011001011001;
//  t = 0b0001100101010110;
//  t = 0x159D;
  drawHex(t);
  drawBits(t);

//  for (uint16_t t: tetrominos) drawTet(t);
//  for (uint16_t t: tetrominos) drawHex(t);
//  drawBits((uint16_t)0b1100110011001100);
//  drawHex((uint16_t)0b1100110011001100);
//  drawBits((uint16_t)0b0011001100110011);
//  drawHex((uint16_t)0b0011001100110011);

//  drawHex((uint16_t)48271);

//  drawBits(((uint16_t)~0b0011001100110011));

//  drawHex(lg, 31);
//  for (int i = 0; i < 32; i += 8) drawHex(*(uint64_t*)&lg[i]);
//  for (int i = 0; i < 31; i++) std::cout << (int)lg[i] << ',';
//  std::cout << std::endl;

//  drawHex(*(uint64_t*)(&lsb[1]));

//  uint16_t t = tetrominos[2];
//  uint16_t t = tet[2];

//  drawTet(t);
  for (int i = 0; i < 4; i++) drawBits(getLine(t, i));
//  getLine(t, 0);
//  getLine(t, 1);
//  getLine(t, 2);
//  getLine(t, 3);

//  drawTet(t);
//  t = rotateLeft(t);
//  drawTet(t);
//  t = rotateLeft(t);
//  drawTet(t);
//  t = rotateLeft(t);
//  drawTet(t);
//  t = rotateLeft(t);
//  drawTet(t);

//  drawTet(t);
//  t = rotateRight(t);
//  drawTet(t);
//  t = rotateRight(t);
//  drawTet(t);
//  t = rotateRight(t);
//  drawTet(t);
//  t = rotateRight(t);
//  drawTet(t);
}
