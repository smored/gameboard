#pragma once

// Game
#define TARGET_FPS 20
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define NUM_BRICKS_H 16
#define NUM_BRICKS_V 4
#define OLED_ADDR 0x3C 
#define SPINMULT 4 // effective 'speed' of spinning the encoder
#define MIN_T_ENC 500 // debounce time for encoder

// Pins
#define BUTTON_PIN 25
#define ENC_BUTTON_PIN 13
#define BUZZER_PIN 2
#define MY_LED_BUILTIN 1
#define ENC_A 12
#define ENC_B 11
#define SDA_PIN 22 
#define SCL_PIN 14

// math
//#define SQRT2OVER2 = sqrt(2)/2