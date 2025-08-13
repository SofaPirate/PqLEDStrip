
// THIS EXAMPLE IS PSEUDOCODE AND DOES NOT COMPILE

#include <Plaquette.h>
#include <FastLED.h>

#include <PqLEDStrip.h>

// The LED strip.
LEDStripWS281X<4, GRB, 16> strip{}; // <PIN RGB_ORDER COUNT>

// Level field.
LevelField levelField;

// Sensor (temperature).
AnalogIn sensor(34, INVERTED);

// Re-scaler (so that values span complete range).
MinMaxScaler sensorScaler;

// Potentiometers.
AnalogIn potLeftSkew(35);
AnalogIn potRightSkew(32);

// Switch for falling vs rising.
DigitalIn switchRising(12, INTERNAL_PULLUP);

// Metronome updating the strip.
Metronome stripMetronome{0.05};

void begin()
{
}

void step()
{
    sensor >> sensorScaler >> levelField;

    if (stripMetronome)
    {
        // Update level field parameters.
        levelField.leftSkew(potLeftSkew);
        levelField.rightSkew(potRightSkew);
        if (switchRising) levelField.rising();
        else levelField.falling();

//            strip.setPalette(CloudColors_p);
        strip.draw(levelField);
    }
}