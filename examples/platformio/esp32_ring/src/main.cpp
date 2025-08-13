
// THIS EXAMPLE IS PSEUDOCODE AND DOES NOT COMPILE

#include <Plaquette.h>
#include <FastLED.h>

#include <PqLEDStrip.h>

LEDStripWS281X<4, GRB, 16> strip{}; // <PIN RGB_ORDER COUNT>

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

TimeField<16> timeField{0.05}; // 16 samples over a period of 50ms

Metronome stripMetronome{0.05};

Metronome modeMetronome{5.0};

int demoMode = 0;
int demoModeCount = 5;

void begin()
{
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
            strip.setPalette(CloudColors_p);
            sineWaveSlow >> strip;
        }
    }
    else if (demoMode == 1)
    {

        rampWaveFast >> timeField;

        if (timeField.triggered())
        {
            strip.unsetPalette();
            strip.draw(timeField);
            rampWaveFast.phase(rampWaveSlow);
        }
    }
    else if (demoMode == 2)
    {
        rampWaveFast >> timeField;

        if (timeField.triggered())
        {
            strip.setPalette(RainbowColors_p);
            strip.setBrightness( sineWaveSlow );
            strip.draw(timeField);
            rampWaveFast.phase(0);
        }
    } else if (demoMode == 3)
    {
        rampWaveFast >> timeField;

        if (timeField.triggered())
        {
            strip.setPalette(customPalette_p);
            strip.draw(timeField);
            rampWaveFast.phase(sineWaveSlow);
        }
    } else if (demoMode == 4)
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