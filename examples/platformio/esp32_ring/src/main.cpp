/**
 * esp32_ring_time_slice
 *
 * Shows different ways to control an LED strip using PqLedStrip library and TimeSliceField.
 *
 * Created in 2025 by Thomas O. Fredericks
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>
#include <FastLED.h>

#include <PqLEDStrip.h>

constexpr int NUM_LEDS = 16;
LEDStripWS281X<4, GRB, NUM_LEDS> strip{}; // <PIN RGB_ORDER COUNT>

const TProgmemPalette16 customPalette_p FL_PROGMEM =
    {
        CRGB::Red,
        CRGB::White,
        CRGB::Blue,
        CRGB::Black,

        CRGB::Red,
        CRGB::Gray,
        CRGB::Blue,
        CRGB::Black,

        CRGB::Red,
        CRGB::Red,
        CRGB::Gray,
        CRGB::Gray,
        
        CRGB::Blue,
        CRGB::Blue,
        CRGB::Black,
        CRGB::Black};

TriangleWave rampWaveFast{0.05}; // period of 50ms
TriangleWave rampWaveSlow{2.0, 0};
SineWave sineWaveSlow{3.0};

TimeSliceField<NUM_LEDS> baseTimeSliceField{rampWaveFast.period()}; // 16 samples over a period of 50ms
TimeSliceField<NUM_LEDS> rollingTimeSliceField{2*rampWaveSlow.period()}; // 16 samples over a period of 3 seconds

Metronome stripMetronome{0.1};

Metronome modeMetronome{5.0};

int demoMode = 0;
int demoModeCount = 6;

void begin()
{
    rollingTimeSliceField.rolling();
}

void step()
{
    // CHANGE DEMO MODE AUTOMATICALLY
    if (modeMetronome)
    {
        demoMode = (demoMode + 1) % demoModeCount;
        // RESET BRIGHTNESS TO 1.0 (default)
        strip.brightness(1.0);
    }

    if (demoMode == 0)
    {
        // WE ARE UPDATING THE STRIP EVERY 50 MILLISECONDS
        if (stripMetronome)
        {
            strip.palette(CloudColors_p);
            sineWaveSlow >> strip;
        }
    }
    else if (demoMode == 1)
    {
        rampWaveFast >> baseTimeSliceField;

        if (baseTimeSliceField.updated())
        {
            strip.noPalette();
            strip.draw(baseTimeSliceField);
            rampWaveFast.phase(rampWaveSlow);
        }
    }
    else if (demoMode == 2)
    {
        rampWaveFast >> baseTimeSliceField;

        if (baseTimeSliceField.updated())
        {
            strip.palette(RainbowColors_p);
            strip.brightness(sineWaveSlow);
            strip.draw(baseTimeSliceField);
            rampWaveFast.phase(0);
        }
    } else if (demoMode == 3)
    {
        rampWaveFast >> baseTimeSliceField;

        if (baseTimeSliceField.updated())
        {
            strip.palette(customPalette_p);
            strip.draw(baseTimeSliceField);
            rampWaveFast.phase(sineWaveSlow);

        }
    } else if (demoMode == 4) {
        sineWaveSlow >> rollingTimeSliceField;

       if (rollingTimeSliceField.updated()) {
            strip.noPalette();
            strip.draw(rollingTimeSliceField);
       }
    } else if (demoMode == 5)
    {
        // WE ARE UPDATING THE STRIP EVERY 50 MILLISECONDS
        if (stripMetronome)
        {
            // SETTING PIXEL COLORS WITHOUT A MAP AND WITOUT A PALETTE :(
            for ( int i =0; i < strip.nPixels() ; i++) {
                CRGB color = CRGB( floor(float(i)/float(strip.nPixels())*255.0), 0, 0 );
                strip.setPixel(i,color);
            }
        }
    }
}