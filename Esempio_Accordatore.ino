const int micPin = A7;
const int samples = 512;
const double samplingFrequency = 9000;  // Hz
int audioBuffer[samples];
unsigned int samplingPeriodUs;

void setup() {
  Serial.begin(115200);
  samplingPeriodUs = round(1000000.0 / samplingFrequency);
}

void loop() {
  // 1. Acquisizione audio
  long energy = 0;
  for (int i = 0; i < samples; i++) {
    unsigned long t0 = micros();
    int val = analogRead(micPin) - 512;  // rimuove offset DC
    audioBuffer[i] = val;
    energy += (long)val * val;
    while (micros() - t0 < samplingPeriodUs)
      ;
  }

  // 2. Controllo soglia minima di energia
  const long energyThreshold = 50000;  // regola in base al segnale
  if (energy < energyThreshold) {
    Serial.println("Segnale troppo debole, ignorato");
    delay(300);
    return;
  }

  // 3. Calcolo autocorrelazione per frequenza
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

    // 4. Ignora frequenze fuori range chitarra (70 - 350 Hz)
    if (freq < 70 || freq > 350) {
      Serial.print("Frequenza fuori range: ");
      Serial.print(freq, 2);
      Serial.println(" Hz");
    } else {
      checkNote(freq, "A2", 110.00);
    }
  } else {
    Serial.println("Nessuna frequenza rilevata");
  }

  delay(500);
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
  } else if (diff < -toleranceBelow) {
    Serial.print("TROPPO BASSA ");
  } else {
    Serial.print("TROPPO ALTA ");
  }

  Serial.print("(Nota: ");
  Serial.print(noteName);
  Serial.print(", attesa: ");
  Serial.print(targetFreq, 2);
  Serial.println(" Hz)");
}