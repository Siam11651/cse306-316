#include <SPI.h>
#include <SoftwareSerial.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <EEPROM.h>

#define GRAVITY
#define GRAVITY_THRESHOLD 9

bool fallState = false;
int fallCount = 0;

Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;
int acceleration;
float gyrox,gyroy,gyroz;
float accx,accy,accz;
// FILE file;
const int numberStartAddress = 0;  //starting address of eeprom
int time_count;
//bluetooh items
bool bt_flag;
String command = "";
String state = "";
bool onInput;
String tempnum="01521564856";
bool buzzflag;
bool helpflag=false;



SoftwareSerial dSerial(2, 3);
SoftwareSerial sSerial(11, 12);

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
void set_num()
{
  for(int i=0;i<11;i++)
  {
    Serial.println(tempnum[i]);
    EEPROM.write(numberStartAddress + i, tempnum[i]);
  }
  sSerial.listen();
  sSerial.write("2");
}
void read_num()
{
    tempnum = "";
    for (int offset = 0; offset < 11; offset++) {

      tempnum += (char)EEPROM.read(numberStartAddress + offset);
      //Serial.println((char)EEPROM.read(numberStartAddress + offset));
      //delay(1000);
    }
}

void send_num()
{
  char temp[13];
  temp[0]='s';
  for(int i=1;i<12;i++)
  {
    temp[i]=tempnum[i-1];
  }
  temp[12]='x';
  char s[8];
  for(int i=0;i<8;i++)
  {
    s[i]=temp[i];
  }
  dSerial.listen();
  dSerial.write(s,8);
  char s1[8];
  for(int i=8;i<13;i++)
  {
    s1[i-8]=temp[i];
  }
  dSerial.listen();
  dSerial.write(s1,5);
  delay(100);
}
void setup() {

  Serial.begin(115200);
  dSerial.begin(19200);
  sSerial.begin(9600);
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
  send_num();
  sSerial.listen();
 
}

void loop() {
  int xAxisInput = analogRead(X_AXIS_PIN);
  int yAxisInput = analogRead(Y_AXIS_PIN);
  int joystickinput = digitalRead(SW_PIN);
  mpu.getEvent(&a, &g, &temp);
  read_mpu();
// Serial.print(accx);
// Serial.print(",");
// Serial.print(accy);
// Serial.print(",");
// Serial.print(accz);
// Serial.print(",");

  // Serial.println("z = " + String(accz));
  // Serial.println("side = " + String(accx * accx + accy * accy));

  if(accz < 0 || accz < 3 && accx * accx + accy * accy > GRAVITY_THRESHOLD * GRAVITY_THRESHOLD)
  {
    fallState = true;
  }
  else
  {
    fallState = false;
  }

  // Serial.print((int)fallState);
  // Serial.print(",");

  if(fallState)
  {
    ++fallCount;
  }
  else
  {
    fallCount = 0;
  }

  if(fallCount > 100)
  {
    buzzflag = true;
    fallCount = 0;
    time_count=0;
  }
  if(buzzflag==true && time_count>200)
    {
      if(helpflag==false)
      {
      dSerial.listen();
      dSerial.write("h",1);
      sSerial.listen();
      helpflag=true;
      }
    }
  
  if(joystickinput==LOW || state =="o")
  {
    if(buzzflag==false)
    {
      if(helpflag==false)
      {
      buzzflag=true;
      dSerial.listen();
      dSerial.write("h",1);
      delay(1000);
      sSerial.listen();
      helpflag=true;
      }
      
    }
    else
    {
      
      if(helpflag ==  true)
      {
      buzzflag=false;
      dSerial.listen();
      dSerial.write("i",1);
      delay(1000);
      sSerial.listen();
      helpflag=false;
      }
    }
    state ="";
  }
  if(buzzflag==true)
  {
    digitalWrite(BUZZER_OUT, HIGH);
  }
  else
  {
    digitalWrite(BUZZER_OUT, LOW);
  }
  for (int i = 0; i < sSerial.available(); i++) {

    char c = sSerial.read();
   Serial.println((char)c);
    onInput = true;
    if (c != '\\') {
      if ('a' <= c && c <= 'z' || c == '$' || c == '%') {
        command += c;
        if(c == 's')
        {
          tempnum="";
        }
        else if(c == 'o')
        {
          state = "o";
        }
      }
      else if('0' <= c && c <= '9' && state == "s")
      {
        tempnum += c;
        Serial.println(tempnum);
      }
    } else {
      if (command == "$") {
        state = "";
      }
      else if(command == "%")
      {
        state = "";
        Serial.println(tempnum);
        set_num();
        dSerial.listen();
        send_num();
        sSerial.listen();
             }
      else {
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

  if (!onInput) {
    command = "";
  }
  time_count++;
 
}
