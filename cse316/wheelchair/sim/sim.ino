#include <SoftwareSerial.h>

SoftwareSerial sSerial(2, 3);
// (brown, black)

void setup() 
{
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  sSerial.begin(9600);
}

int state = 0;

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
}

bool started = false;

void loop()
{
  if(!started)
  {
    SendMessage("01721869544", "Lol");

    started = true;
  }

  if(sSerial.available())
  {
    Serial.println("paise");
    Serial.println(sSerial.readString());
  }

  if(Serial.available())
  {
    sSerial.write(Serial.read());
  }
}
