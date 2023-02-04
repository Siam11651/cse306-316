#include<SoftwareSerial.h>
#include<EEPROM.h>
String num; //Initialized variable to store receive
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
 
}
