#include <Bounce2.h>

const int ledR = 5;
const int ledV = 4;
const int but = 3;

int statoLed = 0; // 0 = rosso, 1 = verde

Bounce debouncer = Bounce(); // crea un oggetto Bounce

void setup() {
  pinMode(ledR, OUTPUT);
  pinMode(ledV, OUTPUT);
  pinMode(but, INPUT); // usi una resistenza pull-down esterna

  debouncer.attach(but);       // collega il pulsante al debouncer
  debouncer.interval(25);      // imposta l'intervallo di debounce (in ms)
}

void loop() {
  debouncer.update(); // aggiorna lo stato del pulsante

  if (debouncer.fell()) { // rileva il fronte di salita (pressione)
    statoLed = 1 - statoLed;

    if (statoLed == 0) {
      digitalWrite(ledR, HIGH);
      digitalWrite(ledV, LOW);
    } else {
      digitalWrite(ledR, LOW);
      digitalWrite(ledV, HIGH);
    }
  }
}

