/**
 * Adapted from https://github.com/DaleGia/nano-33-sense-serial-example
 */

#include <PDM.h>
#include <arm_math.h>

#define MICROPHONE_BUFFER_SIZE_IN_WORDS (256U)
#define MICROPHONE_BUFFER_SIZE_IN_BYTES (MICROPHONE_BUFFER_SIZE_IN_WORDS * sizeof(int16_t))


/**
 * Simple access to Ble Sense mic
 */
class Mic {
public:
    Mic() :
            _ready(false) {
    }

    bool begin(uint8_t gain = 20) {
        PDM.begin(1, 16000);
        PDM.setGain(gain);
    }

    bool hasData() {
        return _ready;
    }

    int16_t data() {
        return _rms;
    }

    int16_t pop() {
        int16_t rms = data();
        reset();
        return rms;
    }

    void update() {
        int bytesAvailable = PDM.available();
        if(bytesAvailable == MICROPHONE_BUFFER_SIZE_IN_BYTES) {
            int16_t _buffer[MICROPHONE_BUFFER_SIZE_IN_WORDS];
            _ready = true;
            PDM.read(_buffer, bytesAvailable);
            arm_rms_q15((q15_t*)_buffer, MICROPHONE_BUFFER_SIZE_IN_WORDS, (q15_t*)&_rms);
        }
    }

    void reset() {
        _ready = false;
    }


protected:
    int16_t _rms;
    bool _ready;
};
