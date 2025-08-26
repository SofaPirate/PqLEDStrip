/**
 * esp32_ring_pivot
 *
 * Shows different ways to control an LED strip using PqLedStrip library and PivotField.
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>
#include <FastLED.h>

#include <PqLEDStrip.h>

// The LED strip.
LEDStripWS281X<4, GRB, 16> strip{}; // <PIN RGB_ORDER COUNT>

// Pivot field.
PivotField pivotField;

// Sensor (potentiometer).
AnalogIn sensor(34);

// Wave.
Wave wave(SINE);

// Potentiometers.
AnalogIn potRampWidth(35); // controls ramp width
AnalogIn potRampShift(33); // controls ramp shift
AnalogIn potCenter(32); // controls center

// Switches for mode.
DigitalIn switchBump(14, INTERNAL_PULLUP);
DigitalIn switchRising(12, INTERNAL_PULLUP);

// Switch for direct vs wave.
DigitalIn switchWave(27, INTERNAL_PULLUP);

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
        wave >> pivotField;
    }
    else {
        // Use sensor directly.
        sensor >> pivotField;
    }

    // Metronome tick event.
    if (stripMetronome)
    {
        // Update level field parameters.
        pivotField.rampWidth(potRampWidth.mapTo(0, 0.5));
        pivotField.rampShift(potRampShift);
        pivotField.bumpWidth(0.25);

        pivotField.center(potCenter);
        if (switchBump)
            pivotField.mode(switchRising ? PIVOT_BUMP : PIVOT_NOTCH);
        else
            pivotField.mode(switchRising ? PIVOT_RISING : PIVOT_FALLING); // setRising(switchRising);

        // Draw field on the strip.
        strip.draw(pivotField);
   }
}