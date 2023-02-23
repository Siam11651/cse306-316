#include <LiquidCrystal_I2C.h>
#include<SoftwareSerial.h>
#include<EEPROM.h>
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
String num = "Not Set Yet"; //Initialized variable to store receive
String tempnum;
String state;
String printline="Operating....    ";
int count=0;
const int numberStartAddress=0;
bool started = false;
int message_sent=0;
SoftwareSerial dSerial(11, 12);
SoftwareSerial sSerial(2, 3);
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
      num += (char)EEPROM.read(numberStartAddress + offset);
    }
}
void SendMessage(String number, String message)
{
  sSerial.println("AT+CMGF=1");
  delay(1000);
  sSerial.println("AT+CMGS=\""+ number +"\"");
  delay(1000);
  sSerial.print(message);
  delay(1000);
  sSerial.write(26);
  delay(1000);
  started = false;
}
void setup() {
  lcd.begin();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on
  // Print a message on both lines of the LCD.
  Serial.begin(9600);
  sSerial.begin(4800);
  dSerial.begin(19200);
  
  
}

void loop() {
  
  for(int i=0;i<dSerial.available();i++)
  {
    char x =  dSerial.read();
    Serial.println(x);
    if(x == 's')
    {
      state = "s";
      num="";
    }
    else if( x == 'x')
    {
      state = "x";
     Serial.println(num);
      set_num();
    }
    else if(x == 'h')
    {
      state = "h"; 
      printline="Need Help";
     if(!started && message_sent==0)
   {
     sSerial.listen();
     SendMessage(num, "Need Help     ");
     dSerial.listen();
     started = true;
     message_sent=1;
   }
    }
    else if(x == 'i')
    {
      state = "i";
    printline="Help Coming     ";
    count=0;
    }
    if(x!='s' && state == "s")
    {
      if(x>='0' && x<= '9')
      {
        num += x;
      }
      
    }
  }
   if(state == "h")
   {
     printline="Need Help   ";
  //    if(!started && message_sent==0)
  //  {
  //    sSerial.listen();
  //    SendMessage(num, "Need Help");
  //    dSerial.listen();
  //    started = true;
  //    message_sent=1;
  //  }
   }
  else if(state == "i")
  {
    printline="Help Coming   ";
    count++;
    message_sent = 0;
  }
  else
  {
    printline="Operating     ";
  }
  if(count>500)
  {
    state = "x";
  }
  // sSerial.listen();

  // if(sSerial.available())
  // {
  //   Serial.println("paise");
  //   Serial.println(sSerial.readString());
  // }

  // if(Serial.available())
  // {
  //   sSerial.write(Serial.read());
  // }
  // dSerial.listen();
   
  lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
  lcd.print("Num:");
  lcd.setCursor(4, 0);
  lcd.print(num);  
  lcd.setCursor(0,1);   //Move cursor to character 2 on line 1
  lcd.print(printline);
}
