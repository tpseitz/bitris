void _drawBits(uint64_t dt, int bits) {
  std::cout << "0b";
  for (int i = bits - 1; i >= 0; i--) {
    std::cout << (((dt >> i) & 1) ? '1' : '0');
  }
  std::cout << std::endl;
}

void drawBits(uint8_t  dt) { _drawBits(dt, 8);  }
void drawBits(uint16_t dt) { _drawBits(dt, 16); }
void drawBits(uint32_t dt) { _drawBits(dt, 32); }
void drawBits(uint64_t dt) { _drawBits(dt, 64); }

const char hxc[] = "0123456789ABCDEF";
void _drawHex(uint64_t dt, int bits) {
  std::cout << "0x";
  for (int i = bits - 4; i >= 0; i -= 4) std::cout << hxc[(dt >> i) & 0b1111];
  std::cout << std::endl;
}

void drawHex(const uint8_t *dt, int len) {
  std::cout << "0x";
  for (int i = 0; len ? i < len : dt[i]; i++)
    std::cout << hxc[(dt[i] >> 4) & 0b1111] << hxc[dt[i] & 0b1111];
  std::cout << std::endl;
}

void drawHex(const uint8_t *dt) { drawHex(dt, 0);  }

void drawHex(const uint8_t  dt) { _drawHex(dt, 8);  }
void drawHex(const uint16_t dt) { _drawHex(dt, 16); }
void drawHex(const uint32_t dt) { _drawHex(dt, 32); }
void drawHex(const uint64_t dt) { _drawHex(dt, 64); }

void drawString(const uint8_t *dt, int len) {
  std::cout << '"';
  for (int i = 0; len ? i < len : dt[i]; i++)
    if (dt[i] > 32 && dt[i] < 128) std::cout << (char)dt[i];
    else std::cout << "0x" << hxc[(dt[i] >> 4) & 0b1111] << hxc[dt[i] & 0b1111];
  std::cout << '"' << std::endl;
}

void drawString(const uint8_t *dt) { drawString(dt, 0);  }
