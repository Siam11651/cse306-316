#define HEART_RATE_PIN A0

enum class PulseState
{
    PULSE_HIGH, PULSE_LOW, COMPLETE
};

class HeartRateMonitor
{
protected:
    unsigned int monitorTimeLimit;
    unsigned int signalThreshold;
    PulseState pulseState;
    unsigned int count;

    virtual void OnPulse()
    {
        ++count;
    }

    virtual void OnMonitorEnd()
    {
        Serial.println(count * (60000 / monitorTimeLimit));
    }

public:
    HeartRateMonitor(unsigned int monitorTimeLimit, unsigned int signalThreshold)
    {
        pulseState = PulseState::PULSE_LOW;
        this->monitorTimeLimit = monitorTimeLimit;
        this->signalThreshold = signalThreshold;

        Reset();
    }

    virtual void Reset()
    {
        count = 0;
    }

    virtual void Update(unsigned int time, unsigned int signal)
    {
        Serial.println(signal);

        if(pulseState != PulseState::COMPLETE)
        {
            if(time < monitorTimeLimit)
            {
                if(pulseState == PulseState::PULSE_LOW)
                {
                    if(signal > signalThreshold)
                    {
                        pulseState = PulseState::PULSE_HIGH;
                        
                        OnPulse();
                    }
                }
                else
                {
                    if(signal <= signalThreshold)
                    {
                        pulseState = PulseState::PULSE_LOW;
                    }
                }
            }
            else
            {
                pulseState = PulseState::COMPLETE;

                OnMonitorEnd();
            }
        }
    }
};

HeartRateMonitor *heartRateMonitor;

void setup()
{
	Serial.begin(9600);

    heartRateMonitor = new HeartRateMonitor(60000, 550);
}

void loop()
{
	heartRateMonitor->Update(millis(), analogRead(HEART_RATE_PIN));
    delay(20);
}
