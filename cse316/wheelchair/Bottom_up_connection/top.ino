#include<SoftwareSerial.h>
String num; //Initialized variable to store receive
String state;

SoftwareSerial dSerial(3, 2);
void setup() {
 Serial.begin(38400); // Begin the Serial at 9600 Baud
 dSerial.begin(19200);
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
      Serial.println(num);
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