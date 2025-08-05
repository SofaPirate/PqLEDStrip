# PqLEDStrip
LED strip library for Plaquette.

> [!CAUTION]
> Work in progress — not ready for use.

## Introduction

- `PqLEDStrip` use the `FastLED` library for LED control.
- Do not call `FastLED.show()` as you might be used to. `PqLEDStrip` manages this automagically for you.
- Try *not* to set the value of the pixels every iteration of the loop, but either with a `Plaquette TimeSlice Map` or on an interval set with a `Plaquette Metronome`. 

## Example declaration

`LEDStripWS281X<3, RGB, 16> myStrip()`

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
- `blend` is either `LINEARBLEND` or `NOBLEND`.

## Pixel  Management

### `void draw(AbstractMap &map)`
Fills the entire strip using values from a Plaquette Map, rendered with the current palette. See below on how to use with a `TimeSlice.

### `void setPixel(int index, CRGB color)`
Manually set the color of an individual pixel.

### `size_t getCount()`
Get the count of the pixels.

### `void setBrightness(float brightness)`
Set the strip brightness between 0.0 and 1.0.

## Palette Management

### `void unsetPalette()`
Removes the currently assigned palette. Default fallback is grayscale gradient.

### `void setPalette(const palette &p, TBlendType blend = LINEARBLEND)`
Set a palette.
- `palette` is a FastLED palette of type `CRGBPalette16`, `CRGBPalette32`, `CRGBPalette256`, `TProgmemRGBPalette16`,`CHSVPalette16`, `CHSVPalette32` or `CHSVPalette256`.
- `blend` is either `LINEARBLEND` or `NOBLEND`.

## Best practices

Usually, your `loop()` is running very fast (more than 1000 times per second). Now, a LED Strip does not need to (and cannot) be updated so fast. So you **must update the LED Strip at a slower internal**. Here are two techniques. One with a `Plaquette Metronome` and another with a `Plaquette TimeMap`.

### With a `Metronome`

In *global space*, declare a Metronome with a periode of 50 milliseconds (for a refresh rate of 20 hz) :
```cpp
Metronome stripUpdateMetronome{0.05};
```

In your `loop()`, check if the `Metronome` has triggered and update the `Strip` :
```cpp
if (stripUpdateMetronome)
{
    // Update the Strip here
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

### Draw a `TimeMap`

A `TimeMap` allows you to establish a "time portal" between the fast `loop()` and the slow update speed of the LED Strip. 

In *global space*, declare a TimeMap that contains 16 samples that will be taken over a periode of 50 milliseconds (for a refresh rate of 20 hz) :
```cpp
TimeMap<16> timeMap{0.05}; // 16 samples over a period of 50ms
```


In your `loop()`, feed any float data (a Wave, a Ramp, analogIn, etc...) into the `TimeMap` and then check if the `TimeMap` has triggered (over the time of its period) and thell the `Strip` to draw the contents of the `TimeMap` :
```cpp
SineWave >> timeMap;

if (timeMap.triggered()) // 50 milliseconds have passed
{
    strip.draw(timeMap); // Draw the 16 samples onto the strip using the current palette
}
```
This TimeMap accumulates 16 samples (e.g., from a wave or analog input) over 50 ms. Then it triggers once every 50 ms to update the LED strip using all the collected data.

```text
loop():      [ x ][ x ][ x ][ x ][ x ][ x ][ x ][ x ][ x ][ x ]...
Time (ms):                                                 50
Sample Wave: [0.3][0.2][0.1][0.0][0.1][0.2][0.3][0.4[0.5][0.6]...   
                                                           |
                                                TimeMap triggers here (50 ms have passed)
LED draw:                                       Draw the collected data to the strip 
```

The collected data is mapped to each pixel in order. The values are mapped to CRGB colors through a palette : 
```text
Pixel index :      0    1    2    3    4    5    6    7   8    9
Collected data : [0.3][0.2][0.1][0.0][0.1][0.2][0.3][0.4[0.5][0.6]...
Palette lookup:  CRGB CRGB CRGB CRGB CRGB CRGB CRGB CRGB CRGB CRGB
```
