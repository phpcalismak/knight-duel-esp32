#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <Arduino.h> // For types like bool, unsigned long, etc.

// --- Game States ---
enum GameState {
  TITLE_SCREEN,
  PLAYING,
  GAME_OVER
};
//can
#define HEALTH_PICKUP_AMOUNT    10   // Ne kadar can vereceği
#define HEALTH_PICKUP_SPAWN_CHANCE 10 // Her frame'de can noktası oluşma şansı (örneğin 1000'de 10)
#define HEALTH_PICKUP_FALL_SPEED 2.0 // Can noktasının düşme hızı (piksel/frame)

// --- Joystick Pins ---
#define JOY1_X_PIN 34
#define JOY1_Y_PIN 35
#define JOY1_BTN_PIN 32

#define JOY2_X_PIN 33
#define JOY2_Y_PIN 25
#define JOY2_BTN_PIN 27

// --- Buzzer Pin ---
#define BUZZER_PIN 26

// --- Colors ---
#define TFT_SKYBLUE 0x867D
#define TFT_BROWN 0xA145
#define TFT_MOUNTAIN 0x5ACB
#define NINJA_BLUE TFT_BLUE
#define NINJA_RED TFT_RED

// --- Screen Dimensions ---
const int SCREEN_WIDTH = 320;
const int SCREEN_HEIGHT = 240;

// --- Physics Constants ---
const float ACCELERATION = 1.0;
const float DECELERATION = 2.0;
const float FRICTION = 0.3;
const float STOP_THRESHOLD = 0.05;
const float MAX_SPEED = 5.0;
const float GRAVITY = 0.5;
const float JUMP_FORCE = -10.0;
const int GROUND_LEVEL = 180;

// --- Attack Constants ---
const unsigned long ATTACK_COOLDOWN = 300;
const unsigned long ANIM_SPEED = 150; // Milliseconds per animation frame
const unsigned long STRONG_ATTACK_THRESHOLD = 500;
const unsigned long SUPER_ATTACK_THRESHOLD = 1000;

// --- Game Timing and Delays ---
const unsigned long FRAME_INTERVAL = 30; // Target ~33 FPS
const unsigned long TITLE_SCREEN_DELAY = 3000;
const unsigned long GAME_OVER_DELAY = 3000;
const unsigned long DEBUG_INTERVAL = 500;

// --- Joystick Calibration ---
const int JOYSTICK_DEADZONE = 500; // Adjust based on your joysticks

#endif // GAME_CONFIG_H