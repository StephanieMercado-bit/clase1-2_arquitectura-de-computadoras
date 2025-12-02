// Pines
const int BTN_SOS     = 11;
const int BTN_BLINK   = 10;
const int BTN_FADE    = 9;
const int BTN_STOP    = 13;

const int LED_PIN     = 23;   // Pin PWM del ESP32

// Estados
enum Mode { IDLE, SOS, BLINK, FADE };
Mode currentMode = IDLE;

void setup() {
  Serial.begin(115200);

  pinMode(BTN_SOS,   INPUT);
  pinMode(BTN_BLINK, INPUT);
  pinMode(BTN_FADE,  INPUT);
  pinMode(BTN_STOP,  INPUT);

  pinMode(LED_PIN, OUTPUT);

  // Configuración PWM nativa del ESP32 Core 3.x
  analogWriteResolution(LED_PIN, 8);   // Resolución: 0–255
  analogWriteFrequency(LED_PIN, 5000); // Frecuencia 5 kHz
}

void loop() {

  if (digitalRead(BTN_SOS)) {
    currentMode = SOS;
    Serial.println("Modo: SOS");
    delay(200);
  }

  if (digitalRead(BTN_BLINK)) {
    currentMode = BLINK;
    Serial.println("Modo: BLINK");
    delay(200);
  }

  if (digitalRead(BTN_FADE)) {
    currentMode = FADE;
    Serial.println("Modo: FADE");
    delay(200);
  }

  if (digitalRead(BTN_STOP)) {
    currentMode = IDLE;
    Serial.println("Modo: STOP");
    analogWrite(LED_PIN, 0);
    delay(200);
  }

  switch (currentMode) {
    case SOS:   runSOS();   break;
    case BLINK: runBlink(); break;
    case FADE:  runFade();  break;
    case IDLE:  break;
  }
}


// ================== SOS ==================
void runSOS() {
  sosDot(); sosDot(); sosDot();
  sosDash(); sosDash(); sosDash();
  sosDot(); sosDot(); sosDot();
}

void sosDot() {
  analogWrite(LED_PIN, 255);
  delay(200);
  analogWrite(LED_PIN, 0);
  delay(200);
  checkInterrupt();
}

void sosDash() {
  analogWrite(LED_PIN, 255);
  delay(600);
  analogWrite(LED_PIN, 0);
  delay(200);
  checkInterrupt();
}


// ============== PARPADEO ==============
void runBlink() {
  analogWrite(LED_PIN, 255);
  delay(500);
  checkInterrupt();

  analogWrite(LED_PIN, 0);
  delay(500);
  checkInterrupt();
}


// ====================== FADE ======================
void runFade() {
  for (int i = 0; i <= 255; i += 5) {
    analogWrite(LED_PIN, i);
    delay(15);
    checkInterrupt();
  }

  for (int i = 255; i >= 0; i -= 5) {
    analogWrite(LED_PIN, i);
    delay(15);
    checkInterrupt();
  }
}


// =================================================
void checkInterrupt() {
  if (digitalRead(BTN_SOS) ||
      digitalRead(BTN_BLINK) ||
      digitalRead(BTN_FADE) ||
      digitalRead(BTN_STOP)) {
    return;
  }
}
