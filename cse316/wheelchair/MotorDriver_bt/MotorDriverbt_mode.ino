#include<SPI.h>
#include<SoftwareSerial.h>

SoftwareSerial sSerial(11, 12);

#define X_AXIS_PIN A1 // left-right
#define Y_AXIS_PIN A0 // front-back
#define SW_PIN 10

// motor left
#define IN_LEFT1 6 // motor A spin 1
#define IN_LEFT2 7 // motor A spin 2

// motor right
#define IN_RIGHT1 8 // motor B spin 1
#define IN_RIGHT2 9 // motor B spin 2

// speed factor
#define FACTOR 0.3// average fraction of total voltage (PWM)

// loop delay
#define DELTA 10.0 // ms
#define HORIZONTAL_THRESHOLD 400
#define VERTICAL_THRESHOLD 100

//bluetooh items

bool bt_flag=true;
String command="";
String state = "";
bool onInput;

void Forward()
{
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

void Backward()
{
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

void Left()
{
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

void Right()
{
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

void Stop()
{
    digitalWrite(IN_LEFT1, LOW);
    digitalWrite(IN_LEFT2, LOW);
    digitalWrite(IN_RIGHT1, LOW);
    digitalWrite(IN_RIGHT2, LOW);
    delay(DELTA);
}

void setup()
{  
    Serial.begin(9600);
    sSerial.begin(9600);
    pinMode(SW_PIN, INPUT);
    digitalWrite(SW_PIN, HIGH);
    pinMode(X_AXIS_PIN, INPUT);
    pinMode(Y_AXIS_PIN, INPUT);
    pinMode(IN_LEFT1, OUTPUT);
    pinMode(IN_LEFT2, OUTPUT);
    pinMode(IN_RIGHT1, OUTPUT);
    pinMode(IN_RIGHT2, OUTPUT);
}

void loop()
{
    int xAxisInput = analogRead(X_AXIS_PIN);
    int yAxisInput = analogRead(Y_AXIS_PIN);
    int joystickinput =  digitalRead(SW_PIN);
    for(int i=0;i<sSerial.available();i++)
    {   
         
      char c = sSerial.read();
      onInput = true; 
      if(c != '\\')
      {
        if('a' <= c && c <= 'z' || c == '$')
        {
          command += c;
        }
      }
      else
      {
        if(command =="$")
        {
          state = "";
        }
        else
        {
          state = command;
        }
        onInput = false; 
        break;
      }
     
    }
   if(state == "x")
    {
      bt_flag=true;
    }
    else if(state == "y")
    {
      bt_flag=false;
    }
    
    if(bt_flag)
    {
      if(state== "f")
  {
     Forward();       
  }
  else if(state == "b")
  {
      Backward();
  }
  else if(state == "r")
  {
      Right();
  }
  else if(state == "l")
  {
      Left();
  }
    }
    else
    {
      if(VERTICAL_THRESHOLD < yAxisInput && yAxisInput < 1023 - VERTICAL_THRESHOLD) // if y axis not in threshold move left right
    {
        if(HORIZONTAL_THRESHOLD < xAxisInput && xAxisInput < 1023 - HORIZONTAL_THRESHOLD)
        {
            Stop();
        }
        else
        {
            if(xAxisInput < HORIZONTAL_THRESHOLD)
            {
                Left();
            }
            else
            {
                Right();
            }
        }
    }
    else
    {
        if(yAxisInput < VERTICAL_THRESHOLD)
        {
           
            Forward();
        }
        else
        {
 
            Backward();
        }
    }
    }
      if(!onInput)
    {
      command = "";
    }
    
}
