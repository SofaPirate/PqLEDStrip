# PqLEDStrip
LED strip library for Plaquette.

> [!CAUTION]
> Work in progress — not ready for use.

## Basics

### Example declaration

`LEDStripWS281X<PIN, ORDER, COUNT> myStrip()`

### Template Parameters

`LEDStripWS281X<PIN, ORDER, COUNT>`

| Parameter | Description |
|-:|-|
| `PIN`     | The microcontroller pin connected to the LED strip. |
| `ORDER`   | The color order (e.g., `GRB`, `RGB`, etc.). |
| `COUNT`   | The number of LEDs in the strip. |

### Default constructor

#### `LEDStripWS281X()`
Default constructor. No palette is set initially: values constrained between 0.0 and 1.0 will be mapped to grey colors.

### Pixel  Management

#### `void draw(AbstractMap &map)`
Fills the entire strip using values from a Plaquette Map, rendered with the current palette.

#### `void setPixel(int index, CRGB color)`
Manually set the color of an individual pixel.


## Advanced

### Constructors with palette assignement

- Blending is either `LINEARBLEND` or `NOBLEND`.

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


### Palette Management

- Blending is either `LINEARBLEND` or `NOBLEND`.

#### `void unsetPalette()`
Removes the currently assigned palette. Default fallback is grayscale gradient.

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









