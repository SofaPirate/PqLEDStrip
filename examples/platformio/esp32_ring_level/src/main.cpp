
// THIS EXAMPLE IS PSEUDOCODE AND DOES NOT COMPILE

#include <Plaquette.h>
#include <FastLED.h>

#include <PqLEDStrip.h>

// The LED strip.
LEDStripWS281X<4, GRB, 16> strip{}; // <PIN RGB_ORDER COUNT>

// Level field.
LevelField levelField;

// Sensor (potentiometer).
AnalogIn sensor(34, INVERTED);

// Wave.
Wave wave(SINE);

// Potentiometers.
AnalogIn potRampWidth(35);
AnalogIn potRampShift(32);

// Switch for falling vs rising.
DigitalIn switchRising(12, INTERNAL_PULLUP);

// Switch for direct vs wave.
DigitalIn switchWave(14, INTERNAL_PULLUP);

// Metronome updating the strip.
Metronome stripMetronome{0.05};

void begin()
{
    // Set palette. Use NOBLEND to prevent wrap-around effect.
    strip.palette(HeatColors_p, NOBLEND);
}

void step()
{
    // Send sensor value to level field.
    if (switchWave) {
        // Use wave modulated with sensor.
        wave.period(sensor.mapTo(10, 1));
        wave >> levelField;
    }
    else {
        // Use sensor directly.
        sensor >> levelField;
    }

    if (stripMetronome)
    {
        // Update level field parameters.
        levelField.rampWidth(potRampWidth.mapTo(0, 0.5));
        levelField.rampShift(potRampShift);
        levelField.setRising(switchRising);

        // Draw field on the strip.
        strip.draw(levelField);
   }
}