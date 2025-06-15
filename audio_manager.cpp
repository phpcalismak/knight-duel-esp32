#include "audio_manager.h"

void playSound(int soundType) {
  switch(soundType) {
    case 1: // Jump sound
      tone(BUZZER_PIN, 1000, 50);
      break;
    case 2: // Hit sound
      tone(BUZZER_PIN, 500, 100);
      break;
    case 3: // Victory sound
      tone(BUZZER_PIN, 1500, 100);
      delay(100); // Small delay between notes
      tone(BUZZER_PIN, 2000, 200);
      break;
    // Add more sounds as needed
  }
}