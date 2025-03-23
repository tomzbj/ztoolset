#ifndef _SI5351_H_
#define _SI5351_H_

#include "zi2c.h"
#include <cstdint>

class SI5351 {
public:
    enum {
        PLLA, PLLB
    };
    enum {
        MS0, MS1, MS2
    };

    SI5351(const ZI2C& i2c, uint32_t fxtal) : _i2c(i2c), _fxtal(fxtal)
    {
    }
    void init(void) const;
    void read_all_regs(void) const;
    void set_freq(uint32_t freq) const;
    void set_freq(uint32_t freq_1, uint32_t freq_2) const;

private:
    const ZI2C& _i2c;
    const uint32_t _fxtal;

    void set_pll(int pll_id, uint32_t freq) const;
    void set_ms(int ms_id, int pll_id, int divr) const;
    void set_phase(uint8_t divr) const;
};

#endif /* SI5351_H_ */
