#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>


LiquidCrystal_I2C lcd(0x3F, 16, 2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
String phoneNumber = "01557251225";  //Initialized phoneNumber with default
String state;

String lineToPrint = "Operating....    ";

const int numberStartAddress = 0;

bool started = false;
bool messageSent = false;

SoftwareSerial bottomArduinoCommSerial(11, 12); // (blue, violet)
SoftwareSerial simCommSerial(2, 3); // (brown, black)

void SendMessage(String number, String message) {
  simCommSerial.println("AT+CMGF=1");
  delay(1000);
  simCommSerial.println("AT+CMGS=\"" + number + "\"");
  delay(1000);
  simCommSerial.print(message);
  delay(1000);
  simCommSerial.write(26);
  delay(1000);
  started = false;
}

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.backlight();  // Make sure backlight is on

  Serial.begin(9600);
  simCommSerial.begin(9600);
  bottomArduinoCommSerial.begin(19200);
}

void loop() {

  for (int i = 0; i < bottomArduinoCommSerial.available(); i++) {
    char x = bottomArduinoCommSerial.read();
    Serial.println(x);

    if (x == 's') {  // ready to take 11 number from bottom into phoneNumber string
      state = "s";
      phoneNumber = "";
    } else if (x == 'x') {  // number is in phoneNumber String
      state = "x";
      Serial.println(phoneNumber);
    } else if (x == 'h') {  // user needs help state
      state = "h";
    } else if (x == 'i') {
      state = "i";
    }

    if (x != 's' && state == "s") { // saving number in phoneNumber string
      if (x >= '0' && x <= '9') {
        phoneNumber += x;
      }
    }
  }
  
  if (state == "h") { // message for help
    lineToPrint = "User Needs Help";
    
    if (!started && !messageSent) {
      simCommSerial.listen();
      SendMessage(phoneNumber, "Need Help");
      bottomArduinoCommSerial.listen();

      started = true;
      messageSent = true;
    }
  } else if (state == "i") {  
    lineToPrint = "Help Coming     ";
    messageSent = false;
  } else {
    lineToPrint = "Operating....     ";
  }
   
  
  lcd.setCursor(0, 0);  //Set cursor to character 2 on line 0
  lcd.print("Num:");
  lcd.setCursor(4, 0);
  lcd.print(phoneNumber);
  lcd.setCursor(0, 1);  //Move cursor to character 2 on line 1
  lcd.print(lineToPrint);
}
