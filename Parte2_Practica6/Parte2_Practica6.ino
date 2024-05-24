/*
   Fundacion Kinal
   Centro educativo tecnico laboral Kinal
   Quinto perito
   Quinto electronica
   Codigo Tecnico: EB5AV 
   Curso: Taller de electronica digital y reparacion de computadoras I
   Dev: Sebastian Enrique Lemus Salvador
   Fecha: 24 de mayo de 2024
   
   link de la simulacion: https://www.tinkercad.com/things/3OA7WsNMbBh-parte-2-practica-6
   
*/

#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define direccion_lcd 0x27

LiquidCrystal_I2C LCD_SebasLem(direccion_lcd, 16, 2);

Servo servo_SebasLem;

int tono = 600;

const int filas = 4;
const int columnas = 4;
char keys[filas][columnas] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pinesFila[filas] = {11, 10, 9, 8};
byte pinesColumna[columnas] = {7, 6, 5, 4};

Keypad teclado = Keypad(makeKeymap(keys), pinesFila, pinesColumna, filas, columnas);

char teclaPrecionada;

// Clave para el teclado matricial 4x4 
char clave[9] = "ABCD1590";
char ingresaClave[9];
int inicio = 0;

// Clave para el monitor serial
String claveSerial = "ABCD1590";
String claveRecibida = "";

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(12, OUTPUT);
  servo_SebasLem.attach(3, 0.9, 2.1);
  servo_SebasLem.write(0);
  LCD_SebasLem.init();
  LCD_SebasLem.backlight();
  LCD_SebasLem.clear();
  LCD_SebasLem.setCursor(0, 0);
  LCD_SebasLem.print("-> Bienvenido <-");
  LCD_SebasLem.setCursor(1, 1);
  LCD_SebasLem.print("Puerta Cerrada");
  Serial.println("Bienvenido");
  Serial.println("Ingresa la clave: ");
}

void loop() {
  digitalWrite(15, HIGH);
  
  teclaPrecionada = teclado.getKey();
  
  if (teclaPrecionada) {
    ingresaClave[inicio] = teclaPrecionada;
    inicio++;
    Serial.print(teclaPrecionada);
    LCD_SebasLem.setCursor(1, 0);
    LCD_SebasLem.print("Clave:");
    LCD_SebasLem.setCursor(inicio + 1, 1);
    LCD_SebasLem.print("*");
  }
  if (inicio == 8) {
    verificarClaveTeclado();
  }
  
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c != '\n' && c != '\r') {
      claveRecibida += c;
      Serial.print(c);
    }
  }
  if (claveRecibida.length() == 8) {
    verificarClaveSerial();
  }
  
  if (digitalRead(2) == HIGH) {
    LCD_SebasLem.clear();
  }
}

void verificarClaveTeclado() {
  if (strcmp(clave, ingresaClave) == 0) {
    claveCorrecta();
  } else {
    claveIncorrecta();
  }
  Reinicio();
}

void verificarClaveSerial() {
  if (claveRecibida == claveSerial) {
    claveCorrecta();
  } else {
    claveIncorrecta();
  }
  Reinicio();
}

void claveCorrecta() {
  digitalWrite(17, HIGH);
  LCD_SebasLem.clear();
  LCD_SebasLem.setCursor(1, 0);
  LCD_SebasLem.print("Clave Correcta");
  LCD_SebasLem.setCursor(1, 1);
  LCD_SebasLem.print("Puerta Abierta");
  Serial.print("\n");
  Serial.println("Bienvenido");
  Serial.println("Puerta Abierta");
  tone(12, tono, 600);
  delay(650);
  noTone(12);
  tone(12, tono, 600);
  delay(650);
  noTone(12);
  servo_SebasLem.write(180);
  delay(2000);
}

void claveIncorrecta() {
  digitalWrite(16, HIGH);
  LCD_SebasLem.clear();
  LCD_SebasLem.setCursor(0, 0);
  LCD_SebasLem.print("Clave Incorrecta");
  LCD_SebasLem.setCursor(1, 1);
  LCD_SebasLem.print("Puerta Cerrada");
  Serial.print("\n");
  Serial.println("Clave Incorrecta");
  Serial.println("Puerta Cerrada");
  for (int i = 0; i < 5; i++) {
    tone(12, tono, 600);
    delay(650);
    noTone(12);
  }
  servo_SebasLem.write(0);
  delay(2000);
}

void Reinicio() {
  delay(3000);
  inicio = 0;
  claveRecibida = "";
  memset(ingresaClave, 0, sizeof(ingresaClave));
  digitalWrite(17, LOW);
  digitalWrite(16, LOW);
  servo_SebasLem.write(0);
  LCD_SebasLem.clear();
  LCD_SebasLem.setCursor(0, 0);
  LCD_SebasLem.print("-> Bienvenido <-");
  LCD_SebasLem.setCursor(1, 1);
  LCD_SebasLem.print("Puerta Cerrada");
  Serial.print("\n");
  Serial.println("Bienvenido");
  Serial.println("Ingresa la clave: ");
}
