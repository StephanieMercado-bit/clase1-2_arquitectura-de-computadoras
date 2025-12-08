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
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- PINES DEL TECLADO (confirmados por diagnóstico) ---
const int rowPins[4] = {2, 3, 4, 5};
const int colPins[4] = {6, 7, 8, 9};

// --- Tablero 3x3 ---
char board[9] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};

int selectedCell = -1; 
int turno = 1;  // 1 = X , 2 = O

// ------------------------------------------------------
//                     Dibujar Tablero
// ------------------------------------------------------
void drawBoard() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  // Líneas verticales
  display.drawLine(42, 0, 42, 64, SSD1306_WHITE);
  display.drawLine(84, 0, 84, 64, SSD1306_WHITE);

  // Líneas horizontales
  display.drawLine(0, 21, 128, 21, SSD1306_WHITE);
  display.drawLine(0, 42, 128, 42, SSD1306_WHITE);

  // Colocar X / O
  for (int i = 0; i < 9; i++) {
    int x = (i % 3) * 42 + 15;
    int y = (i / 3) * 21 + 4;
    display.setCursor(x, y);
    display.print(board[i]);
  }

  display.display();
}

// ------------------------------------------------------
//                    Leer Teclado
// ------------------------------------------------------
char readKeypad() {
  for (int r = 0; r < 4; r++) {

    digitalWrite(rowPins[r], LOW);

    for (int c = 0; c < 4; c++) {
      if (digitalRead(colPins[c]) == LOW) {

        delay(150); 
        while (digitalRead(colPins[c]) == LOW);

        const char keymap[4][4] = {
          {'1','2','3','A'},
          {'4','5','6','B'},
          {'7','8','9','C'},
          {'*','0','#','D'}
        };

        digitalWrite(rowPins[r], HIGH);
        return keymap[r][c];
      }
    }

    digitalWrite(rowPins[r], HIGH);
  }

  return '\0';
}

// ------------------------------------------------------
//                Verificar ganador
// ------------------------------------------------------
bool checkWinner(char p) {
  int combos[8][3] = {
    {0,1,2}, {3,4,5}, {6,7,8},
    {0,3,6}, {1,4,7}, {2,5,8},
    {0,4,8}, {2,4,6}
  };

  for (int i = 0; i < 8; i++) {
    if (board[combos[i][0]] == p &&
        board[combos[i][1]] == p &&
        board[combos[i][2]] == p) {
      return true;
    }
  }
  return false;
}

// ------------------------------------------------------
//               Verificar si hay empate
// ------------------------------------------------------
bool isDraw() {
  for (int i = 0; i < 9; i++) {
    if (board[i] == ' ') return false;
  }
  return true;
}

// ------------------------------------------------------
//                    Reiniciar juego
// ------------------------------------------------------
void resetGame() {
  for (int i = 0; i < 9; i++) board[i] = ' ';
  selectedCell = -1;
  turno = 1;
  drawBoard();
}

// ------------------------------------------------------
//                        SETUP
// ------------------------------------------------------
void setup() {
  Serial.begin(115200);

  for (int i = 0; i < 4; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }

  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  drawBoard();

  Serial.println("Jugador 1 = X");
  Serial.println("Jugador 2 = O");
}

// ------------------------------------------------------
//                         LOOP
// ------------------------------------------------------
void loop() {
  char key = readKeypad();
  if (key == '\0') return;

  Serial.print("Tecla presionada: ");
  Serial.println(key);

  // Selección de celda 1–9
  if (key >= '1' && key <= '9') {
    selectedCell = (key - '1'); 

    // Si está ocupada, no se puede usar
    if (board[selectedCell] != ' ') {
      Serial.println("Esa celda ya está ocupada.");
      selectedCell = -1;
      return;
    }

    // Determinar símbolo según turno
    char symbol = (turno == 1) ? 'X' : 'O';

    board[selectedCell] = symbol;
    drawBoard();

    // ¿Ganó?
    if (checkWinner(symbol)) {
      display.clearDisplay();
      display.setCursor(10, 25);
      display.setTextSize(2);
      display.print(symbol);
      display.print(" gana!");
      display.display();
      delay(2500);
      resetGame();
      return;
    }

    // ¿Empate?
    if (isDraw()) {
      display.clearDisplay();
      display.setCursor(10, 25);
      display.setTextSize(2);
      display.print("Empate!");
      display.display();
      delay(2500);
      resetGame();
      return;
    }

    // Cambiar turno
    turno = (turno == 1) ? 2 : 1;
    Serial.print("Turno del jugador ");
    Serial.println(turno);

    selectedCell = -1;
  }
}
