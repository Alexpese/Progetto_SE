#include <LiquidCrystal.h>

// LiquidCrystal(rs, e, d4, d5, d6, d7)
LiquidCrystal lcd(32, 30, 28, 26, 24, 22);

void setup() {
  lcd.begin(16, 2);         // Inizializza display 16x2
  lcd.print("LCD collegato!");
  lcd.setCursor(0, 1);
  lcd.print("Funziona? :)");
}

void loop() {
  // Nulla nel loop
}

