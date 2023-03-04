#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   
#include <LiquidCrystal_I2C.h>
#include<SoftwareSerial.h>
#include<EEPROM.h>
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
String num = "Not Set Yet"; //Initialized variable to store receive
String tempnum;
String state;
String printline="Operating....    ";
String prebpm = "BPM:";
int count=0;
int start_count=0;
const int numberStartAddress=0;
bool started = false;
int message_sent=0;
//SoftwareSerial dSerial(4, 5);
SoftwareSerial sSerial(2, 3);
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED = LED_BUILTIN;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 
                               
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

int mybpm;
bool panicstate=false;
int panic_count=0;

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
  lcd.backlight();
  // Make sure backlight is on
  // Print a message on both lines of the LCD.
  Serial.begin(9600);
  sSerial.begin(4800);
  //dSerial.begin(9600);
//  read_num();  
  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
}

void loop() {
  if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
mybpm = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 
//  Serial.println(A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
if (pulseSensor.sawStartOfBeat()) {            // Constantly test to see if "a beat happened".
mybpm = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 
//  Serial.println(A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
}

  delay(20);                    // considered best practice in a simple sketch.
mybpm = mybpm*3;
}
                      // Print the value inside of myBPM. 

  for(int i=0;i<Serial.available();i++)
  {
    char x =  Serial.read();
    // Serial.println(x);
    if(x == 's')
    {
      state = "s";
      num="";
    }
    else if( x == 'x')
    {
      state = "x";
     //Serial.println(num);
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
     //dSerial.listen();
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
  start_count++;
  if(mybpm < 75 )
  {
    panicstate=true;
    panic_count++;
  }
  else
  {
    panicstate=false;
    panic_count=0;
  }
  if(panicstate==true && panic_count > 200 )
  {
    state = "h"; 
      printline="Need Help";
     if(!started && message_sent==0)
   {
     sSerial.listen();
     SendMessage(num, "Need Help     ");
     //dSerial.listen();
     started = true;
     message_sent=1;
   }  }
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
  lcd.setCursor(10, 1);
  lcd.print(prebpm);
  lcd.setCursor(14,1);
  lcd.print(mybpm);
}
