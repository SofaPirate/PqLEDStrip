/**
 * arduino_ring
 *
 * Shows different ways to control an LED strip using PqLedStrip library.
 *
 * Created in 2025 by Sofian Audry
 *
 * This example code is in the public domain.
 */
#include <Plaquette.h>
#include <PqLEDStrip.h>

// Number of LEDs.
const int NUM_LEDS = 16;

// You can change this to true to enable the wave instead of potentiometer raw value.
const bool USE_WAVE = true;

// The LED strip.
LEDStripWS281X<6, GRB, NUM_LEDS> strip{}; // <PIN RGB_ORDER COUNT>

// Sensor (potentiometer).
AnalogIn pot(A0);

// Sine wave.
Wave wave(SINE, 2.0);

// Pushbutton for mode.
DigitalIn button(2, INTERNAL_PULLUP);

// Pivot field.
PivotField pivotField{};

// Time slice field over a time window of 2 seconds.
TimeSliceField<NUM_LEDS> timeSliceField(2.0);

// Metronome updating the strip.
Metronome stripMetronome(0.05);

// Animation modes.
enum AnimationMode {
    MODE_DEFAULT,
    MODE_PIVOT_RAMP,
    MODE_PIVOT_CENTER,
    MODE_PIVOT_BUMP,
    MODE_TIME_SLICE,
    N_MODES
};

AnimationMode currentMode = MODE_DEFAULT;

// Gradient palette.
// The 1st column specifies where along the palette's indicies (0-255), 
// where the gradient should be anchored. The other 3 columns are RGB values.
// See https://github.com/FastLED/FastLED/wiki/Gradient-color-palettes
DEFINE_GRADIENT_PALETTE( customGradient_gp ) {
  0, /* 0%   */     0,     0,     0,   // black
128, /* 50%  */   255,     0,     0,   // red
224, /* 88%  */   255,   255,     0,   // bright yellow
255, /* 100% */   255,   255,   255 }; // full white

CRGBPalette16 customGradient_p = customGradient_gp;

void startCurrentMode();

void begin()
{
    // Debounce button.
    button.debounce();
    startCurrentMode();
    timeSliceField.rolling();

    // Set palette.
    // List of palettes: https://fastled.io/docs/d3/d4f/group___predefined_palettes.html
    strip.palette(HeatColors_p, NOBLEND);

    // You can also define a custom palette using the DEFINE_GRADIENT_PALETTE macro (see above).
    //strip.palette(customGradient_p, LINEARBLEND_NOWRAP);
}

void step()
{
    // Change mode on button press.
    if (button.rose()) {
        currentMode = (AnimationMode)((currentMode + 1) % N_MODES);
        startCurrentMode();
    }

    // Manage wave.
    wave.period(pot.mapTo(10, 1));
    
    // Assign signal depending on using wave or not.
    float signal = USE_WAVE ? wave : pot;

    // Manage modes.

    // Direct signal to strip: controls brightness.
    if (currentMode == MODE_DEFAULT) {
        signal >> strip;
    }

    // Pivot field default mode.
    else if (currentMode == MODE_PIVOT_RAMP) {
        signal >> pivotField;

        if (stripMetronome) {
            strip.draw(pivotField);
        }
    }

    // Pivot field starting from center.
    else if (currentMode == MODE_PIVOT_CENTER) {
        pivotField.center(0.5); // (*) Try changing this to 0.25 or 1.0.

        signal >> pivotField;

        if (stripMetronome) {
            strip.draw(pivotField);
        }
    }

    // Pivot field bump mode.
    else if (currentMode == MODE_PIVOT_BUMP) {
        pivotField.mode(PIVOT_BUMP);

        signal >> pivotField;

        if (stripMetronome) {
            strip.draw(pivotField);
        }
    }

    // TIme slice mode.
    else if (currentMode == MODE_TIME_SLICE) {
        signal >> timeSliceField;

        if (timeSliceField.updated()) {
            strip.draw(timeSliceField, true);
        }
    }
}


void printCurrentMode() {
    switch (currentMode) {
        case MODE_DEFAULT:
            println("MODE_DEFAULT");
            break;
        case MODE_PIVOT_RAMP:
            println("MODE_PIVOT_RAMP");
            break;
        case MODE_PIVOT_CENTER:
            println("MODE_PIVOT_CENTER");
            break;
        case MODE_PIVOT_BUMP:
            println("MODE_PIVOT_BUMP");
            break;
        case MODE_TIME_SLICE:
            println("MODE_TIME_SLICE");
            break;
        default:;
    }
}

void startCurrentMode() {
    printCurrentMode();
    strip.brightness(1.0);
    pivotField.rampWidth(0.25);
    pivotField.rampShift(0.5);
    pivotField.bumpWidth(0.25);
    pivotField.center(0);    
}
