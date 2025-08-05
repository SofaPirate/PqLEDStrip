# PqLEDStrip
LED strip library for Plaquette.

> [!CAUTION]
> Work in progress — not ready for use.

## Basics

- `PqLEDStrip` use the `FastLED` library for LED control.
- Do not call `FastLED.show()` as you might be used to. `PqLEDStrip` manages this automagically for you.
- Try *not* to set the value of the pixels every iteration of the loop, but either with a `Plaquette TimeSlice Map` or on an interval set with a `Plaquette Metronome`. 

### Example declaration

`LEDStripWS281X<3, RGB, 16> myStrip()`

### Template Parameters

`LEDStripWS281X<PIN, ORDER, COUNT>`

| Parameter | Description |
|-:|-|
| `PIN`     | The microcontroller pin connected to the LED strip. |
| `ORDER`   | The color order (e.g., `GRB`, `RGB`, etc.). |
| `COUNT`   | The number of LEDs in the strip. |

### Constructors

#### `LEDStripWS281X()`
No palette is set initially. Default fallback is grayscale gradient.

#### `LEDStripWS281X(const palette &p, TBlendType blend = LINEARBLEND)`
Creates a strip and sets a palette.

- `palette` is a FastLED palette of type `CRGBPalette16`, `CRGBPalette32`, `CRGBPalette256`, `TProgmemRGBPalette16`,`CHSVPalette16`, `CHSVPalette32` or `CHSVPalette256`.
- `blend` is either `LINEARBLEND` or `NOBLEND`.

### Pixel  Management

#### `void draw(AbstractMap &map)`
Fills the entire strip using values from a Plaquette Map, rendered with the current palette.

#### `void setPixel(int index, CRGB color)`
Manually set the color of an individual pixel.

#### `size_t getCount()`
Get the count of the pixels.

#### `void setBrightness(float brightness)`
Set the strip brightness between 0.0 and 1.0.

### Palette Management

#### `void unsetPalette()`
Removes the currently assigned palette. Default fallback is grayscale gradient.

#### `void setPalette(const palette &p, TBlendType blend = LINEARBLEND)`
Set a palette.
- `palette` is a FastLED palette of type `CRGBPalette16`, `CRGBPalette32`, `CRGBPalette256`, `TProgmemRGBPalette16`,`CHSVPalette16`, `CHSVPalette32` or `CHSVPalette256`.
- `blend` is either `LINEARBLEND` or `NOBLEND`.


