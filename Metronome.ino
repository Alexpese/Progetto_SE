#include <Keypad.h>

const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

const int buzPin = 10;
int tempo = 60;
int tdc[3] = {0, 6, 0};
int i = 0;

unsigned long lastBeat = 0; // Tempo dell'ultimo beep

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Gestione dell'input da keypad
  char customKey = customKeypad.getKey();
  if (customKey && customKey >= '0' && customKey <= '9') {
    if (i < 3) {
      tdc[i] = customKey - '0';
      i++;
    }
    if (i == 3) {
      tempo = tdc[0] * 100 + tdc[1] * 10 + tdc[2];
      Serial.print("Nuovo tempo: ");
      Serial.println(tempo);
      i = 0;
    }
  }

  // Calcolo del delay tra i beep
  unsigned long interval = 60000UL / tempo;

  // Suona il beep se è passato abbastanza tempo
  if (millis() - lastBeat >= interval) {
    tone(buzPin, 262, 50); // Beep di 50 ms
    lastBeat = millis();
  }
}
