#ifndef _MCP3421_H
#define _MCP3421_H

#include "zi2c.h"

// default addr = 0x68

class MCP3421 {
public:
    enum {
        MODE_SINGLE_CONV = 0, MODE_CONTINUOUS_CONV = 1,    //
        DATA_RATE_240 = 0, DATA_RATE_60 = 1, DATA_RATE_15 = 2,
        DATA_RATE_3_75 = 3,    //
        GAIN_1X = 0, GAIN_2X = 1, GAIN_4X = 2, GAIN_8X = 3
    };

    MCP3421(const ZI2C& i2c)
        : _i2c(i2c)
    {
    }
    void start(void)
    {
        _reg = 0x90;
        _i2c.write_raw(&_reg, 1);
    }

//    static void reset(void) { _i2c.write_reg(0x00, 0x06); }

    void set_mode(int mode)
    {
        mode &= 0x1;
        _reg = (_reg & 0xef) | (mode << 4);
        _i2c.write_raw(&_reg, 1);
    }
    void set_data_rate(int data_rate)
    {
        data_rate &= 0x3;
        _reg = (_reg & 0xf3) | (data_rate << 2);
        _i2c.write_raw(&_reg, 1);
    }
    void set_gain(int gain)
    {
        gain &= 0x3;
        _reg = (_reg & 0xfc) | (gain);
        _i2c.write_raw(&_reg, 1);
    }

    uint32_t read_data(void)
    {
        uint32_t data = 0;
        _i2c.read_raw((uint8_t*)&data, 4);
        data = (data >> 16) | (data << 16);
        data = ((data & 0xff00ff00) >> 8) | ((data & 0x00ff00ff) << 8);
        data >>= 8;
        return data;
    }

private:
    const ZI2C& _i2c;
    uint8_t _reg = 0x90;
};

#endif
