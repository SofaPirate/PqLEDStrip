#ifndef _PQ_LED_STRIP_
#define _PQ_LED_STRIP_

#include <PlaquetteLib.h>
#include <FastLED.h>

/* TODO

NEED AN ASSERTION FOR COUNT > 0


*/

namespace pq
{
    class CubeLayout
    {
        // Dimensions of your cube
        const uint32_t _lanes;
        const uint32_t _depths;
        const uint32_t _heights;

    public:
        CubeLayout(uint32_t lanes, uint32_t depths, uint32_t heights) : _lanes(lanes), _depths(depths), _heights(heights)
        {
        }

        size_t getIndex(uint32_t l_q032, uint32_t d_q032, uint32_t h_q032)
        {
            // Convert Q0.32 to integer coordinates
            uint32_t l = (uint64_t)l_q032 * _lanes >> 32;
            uint32_t d = (uint64_t)d_q032 * _depths >> 32;
            uint32_t h = (uint64_t)h_q032 * _heights >> 32;

            // Clamp to bounds (really necessary?)
            if (l >= _lanes)
                l = _lanes - 1;
            if (d >= _depths)
                d = _depths - 1;
            if (h >= _heights)
                h = _heights - 1;

            // Compute linear index (l fast, then d, then h)
            return h + d * _lanes + l * _heights * _depths; // l + d * _lanes + h * _lanes * _depths;
        }

        // uint32_t pos[3];
        // cube.getPosition(idx, pos);
        void getPosition(size_t index, uint32_t out_pos[3])
        {
            out_pos[0] = index / (_heights * _depths); // lane
            uint32_t rem = index % (_heights * _depths);
            out_pos[1] = rem / _heights; // depth
            out_pos[2] = rem % _heights; // height
        }
    };

    // --------------------------------------------------------

    class LEDStripPalette
    {

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

        PaletteType _type = PaletteType::NONE;
        TBlendType _blend = LINEARBLEND;

        const CRGBPalette16 *_palette16;
        const CRGBPalette32 *_palette32;
        const CRGBPalette256 *_palette256;
        const TProgmemRGBPalette16 *_progmemPalette16;
        const CHSVPalette16 *_hsvPalette16;
        const CHSVPalette32 *_hsvPalette32;
        const CHSVPalette256 *_hsvPalette256;

    public:
        // Setters
        void set(const CRGBPalette16 &p, TBlendType blend = LINEARBLEND)
        {
            _palette16 = &p;
            _blend = blend;
            _type = PaletteType::PALETTE16;
        }

        void set(const CRGBPalette32 &p, TBlendType blend = LINEARBLEND)
        {
            _palette32 = &p;
            _blend = blend;
            _type = PaletteType::PALETTE32;
        }

        void set(const CRGBPalette256 &p, TBlendType blend = LINEARBLEND)
        {
            _palette256 = &p;
            _blend = blend;
            _type = PaletteType::PALETTE256;
        }

        void set(const TProgmemRGBPalette16 &p, TBlendType blend = LINEARBLEND)
        {
            _progmemPalette16 = &p;
            _blend = blend;
            _type = PaletteType::PROGMEM16;
        }

        void set(const CHSVPalette16 &p, TBlendType blend = LINEARBLEND)
        {
            _hsvPalette16 = &p;
            _blend = blend;
            _type = PaletteType::HSV16;
        }

        void set(const CHSVPalette32 &p, TBlendType blend = LINEARBLEND)
        {
            _hsvPalette32 = &p;
            _blend = blend;
            _type = PaletteType::HSV32;
        }

        void set(const CHSVPalette256 &p, TBlendType blend = LINEARBLEND)
        {
            _hsvPalette256 = &p;
            _blend = blend;
            _type = PaletteType::HSV256;
        }

        /// Returns a color from the current palette using a normalized float in [0,1].
        CRGB getColor(float value) const
        {
            uint8_t frac8 = static_cast<uint8_t>(constrain(value, 0.0f, 1.0f) * 255.0f);

            switch (_type)
            {
            case PaletteType::PALETTE16:
                return ColorFromPalette(*_palette16, frac8, 255, _blend);
            case PaletteType::PALETTE32:
                return ColorFromPalette(*_palette32, frac8, 255, _blend);
            case PaletteType::PALETTE256:
                return ColorFromPalette(*_palette256, frac8, 255, _blend);
            case PaletteType::PROGMEM16:
                return ColorFromPalette(*_progmemPalette16, frac8, 255, _blend);
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
        /*
            PaletteType type() const { return _type; }
            TBlendType blend() const { return _blend; }
            */
    };

    // =================================================

    template <uint32_t PIN, EOrder ORDER, uint32_t COUNT>
    class LEDStripWS281X : public Unit
    {
    private:
        const uint32_t _count = COUNT;
        CRGB _pixels[COUNT];
        int _pixelIndex = 0;
        CLEDController *controller;
        bool _needToShow = false;
        bool _needToFill = false;
        float _value;

    public:
        LEDStripPalette palette;

    protected:
        void begin() override
        {
            controller = &FastLED.addLeds<WS2812, PIN, ORDER>(_pixels, COUNT);
        }

        void step()
        {

            if (_needToShow)
            {
                _needToShow = false;
                //_pixels[0] = CRGB(255,0,0);
                //_pixels[1] = CRGB(0,255,0);
                controller->showLeds(255);
            }
        }

    public:
        /// Constructor.
        LEDStripWS281X() : Unit()
        {
            // engine.addAttachment(&_fastLedSingletonAttachment);
            // _value = constrain(initialValue, 0, 1);
        }

        void draw(AbstractMap &map)
        {

            float stepInterval = pq::stepIntervalForNormalizedValue(COUNT);
            // if COUNT == 2, step = 0.5, so interated values are 0, 0.5 and 1
            // if COUNT == 3, step = 0.33333,
            /*for (float f =0; f <= 1.0; f += step) {

            }
            */
            for (int i = 0; i < COUNT; i++)
            {

                _pixels[i] = palette.getColor(map.read(float(i) * stepInterval));
            }

            _needToShow = true;
        }
        /*
                void applyWave(AbstractWave &wave, float period = 1.0)
                {
                    if (period > 0)
                    {
                        float step = period / float(COUNT);
                        for (int i = 0; i < COUNT; i++)
                        {
                            _pixels[i] = palette.getColor(wave.getShiftedByTime(float(i) * step));
                        }
                    }
                }
                    */

        /*
                            PqStripWS281X & pixel(int pixelIndex) {
                                _pixelIndex = constrain(pixelIndex,0,COUNT);
                                return *this;
                            }

                            PqStripWS281X &  setRGBFromBlend(CRGB start, CRGB end) {
                                uint8_t amount = (uint8_t)(_value * 255.0);
                                _pixels[_pixelIndex] = CRGB::blend(start, end, amount);
                                return *this;
                            }


                    void setPalette( CRGB alpha , CRGB beta ) {

                    }

                    void pixel(int index) {

                    }
                */
        // virtual ~PqStripWS281X() {}

        // PUT SHOULD BE A FORCED OVERRIDE
        float put(float value) override
        {
            _value = constrain01(value);

            CRGB color = palette.getColor(_value);
            fill_solid(_pixels, COUNT, color);
            _needToShow = true;

            return get();
        } // do nothing by default (read-only)

        /// Returns value in [0, 1].
        float get() override { return _value; }

        /// Returns value in [0, 1].
        // virtual float get() { return _value; }

        /// Maps value to new range.
        // virtual float mapTo(float toLow, float toHigh) { return mapFrom01(get(), toLow, toHigh); }
    };
}
#endif