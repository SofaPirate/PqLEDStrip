# PqLEDStrip
LED strip library for Plaquette

> [!CAUTION]
> Work in progress — not ready for use.

## `LEDStripWS281X<PIN, ORDER, COUNT>` — Class Documentation

Class for controlling WS2812 (or compatible) LED strips using the FastLED library.  
Supports various palette types (RGB, HSV, PROGMEM) and smooth gradient blending.

### Template Parameters

| Parameter | Description |
|-:|-|
| `PIN`     | The microcontroller pin connected to the LED strip. |
| `ORDER`   | The color order (e.g., `GRB`, `RGB`, etc.). |
| `COUNT`   | The number of LEDs in the strip. |

- Blending is set with `TBlendType` (`LINEARBLEND` or `NOBLEND`).

### Constructors

#### `LEDStripWS281X()`
Default constructor. Initializes internal state. No palette is set initially.


#### `LEDStripWS281X(const CRGBPalette16 &p, TBlendType blend = LINEARBLEND)`
Creates a strip and sets a `CRGBPalette16` palette.



#### `LEDStripWS281X(const CRGBPalette32 &p, TBlendType blend = LINEARBLEND)`
Creates a strip and sets a `CRGBPalette32` palette.



#### `LEDStripWS281X(const CRGBPalette256 &p, TBlendType blend = LINEARBLEND)`
Creates a strip and sets a `CRGBPalette256` palette.



#### `LEDStripWS281X(const TProgmemRGBPalette16 &p, TBlendType blend = LINEARBLEND)`
Creates a strip and sets a palette stored in flash memory (PROGMEM), such as `RainbowColors_p`.



#### `LEDStripWS281X(const CHSVPalette16 &p, TBlendType blend = LINEARBLEND)`
Creates a strip and sets a `CHSVPalette16` (HSV) palette.



#### `LEDStripWS281X(const CHSVPalette32 &p, TBlendType blend = LINEARBLEND)`
Creates a strip and sets a `CHSVPalette32` palette.



#### `LEDStripWS281X(const CHSVPalette256 &p, TBlendType blend = LINEARBLEND)`
Creates a strip and sets a `CHSVPalette256` palette.

### Pixel  Management

#### `void setPixel(int index, CRGB color)`
Manually set the color of an individual pixel.


#### `void draw(AbstractMap &map)`
Fills the entire strip using values from a Plaquette Map, rendered with the current palette.

### Palette Management

#### `void setPalette(const CRGBPalette16 &p, TBlendType blend = LINEARBLEND)`
Set a `CRGBPalette16` palette.

#### `void setPalette(const CRGBPalette32 &p, TBlendType blend = LINEARBLEND)`
Set a `CRGBPalette32` palette.

#### `void setPalette(const CRGBPalette256 &p, TBlendType blend = LINEARBLEND)`
Set a `CRGBPalette256` palette.

#### `void setPalette(const TProgmemRGBPalette16 &p, TBlendType blend = LINEARBLEND)`
Set a flash-stored (PROGMEM) palette like `RainbowColors_p`.

#### `void setPalette(const CHSVPalette16 &p, TBlendType blend = LINEARBLEND)`
Set a `CHSVPalette16` HSV palette.

#### `void setPalette(const CHSVPalette32 &p, TBlendType blend = LINEARBLEND)`
Set a `CHSVPalette32` HSV palette.

#### `void setPalette(const CHSVPalette256 &p, TBlendType blend = LINEARBLEND)`
Set a `CHSVPalette256` HSV palette.

#### `void unsetPalette()`
Removes the currently assigned palette. Default fallback is grayscale gradient.







