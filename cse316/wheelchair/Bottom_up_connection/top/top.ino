#include<SoftwareSerial.h>
#include<EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
String num; //Initialized variable to store receive
String tempnum;
String state;
const int numberStartAddress=0;
bool numinputstart;
SoftwareSerial dSerial(11, 12);

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {10, 9, 8, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 5, 4}; //connect to the column pinouts of the keypad

//Create an object of keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
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
bool number_validation()
{
  if(tempnum.length()!=11)
  {
    return false;
  }
  for(int i=0;i<11;i++)
  {
    if(tempnum[i]=='#' || tempnum[i] == '*')
    {
      return false;
    }
  }
  return true;
  
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
  char key = keypad.getKey();// Read the key
  if (key){
    if(key == '*')
    {
      if(numinputstart==false)
      {
         numinputstart=true;
         tempnum = "";    
      }
     
    }
    else if(key == '#')
    {
      if(numinputstart ==  true)
      {
         numinputstart=false;
      if(number_validation())
      {
        num=tempnum;
      }    
      }
    }
    else
    {
      if(numinputstart ==  true)
      {
        tempnum+=key;
      }
    }
  }
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
