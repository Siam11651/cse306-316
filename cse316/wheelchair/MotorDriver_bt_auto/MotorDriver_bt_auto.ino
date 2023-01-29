#include<SPI.h>
#include<SoftwareSerial.h>

SoftwareSerial sSerial(11, 12);

bool onInput;

#define X_AXIS_PIN A1 // left-right
#define Y_AXIS_PIN A0 // front-back
#define SW_PIN 10

// motor left
#define ENABLE_LEFT 5 // enable motor A
#define IN_LEFT1 6 // motor A spin 1
#define IN_LEFT2 7 // motor A spin 2

// motor right
#define ENABLE_RIGHT 10 // enable motor B
#define IN_RIGHT1 8 // motor B spin 1
#define IN_RIGHT2 9 // motor B spin 2

// speed factor
#define FACTOR 0.3// average fraction of total voltage (PWM)

// loop delay
#define DELTA 10.0 // ms
#define HORIZONTAL_THRESHOLD 400

#define VERTICAL_THRESHOLD 1

int bt_flag=0;
char command;
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
  if(command == "forward")
  {
     Forward();       
  }
  else if(command == "backward")
  {
      Backward();
  }
  else if(command == "right")
  {
      Right();
  }
  else if(command == "left")
  {
      Left();
  }
  else
  {
    digitalWrite(IN_LEFT1, LOW);
    digitalWrite(IN_LEFT2, LOW);
    digitalWrite(IN_RIGHT1, LOW);
    digitalWrite(IN_RIGHT2, LOW);
    delay(DELTA);
  }
}

void setup()
{
  onInput = true;
  
    Serial.begin(9600);
    sSerial.begin(9600);
    pinMode(SW_PIN, INPUT);
    digitalWrite(SW_PIN, HIGH);
    pinMode(X_AXIS_PIN, INPUT);
    pinMode(Y_AXIS_PIN, INPUT);
    pinMode(ENABLE_LEFT, OUTPUT);
    pinMode(IN_LEFT1, OUTPUT);
    pinMode(IN_LEFT2, OUTPUT);
    pinMode(ENABLE_RIGHT, OUTPUT);
    pinMode(IN_RIGHT1, OUTPUT);
    pinMode(IN_RIGHT2, OUTPUT);
}
void loop()
{
  command = '$';
  int xAxisInput = analogRead(X_AXIS_PIN);
    int yAxisInput = analogRead(Y_AXIS_PIN);
    int joystickinput =  digitalRead(SW_PIN);
    
    if(sSerial.available())
    {      
      char c = sSerial.read();
      
      if(c != '\\')
      {
        if('a' <= c && c <= 'z')
        {
          command += c;
          onInput = true;
        }
      }
      else
      {
        // Serial.println("command = " + command);

        onInput = false;
      }
    }
    
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

    if(!onInput)
    {
      command = "";
    }
}
