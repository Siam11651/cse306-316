#define X_AXIS_PIN A1 // left-right
#define Y_AXIS_PIN A0 // front-back

// motor left
#define ENABLE_LEFT 5 // enable motor A
#define IN_LEFT1 6 // motor A spin 1
#define IN_LEFT2 7 // motor A spin 2

// motor right
#define ENABLE_RIGHT 10 // enable motor B
#define IN_RIGHT1 8 // motor B spin 1
#define IN_RIGHT2 9 // motor B spin 2

// speed factor
#define FACTOR 1.0// average fraction of total voltage (PWM)

// loop delay
#define DELTA 10.0 // ms

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
	int xAxisInput = analogRead(X_AXIS_PIN);
    int yAxisInput = analogRead(Y_AXIS_PIN);
    int motorSpeedA = 0;
    int motorSpeedB = 0;

    if(470 < yAxisInput && yAxisInput < 550) // if y axis not in threshold move left right
    {
        if(470 < xAxisInput && xAxisInput < 550)
        {
            Stop();
        }
        else
        {
            if(xAxisInput < 470)
            {
                motorSpeedA = map(yAxisInput, 470, 0, 0, 255);
                motorSpeedB = map(yAxisInput, 470, 0, 0, 255);

                Left();
            }
            else
            {
                motorSpeedA = map(yAxisInput, 550, 1023, 0, 255);
                motorSpeedB = map(yAxisInput, 550, 1023, 0, 255);

                Right();
            }
        }
    }
    else
    {
        if(yAxisInput < 470)
        {
            motorSpeedA = map(yAxisInput, 470, 0, 0, 255);
            motorSpeedB = map(yAxisInput, 470, 0, 0, 255);

            Forward();
        }
        else
        {
            motorSpeedA = map(yAxisInput, 550, 1023, 0, 255);
            motorSpeedB = map(yAxisInput, 550, 1023, 0, 255);

            Backward();
        }
    }
}