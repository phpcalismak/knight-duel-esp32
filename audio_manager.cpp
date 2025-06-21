#include "audio_manager.h"

void playSound(int soundType) {
  switch(soundType) {
    case 1: 
      tone(BUZZER_PIN, 1000, 50);
      break;
    case 2: 
      tone(BUZZER_PIN, 500, 100);
      break;
    case 3: 
      tone(BUZZER_PIN, 1500, 100);
      delay(100); 
      tone(BUZZER_PIN, 2000, 200);
      break;
  }
}
