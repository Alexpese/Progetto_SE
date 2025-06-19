#include <Bounce2.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

// LiquidCrystal(rs, e, d4, d5, d6, d7)
LiquidCrystal lcd(32, 30, 28, 26, 24, 22);

const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {10, 9, 8, 7};
byte colPins[COLS] = {6, 5, 4, 3};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

const int micPin = A0;
const int samples = 512;
const double samplingFrequency = 9000;  // Hz
int audioBuffer[samples];
unsigned int samplingPeriodUs;
const int ledA = 50;
const int ledB = 48;
const int ledG = 52;
const int butN = 2;
const int bMod = 18;
int targetNote = 0;
const int buzPin = 44;
int tempo = 60;
int tdc[3] = {0, 6, 0};
int i = 0;

unsigned long lastBeat = 0; // Tempo dell'ultimo beep


volatile int mod = 0;

Bounce debouncer = Bounce(); // crea un oggetto Bounce
Bounce d2 = Bounce();

struct Nota {
  char* nome;
  double frequenza;
};

const Nota accStandard[] = {
  {"E2", 82.41},
  {"A2", 110.00},
  {"D3", 160.83},
  {"G3", 211.00},
  {"B3", 264.71},
  {"E4", 360.00}
};

void changeState() {
  mod = (mod + 1) % 2;
}

void setup() {
  Serial.begin(115200);

  samplingPeriodUs = round(1000000.0 / samplingFrequency);

  pinMode(micPin,INPUT);
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(butN, INPUT);

  debouncer.attach(butN);       // collega il pulsante al debouncer
  debouncer.interval(10);      // imposta l'intervallo di debounce (in ms)

  d2.attach(bMod);
  d2.interval(10);

  attachInterrupt(digitalPinToInterrupt(bMod), changeState, RISING);

  lcd.begin(16, 2);
  lcd.print("Nota Selezionata");
  lcd.setCursor(0, 1);
  lcd.print("E2              ");

}

void loop() {
  d2.update();
  if (mod == 0) {
    Accord();
  } else {
    Metronome();
  }
}

void Accord() {
  // Aggiorna subito il pulsante
  debouncer.update();
  checkButton(); // Controlla se nota da cambiare

  // 1. Acquisizione audio
  long energy = 0;
  for (int i = 0; i < samples; i++) {
    unsigned long t0 = micros();
    int val = analogRead(micPin) - 512;
    audioBuffer[i] = val;
    energy += (long)val * val;

    // Anche qui aggiorna il pulsante durante l'attesa
    while (micros() - t0 < samplingPeriodUs) {
      debouncer.update();
      checkButton();
    }
  }

  // 2. Controllo energia
  const long energyThreshold = 50000;
  if (energy < energyThreshold) {
    return;
  }

  // 3. Autocorrelazione
  int maxLag = samples / 2;
  int bestLag = -1;
  long bestCorr = 0;

  for (int lag = 10; lag < maxLag; lag++) {
    long sum = 0;
    for (int i = 0; i < samples - lag; i++) {
      sum += (long)audioBuffer[i] * audioBuffer[i + lag];
    }

    if (sum > bestCorr) {
      bestCorr = sum;
      bestLag = lag;
    }
  }

  if (bestLag > 0) {
    double freq = samplingFrequency / bestLag;

    if (freq > 70 && freq < 400) {
      checkNote(freq, accStandard[targetNote].nome, accStandard[targetNote].frequenza);
    }
  }
}

void checkButton() {
  if (debouncer.rose()) {
    lcd.clear();
    targetNote = (targetNote + 1) % 6;
    lcd.print("Nota Selezionata");
    lcd.setCursor(0,1);
    lcd.print(accStandard[targetNote].nome);
    lcd.print("              ");
  }
}

void checkNote(double freq, const char* noteName, double targetFreq) {
  double diff = freq - targetFreq;

  const double toleranceBelow = 8.0;   // tolleranza sotto
  const double toleranceAbove = 10.0;  // tolleranza sopra

  bool inTune = (diff >= -toleranceBelow && diff <= toleranceAbove);

  // LED feedback
  digitalWrite(ledA, diff < -toleranceBelow);
  digitalWrite(ledB, diff > toleranceAbove);
  digitalWrite(ledG, inTune);
}

void Metronome() {
  // Gestione dell'input da keypad
  char customKey = customKeypad.getKey();
  if (customKey && customKey >= '0' && customKey <= '9') {
    if (i < 3) {
      tdc[i] = customKey - '0';
      i++;
    }
    if (i == 3) {
      tempo = tdc[0] * 100 + tdc[1] * 10 + tdc[2];
      lcd.clear();
      lcd.print("Tempo:");
      lcd.setCursor(0,1);
      lcd.print(tempo);
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
