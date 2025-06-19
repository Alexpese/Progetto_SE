#include <Bounce2.h>
#include <LiquidCrystal.h>

// LiquidCrystal(rs, e, d4, d5, d6, d7)
LiquidCrystal lcd(32, 30, 28, 26, 24, 22);

const int micPin = A0;
const int samples = 512;
const double samplingFrequency = 9000;  // Hz
int audioBuffer[samples];
unsigned int samplingPeriodUs;
const int ledA = 50;
const int ledB = 48;
const int ledG = 52;
const int butN = 2;
int targetNote = 0;

Bounce debouncer = Bounce(); // crea un oggetto Bounce

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

void setup() {
  Serial.begin(115200);
  samplingPeriodUs = round(1000000.0 / samplingFrequency);
  pinMode(micPin,INPUT);
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(butN, INPUT);
  debouncer.attach(butN);       // collega il pulsante al debouncer
  debouncer.interval(1);      // imposta l'intervallo di debounce (in ms)
  lcd.begin(16, 2);
  lcd.print("Nota Selezionata");
  lcd.setCursor(0, 1);
  lcd.print("E2              ");

}

void loop() {
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
    //Serial.println("Segnale troppo debole, ignorato");
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

  //Serial.print("Frequenza rilevata: ");
  //Serial.print(freq, 2);
  //Serial.print(" Hz → ");

  bool inTune = (diff >= -toleranceBelow && diff <= toleranceAbove);

  // LED feedback
  digitalWrite(ledA, diff < -toleranceBelow);
  digitalWrite(ledB, diff > toleranceAbove);
  digitalWrite(ledG, inTune);


  //Serial.print("(Nota: ");
  //Serial.print(noteName);
  //Serial.print(", attesa: ");
  //Serial.print(targetFreq, 2);
  //Serial.println(" Hz)");
}

// TODO:
/*
  1-Aggiungere la parte del metronomo
  2-Aggregare il tutto in modo da poter switchare tra le modalità
*/
