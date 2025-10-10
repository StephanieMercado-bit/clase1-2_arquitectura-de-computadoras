/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://docs.arduino.cc/hardware/

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://docs.arduino.cc/built-in-examples/basics/Blink/
*/

// the setup function runs once when you press reset or power the board
int verdePin = 13;
int delaypunto = 200;
int espacio = 400;
int delayraya = 600;

// Nuevos tiempos para el pulso lento
int slowOnTime = 2000;   // LED encendido lento (constante)
int slowOffTime = 2000;  // LED apagado lento

void setup() {
  pinMode(verdePin, OUTPUT);  // Inicializa el LED integrado como salida
}

// Funciones para código Morse (punto y raya)
void sendDot() {
  digitalWrite(verdePin, HIGH);
  delay(delaypunto);
  digitalWrite(verdePin, LOW);
  delay(espacio);
}

void sendDash() {
  digitalWrite(verdePin, HIGH);
  delay(delayraya);
  digitalWrite(verdePin, LOW);
  delay(espacio);
}

// Función para enviar el código Morse SOS: ... --- ...
void sendSOS() {
  // S: tres puntos
  sendDot();
  sendDot();
  sendDot();
  delay(espacio * 2); // pausa entre letras

  // O: tres rayas
  sendDash();
  sendDash();
  sendDash();
  delay(espacio * 2); // pausa entre letras

  // S: tres puntos
  sendDot();
  sendDot();
  sendDot();
  delay(espacio * 4); // pausa antes de siguiente acción
}

// Función para hacer 7 pulsos rápidos iguales
void sendSevenPulses() {
  for (int i = 0; i < 7; i++) {
    digitalWrite(verdePin, HIGH);
    delay(delaypunto);
    digitalWrite(verdePin, LOW);
    delay(espacio);
  }
}

// Función para hacer 7 pulsos lentos y constantes
void sendSevenSlowPulses() {
  for (int i = 0; i < 7; i++) {
    digitalWrite(verdePin, HIGH);
    delay(slowOnTime);
    digitalWrite(verdePin, LOW);
    delay(slowOffTime);
  }
}

void loop() {
  sendSOS();             // Primero el código Morse SOS
  sendSevenPulses();     // Luego 7 pulsos rápidos
  sendSOS();             // Después otra vez el código Morse SOS
  sendSevenSlowPulses(); // Finalmente 7 pulsos lentos y constantes

  delay(3000);           // Pausa antes de repetir todo el ciclo
}
