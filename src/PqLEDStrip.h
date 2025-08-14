/*
 * PqLedStrip.h
 *
 * (c) 2025 Thomas O Fredericks :: tof(@)t-o-f(.)info
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _PQ_LED_STRIP_
#define _PQ_LED_STRIP_

#include <PlaquetteLib.h>
#include <FastLED.h>

/* TODO

NEED AN ASSERTION FOR COUNT > 0


*/

namespace pq
{

    const uint8_t PROGMEM gamma8[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
        2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
        5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
        10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
        17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
        25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
        37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
        51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
        69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
        90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
        115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
        144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
        177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
        215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255};

    // --------------------------------------------------------

    // =================================================

    template <uint32_t PIN, EOrder ORDER, uint32_t N_PIXELS>
    class LEDStripWS281X : public Unit
    {
    private:
        const uint32_t _nPixels = N_PIXELS;
        CRGB _pixels[N_PIXELS];
        int _pixelIndex = 0;
        CLEDController *controller;
        bool _needToShow = false;
        bool _needToFill = false;
        float _value;
        float _pixelProportionSize;
        int _brightness = 255;

    private:
        enum class PaletteType
        {
            NONE,
            PALETTE16,
            PALETTE32,
            PALETTE256,
            PROGMEM16,
            HSV16,
            HSV32,
            HSV256
        };

        PaletteType _paletteType = PaletteType::NONE;
        TBlendType _blend = LINEARBLEND;

        const CRGBPalette16 *_palette16;
        const CRGBPalette32 *_palette32;
        const CRGBPalette256 *_palette256;
        const TProgmemRGBPalette16 *_progmemPalette16;
        const CHSVPalette16 *_hsvPalette16;
        const CHSVPalette32 *_hsvPalette32;
        const CHSVPalette256 *_hsvPalette256;

    protected:
        virtual void begin() override
        {
            controller = &FastLED.addLeds<WS2812, PIN, ORDER>(_pixels, N_PIXELS);
        }

        virtual void step() override
        {
            if (_needToShow)
            {
                _needToShow = false;
                controller->showLeds(_brightness);
            }
        }

    private:
        void init()
        {
            _pixelProportionSize = 1.0f / N_PIXELS;
        }

        CRGB getColor(float value) const
        {
            uint8_t frac8 = floor(value * 255.0f);

            switch (_paletteType)
            {
            case PaletteType::PALETTE16:
                return ColorFromPalette(*_palette16, frac8, 255, _blend);
            case PaletteType::PALETTE32:
                return ColorFromPalette(*_palette32, frac8, 255, _blend);
            case PaletteType::PALETTE256:
                return ColorFromPalette(*_palette256, frac8, 255, _blend);
            case PaletteType::PROGMEM16:
                return ColorFromPalette(*_progmemPalette16, frac8, 255, _blend);
                // Serial.println(frac8);
                // return ColorFromPalette(RainbowColors_p,frac8, 255, _blend);
                // return CRGB(frac8, 0, 0);
            case PaletteType::HSV16:
                return ColorFromPalette(*_hsvPalette16, frac8, 255, _blend);
            case PaletteType::HSV32:
                return ColorFromPalette(*_hsvPalette32, frac8, 255, _blend);
            case PaletteType::HSV256:
                return ColorFromPalette(*_hsvPalette256, frac8, 255, _blend);
            case PaletteType::NONE:
            default:
                return CRGB(frac8, frac8, frac8);
            }
        }

    public:
        /// Constructor.
        LEDStripWS281X()
        {
            init();
        }

        LEDStripWS281X(const CRGBPalette16 &p, TBlendType blend = LINEARBLEND)
        {
            init();
            palette(p, blend);
        }

        LEDStripWS281X(const CRGBPalette32 &p, TBlendType blend = LINEARBLEND)
        {
            init();
            palette(p, blend);
        }

        LEDStripWS281X(const CRGBPalette256 &p, TBlendType blend = LINEARBLEND)
        {
            init();
            palette(p, blend);
        }

        LEDStripWS281X(const TProgmemRGBPalette16 &p, TBlendType blend = LINEARBLEND)
        {
            init();
            palette(p, blend);
        }

        LEDStripWS281X(const CHSVPalette16 &p, TBlendType blend = LINEARBLEND)
        {
            init();
            palette(p, blend);
        }

        LEDStripWS281X(const CHSVPalette32 &p, TBlendType blend = LINEARBLEND)
        {
            init();
            palette(p, blend);
        }

        LEDStripWS281X(const CHSVPalette256 &p, TBlendType blend = LINEARBLEND)
        {
            init();
            palette(p, blend);
        }

        void noPalette()
        {
            _paletteType = PaletteType::NONE;
        }

        void palette(const CRGBPalette16 &p, TBlendType blend = LINEARBLEND)
        {
            _palette16 = &p;
            _blend = blend;
            _paletteType = PaletteType::PALETTE16;
        }

        void palette(const CRGBPalette32 &p, TBlendType blend = LINEARBLEND)
        {
            _palette32 = &p;
            _blend = blend;
            _paletteType = PaletteType::PALETTE32;
        }

        void palette(const CRGBPalette256 &p, TBlendType blend = LINEARBLEND)
        {
            _palette256 = &p;
            _blend = blend;
            _paletteType = PaletteType::PALETTE256;
        }

        void palette(const TProgmemRGBPalette16 &p, TBlendType blend = LINEARBLEND)
        {
            _progmemPalette16 = &p;
            _blend = blend;
            _paletteType = PaletteType::PROGMEM16;
        }

        void palette(const CHSVPalette16 &p, TBlendType blend = LINEARBLEND)
        {
            _hsvPalette16 = &p;
            _blend = blend;
            _paletteType = PaletteType::HSV16;
        }

        void palette(const CHSVPalette32 &p, TBlendType blend = LINEARBLEND)
        {
            _hsvPalette32 = &p;
            _blend = blend;
            _paletteType = PaletteType::HSV32;
        }

        void palette(const CHSVPalette256 &p, TBlendType blend = LINEARBLEND)
        {
            _hsvPalette256 = &p;
            _blend = blend;
            _paletteType = PaletteType::HSV256;
        }

        void draw(AbstractField& field)
        {
            float proportion = _pixelProportionSize / 2;
            for (int i = 0; i < N_PIXELS; i++, proportion += _pixelProportionSize)
            {
                float value = field.read(proportion);
                CRGB color = getColor(value);
                _pixels[i] = color;
                
                Serial.print(i); Serial.print(" ");
                Serial.print(value); Serial.print(" ");
                Serial.print(color.r); Serial.print(" ");
                Serial.print(color.g); Serial.print(" ");
                Serial.print(color.b); Serial.println();
                */
            }

            _needToShow = true;
        }

        void setPixel(int index, CRGB color)
        {
            _pixels[index] = color;
            _needToShow = true;
        }

        size_t nPixels() {
            return N_PIXELS;
        }

        void brightness(float f) {
            _brightness = floor(constrain01(f) * 255.0f);
        }

        float brightness() const {
            return _brightness / 255.0f;
        }

        // PUT SHOULD BE A FORCED OVERRIDE
        virtual float put(float value) override
        {
            _value = value;

            CRGB color = getColor(_value);
            fill_solid(_pixels, N_PIXELS, color);

            _needToShow = true;

            return get();
        } // do nothing by default (read-only)

        /// Returns value in [0, 1].
        virtual float get() override { return _value; }
    };
}
#endif