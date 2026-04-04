#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define MATRIX_WIDTH 13
#define MATRIX_HEIGHT 6

// 🔥 Пример 3 кадров огненной анимации (добавьте свои данные)
const uint32_t fireFrames[][MATRIX_WIDTH * MATRIX_HEIGHT] = {
  {
    0xFF0000, 0x880000, /* ... остальные 76 пикселей кадра 1 ... */ 0x000000
  },
  {
    0x990000, 0x550000, /* ... остальные 76 пикселей кадра 2 ... */ 0x000000
  },
  {
    0xAA0000, 0x660000, /* ... остальные 76 пикселей кадра 3 ... */ 0x000000
  }
};

const int fireFrameCount = sizeof(fireFrames) / sizeof(fireFrames[0]);

// 👇 Функция проигрывания анимации на матрице
void playFireAnimation(Adafruit_NeoPixel& pixels, int delayMs = 100) {
  for (int frame = 0; frame < fireFrameCount; frame++) {
    for (int y = 0; y < MATRIX_HEIGHT; y++) {
      for (int x = 0; x < MATRIX_WIDTH; x++) {
        int index = y * MATRIX_WIDTH + x;
        pixels.setPixelColor(index, fireFrames[frame][index]);
      }
    }
    pixels.show();
    delay(delayMs);
  }
}

#endif
