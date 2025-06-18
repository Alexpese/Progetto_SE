#include <Bounce2.h>

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
  debouncer.interval(25);      // imposta l'intervallo di debounce (in ms)
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
    Serial.println("Segnale troppo debole, ignorato");
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

    if (freq < 70 || freq > 400) {
      Serial.print("Frequenza fuori range: ");
      Serial.print(freq, 2);
      Serial.println(" Hz");
    } else {
      checkNote(freq, accStandard[targetNote].nome, accStandard[targetNote].frequenza);
    }
  } else {
    Serial.println("Nessuna frequenza rilevata");
  }
}

void checkButton() {
  if (debouncer.rose()) {
    targetNote = (targetNote + 1) % 6;
    Serial.print("Nota selezionata: ");
    Serial.println(accStandard[targetNote].nome);
  }
}

void checkNote(double freq, const char* noteName, double targetFreq) {
  double diff = freq - targetFreq;

  const double toleranceBelow = 8.0;   // tolleranza sotto
  const double toleranceAbove = 10.0;  // tolleranza sopra

  Serial.print("Frequenza rilevata: ");
  Serial.print(freq, 2);
  Serial.print(" Hz → ");

  if (diff >= -toleranceBelow && diff <= toleranceAbove) {
    Serial.print("CORRETTA! ");
    digitalWrite(ledA, LOW);
    digitalWrite(ledB, LOW);
    digitalWrite(ledG, HIGH);
  } else if (diff < -toleranceBelow) {
    Serial.print("TROPPO BASSA ");
    digitalWrite(ledA, LOW);
    digitalWrite(ledB, HIGH);
    digitalWrite(ledG, LOW);
  } else {
    Serial.print("TROPPO ALTA ");
    digitalWrite(ledA, HIGH);
    digitalWrite(ledB, LOW);
    digitalWrite(ledG, LOW);
  }

  Serial.print("(Nota: ");
  Serial.print(noteName);
  Serial.print(", attesa: ");
  Serial.print(targetFreq, 2);
  Serial.println(" Hz)");
}

// TODO:
/*
  1-Aggiungere la parte del metronomo
  2-Aggregare il tutto in modo da poter switchare tra le modalità
  
*/
