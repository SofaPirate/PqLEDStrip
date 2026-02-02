# PqLEDStrip
LED strip library for [Plaquette](https://github.com/SofaPirate/Plaquette).

## Introduction

- `PqLEDStrip` uses the `FastLED` library for LED control. It was initially developed with `FastLED` 3.10.1.
- Do *not* call `FastLED.show()` as you might be used to! `Plaquette` and `PqLEDStrip` manage the *showing* of LEDs automagically for you.
- Do *not* to set the value of the LEDs every iteration of your update loop because you want your update loop to run as fast as possible and only update the LEDs about every 20 to 50 milliseconds. Use a `Plaquette field` (eg. `PivotField`, `TimeSliceField`) or on an interval set with a `Plaquette` `Metronome`. See best practices at the bottom of this document.

## Strip type support

- Currently, only the WS281X type of strip is supported.

## Example Declaration

`LEDStripWS281X<3, RGB, 16> myStrip`

## Template Parameters

`LEDStripWS281X<PIN, ORDER, COUNT>`

| Parameter | Description |
|-:|-|
| `PIN`     | The microcontroller pin connected to the LED strip. |
| `ORDER`   | The color order (e.g., `GRB`, `RGB`, etc.). |
| `COUNT`   | The number of LEDs in the strip. |

## Constructors

### `LEDStripWS281X()`
No palette is set initially. Default fallback is grayscale gradient.

### `LEDStripWS281X(const palette &p, TBlendType blend = LINEARBLEND)`
Creates a strip and sets a palette.

- `palette` is a FastLED palette of type `CRGBPalette16`, `CRGBPalette32`, `CRGBPalette256`, `TProgmemRGBPalette16`,`CHSVPalette16`, `CHSVPalette32` or `CHSVPalette256`.
- `blend` is either `LINEARBLEND`, `LINEARBLEND_NOWRAP`, or `NOBLEND`.

## Pixel Management

### `void draw(AbstractField& field)`
Fills the entire strip using values from a `Plaquette` field, rendered with the current palette. See below on how to use with a `TimeSliceField`.

### `void setPixel(int index, CRGB color)`
Manually set the color of an individual pixel.

### `size_t nPixels()`
Get the count of the pixels.

### `void brightness(float brightness)`
Set the strip brightness between 0.0 and 1.0.

## Palette Management

### `void noPalette()`
Removes the currently assigned palette. Default fallback is grayscale gradient.

### `void palette(const palette &p, TBlendType blend = LINEARBLEND)`
Set a palette.
- `palette` is a FastLED palette of type `CRGBPalette16`, `CRGBPalette32`, `CRGBPalette256`, `TProgmemRGBPalette16`,`CHSVPalette16`, `CHSVPalette32` or `CHSVPalette256`.
- `blend` is either `LINEARBLEND`, `LINEARBLEND_NOWRAP`, or `NOBLEND`.

## Best practices
Usually, your *update loop* is running very fast (more than 1000 times per second). Now, a LED strip does not need to (and probably cannot) be updated so fast. So you **must update the LED strip at a slower interval**. Here are two techniques: one with a `Plaquette Metronome` and another with a Plaquette `TimeSliceField`.

### With a `Metronome`

In *global space*, declare a Metronome with a period of 50 milliseconds (for a refresh rate of 20 Hz):
```cpp
Metronome stripUpdateMetronome{0.05};
```

In your *update loop*, check if the `Metronome` has triggered and update the `strip`:
```cpp
if (stripUpdateMetronome)
{
    // Update the strip here
}
```

```text
loop():      [x][x][x][x][x][x][x][x][x][x]...
             Check if metronome ticked
Time (ms):    0           50         100         150
Metronome:   [✓]         [✓]         [✓]         [✓]
              ^           ^           ^           ^
              LED update  LED update  LED update  LED update
```

### Draw a `TimeSliceField`

A `TimeSliceField` allows you to establish a "time bridge" between the fast `loop()` and the slow update speed of the LED strip.

A `TimeSliceField` not only collects data over time, but also spreads the processing across multiple `loop()` iterations.

This avoids a performance spike from computing all samples in a single burst right before updating the strip. Instead, a small amount of work is done each loop, keeping performance smooth and consistent.

In *global space*, declare a `TimeSliceField` that contains 16 samples (the number of samples does **not** have to match the number of pixels) that will be taken over a period of 50 milliseconds (for a refresh rate of 20 Hz):
```cpp
TimeSliceField<16> timeSliceField{0.05}; // 16 samples over a period of 50 ms
```

In your `loop()`, feed any float-based data source (such as a Wave, Ramp, or `analogIn`) into the `TimeSliceField`. Then, check if the `TimeSliceField` has triggered (i.e., its time period has elapsed), and instruct the strip to draw using the collected samples:
```cpp
sineWave >> timeSliceField;

if (timeSliceField.updated()) // 50 milliseconds have passed
{
    strip.draw(timeSliceField); // Draw the 16 samples onto the strip using the current palette
}
```

The `TimeSliceField` collects 16 samples over a 50 ms period (e.g., from a wave or analog input). After 50 ms, it triggers once and uses the collected samples to update the LED strip.

To capture a full cycle of a waveform (like a Wave), the waveform's period should match the `TimeSliceField`'s period — in this case, 50 ms.

```text
loop():      [ x ][ x ][ x ][ x ][ x ][ x ][ x ][ x ][ x ][ x ]...
Time (ms):                                                 50  ...
SineWave:    [0.3][0.2][0.1][0.0][0.1][0.2][0.3][0.4][0.5][0.6]...   
                                                            |
                                                TimeSliceField triggers here (50 ms have passed)
LED draw:                                       Draw the collected data to the strip 
```

The collected data is mapped to each pixel in order (the number of samples does **not** have to match the number of puxels) . The values are mapped to `CRGB` colors through a palette:
```text
Pixel index :      0    1    2    3    4    5    6    7    8    9  ...
Collected data : [0.3][0.2][0.1][0.0][0.1][0.2][0.3][0.4][0.5][0.6]...
Palette lookup:  CRGB CRGB CRGB CRGB CRGB CRGB CRGB CRGB CRGB CRGB ...
```

