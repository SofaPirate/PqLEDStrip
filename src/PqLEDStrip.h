/*
 * PqLEDStrip.h
 *
 * (c) 2025 Thomas O Fredericks :: tof(@)t-o-f(.)info
 * (c) 2025 Sofian Audry        :: info(@)sofianaudry(.)com
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

namespace pq
{
    template <uint32_t PIN, EOrder ORDER, uint32_t N_PIXELS>
    class LEDStripWS281X : public Unit
    {
        static_assert(N_PIXELS > 0, "N_PIXELS must be greater than 0");
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
            // Show LEDs only when needed.
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
            value *= 255;
            uint8_t frac8 = round(value);

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

        void draw(AbstractField& field, bool wrap = false)
        {
            float pixelProportionSize;
            float proportion;
            if (wrap) {
                pixelProportionSize = 1.0f / N_PIXELS;
                proportion = pixelProportionSize / 2;            
            }
            else {
                pixelProportionSize = 1.0f / min(N_PIXELS-1, 1);
                proportion = 0;
            }
            for (uint32_t i = 0; i < N_PIXELS; i++, proportion += _pixelProportionSize)
            {
                float value = field.at(proportion);
                CRGB color = getColor(value);
                _pixels[i] = color;

                /*
                Serial.print(i); Serial.print(" ");
                Serial.print(value); Serial.print(" ");
                Serial.print(color.r); Serial.print(" ");
                Serial.print(color.g); Serial.print(" ");
                Serial.print(color.b); Serial.println();
                */
            }

            _needToShow = true;
        }

        /**
         * Sets the color of the pixel/LED at the given index
         * @param index the index of the pixel/LED
         * @param color the color
         */
        void setPixel(int index, CRGB color)
        {
            _pixels[index] = color;
            _needToShow = true;
        }

        /** 
         * Returns the color of the pixel/LED at the given index
         * @param index the index of the pixel/LED
         * @return the color
         */
        CRGB getPixel(int index) { return _pixels[index]; }

        /// Returns the number of pixels
        size_t nPixels() { return N_PIXELS;}

        /**
         * Sets the brightness of the strip.
         * @param f brightness in [0, 1]
         */
        void brightness(float f) {
            _brightness = floor(constrain01(f) * 255.0f);
        }

        /// Returns the brightness of the strip.
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