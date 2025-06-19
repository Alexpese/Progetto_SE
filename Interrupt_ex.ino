#include <Bounce2.h>

const int bPin = 18;
const int ledA = 48;
const int ledG = 52;
const int ledB = 50;

bool state = false;

Bounce debouncer = Bounce();

void setup() {
  Serial.begin(115200);
  pinMode(bPin, INPUT);  // Usa INPUT se hai una resistenza esterna
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(ledG, OUTPUT);

  debouncer.attach(bPin);
  debouncer.interval(10);  // 10 ms di debounce
}

void loop() {
  debouncer.update();

  if (debouncer.fell()) {  // Rileva il fronte di discesa (pressione)
    state = !state;
    Serial.println(state);
  }

  digitalWrite(ledA, state);
  digitalWrite(ledB, state);
  digitalWrite(ledG, !state);
}

