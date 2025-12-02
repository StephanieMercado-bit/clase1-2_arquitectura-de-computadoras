// =============================== 
// CONFIGURACIÓN DEL JUEGO
// ===============================

#define LEDS 6

// Pines de LED (solo Rojo y Azul)
int pinR[LEDS] = {13, 27, 33, 22, 18, 2};
int pinB[LEDS] = {14, 25, 23, 19, 4, 5};

// Buzzer
int buzzerPin = 15;

// Estado del juego
bool barco[LEDS];
bool fallado[LEDS];
int fallos = 0;

// ===============================
// CONTROL DE LEDs
// ===============================

void apagarLED(int i) {
  digitalWrite(pinR[i], HIGH);
  digitalWrite(pinB[i], HIGH);
}

void colorLED(int i, bool r, bool b) {
  digitalWrite(pinR[i], r ? LOW : HIGH);
  digitalWrite(pinB[i], b ? LOW : HIGH);
}

void azulFijo(int i) { colorLED(i, 0, 1); }
void rojoFijo(int i) { colorLED(i, 1, 0); }
void violeta(int i)  { colorLED(i, 1, 1); }

// ===============================
// BUZZER
// ===============================

void beep(int freq, int t) {
  tone(buzzerPin, freq);
  delay(t);
  noTone(buzzerPin);
}

// ===============================
// INICIALIZACIÓN DEL JUEGO
// ===============================

void asignarBarco() {
  for (int i = 0; i < LEDS; i++) {
    barco[i] = false;
    fallado[i] = false;
    apagarLED(i);
  }

  int pos = random(0, LEDS);
  barco[pos] = true;

  fallos = 0;
}

void parpadeoTodos() {
  for (int k = 0; k < 4; k++) {
    for (int i = 0; i < LEDS; i++) violeta(i);
    delay(150);
    for (int i = 0; i < LEDS; i++) apagarLED(i);
    delay(150);
  }
}

// ===============================
// SETUP
// ===============================

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));

  pinMode(buzzerPin, OUTPUT);

  for (int i = 0; i < LEDS; i++) {
    pinMode(pinR[i], OUTPUT);
    pinMode(pinB[i], OUTPUT);
    apagarLED(i);
  }

  asignarBarco();
}

// ===============================
// LOOP PRINCIPAL
// ===============================

void loop() {

  // LECTURA DEL NÚMERO POR SERIAL
  if (Serial.available()) {

    String data = Serial.readStringUntil('\n');
    data.trim();

    if (data.length() == 0) return;

    // VALIDAR QUE SEA 0–9
    if (data[0] < '0' || data[0] > '9') {
      Serial.println("Entrada invalida");
      return;
    }

    int pos = data.toInt();

    // VALIDAR RANGO 0–5
    if (pos < 0 || pos >= LEDS) {
      Serial.println("Posicion fuera de rango (0-5)");
      return;
    }

    // ==================================
    // VERIFICACIÓN
    // ==================================

    if (barco[pos]) {
      rojoFijo(pos);
      Serial.println("ACIERTO!");

      // sonido acierto
      beep(1200, 200);

      delay(1000);
      parpadeoTodos();
      asignarBarco();
      return;

    } else {
      fallado[pos] = true;
      azulFijo(pos);
      Serial.println("FALLO!");

      // sonido fallo
      beep(400, 200);

      fallos++;

      if (fallos >= 5) {
        Serial.println("PERDISTE");

        // sonido derrota
        beep(800, 200);
        beep(600, 200);
        beep(400, 400);

        parpadeoTodos();

        // mostrar barco
        for (int i = 0; i < LEDS; i++)
          if (barco[i]) rojoFijo(i);

        delay(2000);
        asignarBarco();
        return;
      }
    }
  }
}
