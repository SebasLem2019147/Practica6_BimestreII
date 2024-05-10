/*
   Fundacion Kinal
   Centro educativo tecnico laboral Kinal
   Quinto perito
   Quinto electronica
   Codigo Tecnico: EB5AV 
   Curso: Taller de electronica digital y reparacion de computadoras I
   Dev: Sebastian Enrique Lemus Salvador
   Fecha: 14 de mayo
   
   link de la simulacion: https://www.tinkercad.com/things/e82e3IKrRlG-parte-1-practica-6?sharecode=DfZ5b-3ysjrjt6RZIoZkL4T_uVxkn8wb3126YfmG0CM
   
*/

#include <LiquidCrystal.h> 

#define RS 6
#define E  7
#define D4 2
#define D5 3
#define D6 4
#define D7 5

LiquidCrystal LCD_SebasLem(RS, E, D4, D5, D6, D7);
3
void setup(){
  LCD_SebasLem.begin(16, 2);  
  LCD_SebasLem.setCursor(3, 0);           
  LCD_SebasLem.print("Sebastian");  
  LCD_SebasLem.setCursor(3, 1);        
  LCD_SebasLem.print("2019-147");
}

void loop(){
}
