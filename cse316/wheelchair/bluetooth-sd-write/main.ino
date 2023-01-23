#include <SPI.h>
#include <SD.h>

File file;

String command;
size_t index;

void setup()
{
	Serial.begin(9600);

    command = "";
    index = 0;

    if(!SD.begin())
    {
        Serial.println("Error: SD not initialized");
    }

    file = SD.open("number.txt", FILE_WRITE);
}

void loop()
{
	if(Serial.available())
    {
        command += Serial.readString();
    }

    if(command != "")
    {
        if(file)
        {
            file.println(command);
            file.close();
        }

        command = "";
    }
}