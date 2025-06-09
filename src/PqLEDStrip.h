#ifndef _PQ_LED_STRIP_
#define _PQ_LED_STRIP_

#include <Plaquette.h>
#include <FastLED.h>

namespace pq
{
    /*
class _PqStripManager : public Unit {

    float get() override { return 0; }

    void step() override {
        FastLED.show();
    }
};
inline _PqStripManager PqStripManager;
*/

    // --------------------------------------------------------

    class PqPalette
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
    class PqStripWS281X : public Unit
    {
    private:
        const uint32_t _count = COUNT;
        CRGB _pixels[COUNT];
        int _pixelIndex = 0;
        CLEDController *controller;

    public:
        PqPalette palette;

    protected:
        void begin() override
        {
            controller = &FastLED.addLeds<WS2812, PIN, ORDER>(_pixels, COUNT);
        }

        void step()
        {
            
            controller->showLeds(255);
        }

    public:
        /// Constructor.
        PqStripWS281X() : Unit()
        {
            // engine.addAttachment(&_fastLedSingletonAttachment);
            // _value = constrain(initialValue, 0, 1);
        }


        void applyWave(AbstractWave & wave) {
            float step = 1.0 / float(COUNT);
            for ( int i =0; i < COUNT; i++) {
                _pixels[i] = palette.getColor( wave.shiftByTime(float(i)*step) );
            }  
        }
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
        */
        /*
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
            // uint8_t frac8 = floor(_value * 255.0);
            CRGB color = palette.getColor(_value);
            fill_solid(_pixels, COUNT, color);
            return get();
        } // do nothing by default (read-only)

        /// Returns value in [0, 1].
        float get() override { return _value; }

        /// Returns value in [0, 1].
        // virtual float get() { return _value; }

        /// Maps value to new range.
        // virtual float mapTo(float toLow, float toHigh) { return mapFrom01(get(), toLow, toHigh); }

    protected:
        float _value;
    };
}
#endif