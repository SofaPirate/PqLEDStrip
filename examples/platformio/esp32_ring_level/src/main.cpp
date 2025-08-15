
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
AnalogIn potRampWidth(35);
AnalogIn potRampShift(32);

// Switch for falling vs rising.
DigitalIn switchRising(12, INTERNAL_PULLUP);

// Metronome updating the strip.
Metronome stripMetronome{0.05};

void begin()
{
}

void step()
{
    // Rescale sensor and send to level field.
    sensor >> sensorScaler >> levelField;

    if (stripMetronome)
    {
        // Update level field parameters.
        levelField.rampWidth(potRampWidth);
        levelField.rampShift(potRampShift);
        levelField.rising(switchRising);

//            strip.palette(CloudColors_p);
        strip.draw(levelField);
    }
}