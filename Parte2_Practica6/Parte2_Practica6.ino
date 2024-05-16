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

//clave para el teclado matricial 4x4 
char clave[9] = "ABCD1234";
char ingresaClave[9];
int inicio = 0;

//clave para el monitor serial
char claveSerial[9] = "ABCD1234";
char ingresaClaveSerial[9];
int inicioSerial = 0;

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
  LCD_SebasLem.print("1.Pulsa el boton");
  LCD_SebasLem.setCursor(0, 1);
  LCD_SebasLem.print("2.Ingresa clave");
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
    LCD_SebasLem.print(teclaPrecionada);
  }
  if (inicio == 8) {
    verificarClaveTeclado();
  }
  
  while (Serial.available() > 0) {
    char c = Serial.read();
    Serial.print(c);
    if (c != '\n' && c != '\r' && inicioSerial < 8) {
      ingresaClaveSerial[inicioSerial] = c;
      inicioSerial++;
      ingresaClaveSerial[inicioSerial] = '\0';
    }
  }
  if (inicioSerial == 8) {
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
}

void verificarClaveSerial() {
  if (strcmp(claveSerial, ingresaClaveSerial) == 0) {
    claveCorrecta();
  } else {
    claveIncorrecta();
  }
}

void claveCorrecta() {
  digitalWrite(17, HIGH);
  LCD_SebasLem.clear();
  LCD_SebasLem.setCursor(1, 0);
  LCD_SebasLem.print("Bienvenido");
  LCD_SebasLem.setCursor(1, 1);
  LCD_SebasLem.print("Puerta Abierta");
  Serial.print("\n");
  Serial.println("Bienvenido");
  Serial.println("Puerta Abierta");
  tone(12, tono, 500);
  delay(550);
  noTone(12);
  tone(12, tono, 500);
  delay(550);
  noTone(12);
  servo_SebasLem.write(180);
  delay(2000);
  Reinicio();
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
    tone(12, tono, 500);
    delay(550);
    noTone(12);
  }
  servo_SebasLem.write(0);
  delay(2000);
  Reinicio();
}

void Reinicio() {
  delay(3000);
  inicio = 0;
  digitalWrite(17, LOW);
  digitalWrite(16, LOW);
  servo_SebasLem.write(0);
  LCD_SebasLem.clear();
  LCD_SebasLem.setCursor(0, 0);
  LCD_SebasLem.print("1.Pulsa el boton");
  LCD_SebasLem.setCursor(0, 1);
  LCD_SebasLem.print("2.Ingresa clave");
  Serial.print("\n");
  Serial.println("Ingresa la clave: ");
}
