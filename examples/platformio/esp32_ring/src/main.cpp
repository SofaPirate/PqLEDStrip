
// THIS EXAMPLE IS PSEUDOCODE AND DOES NOT COMPILE

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

TriangleWave rampWaveFast{0.05, 0}; // period of 50ms
TriangleWave rampWaveSlow{3.0, 0};
SineWave sineWaveSlow{3.0};

TimeField<NUM_LEDS> timeField{rampWaveFast.period()}; // 16 samples over a period of 50ms
TimeField<NUM_LEDS> rollingTimeField{2*rampWaveSlow.period()}; // 16 samples over a period of 3 seconds

Metronome stripMetronome{0.05};

Metronome modeMetronome{200.0};

int demoMode = 0;
int demoModeCount = 6;

void begin()
{
    rollingTimeField.roll();
}

void step()
{
    // CHANGE DEMO MODE AUTOMATICALLY
    if (modeMetronome)
    {
        demoMode = (demoMode + 1) % demoModeCount;
        // RESET BRIGHTNESS TO 1.0 (default)
        strip.setBrightness(1.0);
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

        rampWaveFast >> timeField;

        if (timeField.triggered())
        {
            strip.noPalette();
            strip.draw(timeField);
            rampWaveFast.phase(rampWaveSlow);
        }
    }
    else if (demoMode == 2)
    {
        rampWaveFast >> timeField;

        if (timeField.triggered())
        {
            strip.palette(RainbowColors_p);
            strip.setBrightness( sineWaveSlow );
            strip.draw(timeField);
            rampWaveFast.phase(0);
        }
    } else if (demoMode == 3)
    {
        rampWaveFast >> timeField;

        if (timeField.triggered())
        {
            strip.palette(customPalette_p);
            strip.draw(timeField);
            rampWaveFast.phase(sineWaveSlow);
        }
    } else if (demoMode == 4) {
        sineWaveSlow >> rollingTimeField;

       if (rollingTimeField.triggered()) {
            strip.noPalette();
            strip.draw(rollingTimeField);
       }
    } else if (demoMode == 5)
    {
        // WE ARE UPDATING THE STRIP EVERY 50 MILLISECONDS
        if (stripMetronome)
        {
            // SETTING PIXEL COLORS WITHOUT A MAP AND WITOUT A PALETTE :(
            for ( int i =0; i < strip.getCount() ; i++) {
                CRGB color = CRGB( floor(float(i)/float(strip.getCount())*255.0), 0, 0 );
                strip.setPixel(i,color);
            }
        }
    }
}