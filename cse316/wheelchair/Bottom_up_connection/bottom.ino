#include <SPI.h>
#include <SoftwareSerial.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <EEPROM.h>

Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;
int acceleration;
float gyrox,gyroy,gyroz;
float accx,accy,accz;
// FILE file;
const int numberStartAddress = 0;  //starting address of eeprom
int time_count;


SoftwareSerial sSerial(11, 12);
SoftwareSerial dSerial(2, 3);

#define X_AXIS_PIN A1  // left-right
#define Y_AXIS_PIN A0  // front-back
#define SW_PIN 10

// motor left
#define IN_LEFT1 6  // motor A spin 1
#define IN_LEFT2 7  // motor A spin 2

// motor right
#define IN_RIGHT1 8  // motor B spin 1
#define IN_RIGHT2 9  // motor B spin 2

// speed factor
#define FACTOR 0.3  // average fraction of total voltage (PWM)

// buzzer
#define BUZZER_OUT 5

//mode led
#define BT_MODE_LED 4

// loop delay
#define DELTA 10.0  // ms
#define HORIZONTAL_THRESHOLD 400
#define VERTICAL_THRESHOLD 100

//bluetooh items
bool bt_flag;
String command = "";
String state = "";
bool onInput;
String tempnum;
bool buzzflag;
bool numsetflag;
void Forward() {
  digitalWrite(IN_LEFT1, HIGH);
  digitalWrite(IN_LEFT2, LOW);
  digitalWrite(IN_RIGHT1, LOW);
  digitalWrite(IN_RIGHT2, HIGH);
  delay(DELTA * FACTOR);
  digitalWrite(IN_LEFT1, LOW);
  digitalWrite(IN_LEFT2, LOW);
  digitalWrite(IN_RIGHT1, LOW);
  digitalWrite(IN_RIGHT2, LOW);
  delay(DELTA * (1.0 - FACTOR));
}

void Backward() {
  digitalWrite(IN_LEFT1, LOW);
  digitalWrite(IN_LEFT2, HIGH);
  digitalWrite(IN_RIGHT1, HIGH);
  digitalWrite(IN_RIGHT2, LOW);
  delay(DELTA * FACTOR);
  digitalWrite(IN_LEFT1, LOW);
  digitalWrite(IN_LEFT2, LOW);
  digitalWrite(IN_RIGHT1, LOW);
  digitalWrite(IN_RIGHT2, LOW);
  delay(DELTA * (1.0 - FACTOR));
}

void Left() {
  digitalWrite(IN_LEFT1, LOW);
  digitalWrite(IN_LEFT2, HIGH);
  digitalWrite(IN_RIGHT1, LOW);
  digitalWrite(IN_RIGHT2, HIGH);
  delay(DELTA * FACTOR);
  digitalWrite(IN_LEFT1, LOW);
  digitalWrite(IN_LEFT2, LOW);
  digitalWrite(IN_RIGHT1, LOW);
  digitalWrite(IN_RIGHT2, LOW);
  delay(DELTA * (1.0 - FACTOR));
}

void Right() {
  digitalWrite(IN_LEFT1, HIGH);
  digitalWrite(IN_LEFT2, LOW);
  digitalWrite(IN_RIGHT1, HIGH);
  digitalWrite(IN_RIGHT2, LOW);
  delay(DELTA * FACTOR);
  digitalWrite(IN_LEFT1, LOW);
  digitalWrite(IN_LEFT2, LOW);
  digitalWrite(IN_RIGHT1, LOW);
  digitalWrite(IN_RIGHT2, LOW);
  delay(DELTA * (1.0 - FACTOR));
}

void Stop() {
  digitalWrite(IN_LEFT1, LOW);
  digitalWrite(IN_LEFT2, LOW);
  digitalWrite(IN_RIGHT1, LOW);
  digitalWrite(IN_RIGHT2, LOW);
  delay(DELTA);
}

float absolute(float x)
{
  if(x<0)
  {
    x=x*(-1);
    return x;
  }
  return x;
}
void read_mpu()
{
  accx=a.acceleration.x;
  accy=a.acceleration.y;
  accz=a.acceleration.z;
//  Serial.print(accx);
//  Serial.print(",");
//  Serial.print(accy);
//  Serial.print(",");
//  Serial.print(accx);
//  Serial.print(",");
//  acceleration = a.acceleration.x * a.acceleration.x + a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z;
////  Serial.print(acceleration);
//  Serial.print(",");
  gyrox=absolute(g.gyro.x);
  gyroy=absolute(g.gyro.y);
  gyroz=absolute(g.gyro.z);
//  Serial.print(gyrox);
//  Serial.print(",");
//  Serial.print(gyroy);
//  Serial.print(",");
//  Serial.print(gyroz);
//  Serial.print(",");
 /* if (acceleration > 130 || acceleration < 70) {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    
      }
  */
}
void read_num()
{
    tempnum = "";
    for (int offset = 0; offset < 11; offset++) {
      tempnum += EEPROM.read(numberStartAddress + offset);
      //Serial.print(tempnum[offset]);
      //delay(1000);
    }
}
void set_num()
{
    for (int offset = 0; offset < 11; offset++) {
      EEPROM.write(numberStartAddress + offset, offset);
    }
    numsetflag=true;
}
void send_num(int dest)
{ 
  char temp[13];
  temp[0]='s';
  for(int i=1;i<12;i++)
  {
    temp[i]=tempnum[i];
  }
  temp[12]='x';
  if(dest==0)
  {
    dSerial.write(temp,13);
  }
  else
  {
    sSerial.write(temp,13);
  }
}
void setup() {
  
  sSerial.begin(9600);
  Serial.begin(115200);
  dSerial.begin(19200);
  pinMode(SW_PIN, INPUT);
  digitalWrite(SW_PIN, HIGH);
  pinMode(BUZZER_OUT,OUTPUT);
  pinMode(BT_MODE_LED,OUTPUT);
  digitalWrite(BUZZER_OUT, LOW);
  pinMode(X_AXIS_PIN, INPUT);
  pinMode(Y_AXIS_PIN, INPUT);
  pinMode(IN_LEFT1, OUTPUT);
  pinMode(IN_LEFT2, OUTPUT);
  pinMode(IN_RIGHT1, OUTPUT);
  pinMode(IN_RIGHT2, OUTPUT);
  //set_num();

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  // Serial.println("MPU6050 Found!");

 // pinMode(LED_BUILTIN, OUTPUT);

  // set accelerometer range to +-8G
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  // set gyro range to +- 500 deg/s
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  // set filter bandwidth to 21 Hz
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  // if (!SD.begin()) {
  //   Serial.println("Error: SD not initialized");
  // }

  // file = SD.open("number.txt", FILE_WRITE);

  delay(100);
}

void loop() {
  mpu.getEvent(&a, &g, &temp);
  
  if(gyrox > 0.5 && gyroy > 0.5  && gyroz > 0.5)
  {
    buzzflag=true;
    time_count=0;
  }
  if(buzzflag==true)
  {
    digitalWrite(BUZZER_OUT, HIGH);
  }
  else
  {
    digitalWrite(BUZZER_OUT, LOW);
  }
  int xAxisInput = analogRead(X_AXIS_PIN);
  int yAxisInput = analogRead(Y_AXIS_PIN);
  int joystickinput = digitalRead(SW_PIN);
  if(buzzflag == true && joystickinput == LOW )
  {
    buzzflag=false; 
  }
  if(buzzflag==true && time_count>500)
  {
    dSerial.write("help",4); 
  }
  if(buzzflag == false && joystickinput == LOW)
  {
   dSerial.write("help",4);  
  }
  for (int i = 0; i < sSerial.available(); i++) {

    char c = sSerial.read();
    onInput = true;
    if (c != '\\') {
      if ('a' <= c && c <= 'z' || c == '$') {
        command += c;
      }
    } else {
      if (command == "$") {
        state = "";
      } else {
        state = command;
      }
      onInput = false;
      break;
    }
  }
  
  if (state == "x") { 
    bt_flag = true;
    digitalWrite(BT_MODE_LED,HIGH);
    state = "";
  } else if (state == "y") {
    bt_flag = false;
    digitalWrite(BT_MODE_LED,LOW);
    state="";
  } else if (state == "q") {
    //bt flag on== 1 or off == 0
    if (bt_flag) {
      sSerial.write("1");  
  
    } else {
      // sSerial.print("0");
      sSerial.write("0");
  
    }
    state = "";

  } else if (state == "s") {  // checking if state(=command) 's', what follows shall be the number
    //string input and parsing job here,
    //assuming phone number in String number, size == 11
     tempnum= "";
    for (int offset = 0;offset < sSerial.available(); offset++) {
      char digit = sSerial.read();
    if (digit != '\\') {
      if ('0' <= digit && digit <= '9') {
         tempnum += digit;
         EEPROM.write(numberStartAddress + offset, (int)digit);
      }
    } else {
        state = "";
        numsetflag=true;
        break;
      }
    }
     
    }
 else if (state == "k") {  // number read command
    read_num();
    //now number has the 11 digit number, write back to bluetooth module
      
    }

  if (bt_flag) {
    if (state == "f") {
      Forward();
    } else if (state == "b") {
      Backward();
    } else if (state == "r") {
      Right();
    } else if (state == "l") {
      Left();
    }
  } else {
    if (VERTICAL_THRESHOLD < yAxisInput && yAxisInput < 1023 - VERTICAL_THRESHOLD)  // if y axis not in threshold move left right
    {
      if (HORIZONTAL_THRESHOLD < xAxisInput && xAxisInput < 1023 - HORIZONTAL_THRESHOLD) {
        Stop();
      } else {
        if (xAxisInput < HORIZONTAL_THRESHOLD) {
          Left();
        } else {
          Right();
        }
      }
    } else {
      if (yAxisInput < VERTICAL_THRESHOLD) {

        Forward();
      } else {

        Backward();
      }
    }
  }
  if(numsetflag)
  {
    read_num();
    send_num(0);
    numsetflag=false;
  }
  if (!onInput) {
    command = "";
  }
  time_count++;
}
