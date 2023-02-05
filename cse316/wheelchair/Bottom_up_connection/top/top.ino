#include<SoftwareSerial.h>
#include<EEPROM.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
String num="01521564856"; //Initialized variable to store receive
String state;
const int numberStartAddress=0;
SoftwareSerial dSerial(3, 2);
void set_num()
{
    for (int offset = 0; offset < 11; offset++) {
      EEPROM.write(numberStartAddress + offset, num[offset]);
    }
}
void read_num()
{
    num = "";
    for (int offset = 0; offset < 11; offset++) {
      num += EEPROM.read(numberStartAddress + offset);
    }
}
void setup() {
 Serial.begin(38400); 
 dSerial.begin(19200); // Arduino Connection Band
 lcd.begin();
 lcd.clear();         
 lcd.backlight();      // Make sure backlight is on
 // Print a message on both lines of the LCD.
 
}
void loop() {
  for(int i=0;i<dSerial.available();i++)
  {
    char x =  dSerial.read();
    if(x == 's')
    {
      state = "s";
      num="";
    }
    else if( x == 'x')
    {
      state = "x";
     // Serial.println(num);
      set_num();
    }
    else if(x == 'h')
    {
      state = "h";
    }
    if(x!='s' && state == "s")
    {
      num += x;
    }
  } 
  lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
  lcd.print("Num:");
  lcd.setCursor(4,0);
  lcd.print(num);
  
  lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
  lcd.print("Add Something");
}