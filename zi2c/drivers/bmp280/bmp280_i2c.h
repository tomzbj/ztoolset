#ifndef _BMP280_I2C_H
#define _BMP280_I2C_H

#include <cstdint>
#include "zi2c.h"

class BMP280 {
public:
    enum {
        SLEEP = 0x0, FORCED = 0x1, NORMAL = 0x3    // power mode
    };
    enum {
        REG_ID = 0xd0, REG_RESET = 0xe0, REG_STATUS = 0xf3,    //
        REG_CTRL_MEAS = 0xf4, REG_CONFIG = 0xf5,    //
        REG_PRESS_MSB = 0xf7, REG_PRESS_LSB = 0xf8, REG_PRESS_XLSB = 0xf9,    //
        REG_TEMP_MSB = 0xfa, REG_TEMP_LSB = 0xfb, REG_TEMP_XLSB = 0xfc,    //
        REG_T1 = 0x88, REG_T2 = 0x8a, REG_T3 = 0x8c, REG_P1 = 0x8e,    //
        REG_P2 = 0x90, REG_P3 = 0x92, REG_P4 = 0x94, REG_P5 = 0x96,    //
        REG_P6 = 0x98, REG_P7 = 0x9a, REG_P8 = 0x9c, REG_P9 = 0x9e,    //
        REG_RESERVED = 0xa0
    };
    BMP280(const ZI2C& i2c) : _i2c(i2c)
    {
    }
    uint8_t read_id(void)
    {
        return _i2c.read_reg(REG_ID);
    }
    uint8_t read_status(void)
    {
        return _i2c.read_reg(REG_STATUS);
    }
    void reset(void)
    {
        _i2c.write_reg(REG_RESET, 0xb6);
    }
    void set_mode(int mode)
    {
        mode &= 0x3;    // keep lowest 2 bits
//        uint8_t val = _i2c.read_reg(REG_CTRL_MEAS);
//        val &= 0xfc;    // clear lowest 2 bits
        uint8_t val = 0xfc;    // clear lowest 2 bits
        val |= mode;
        _i2c.write_reg(REG_CTRL_MEAS, val);
    }
    float read_temp(void)
    {
        uint32_t val = ((uint32_t)_i2c.read_reg(REG_TEMP_MSB)) << 12;
        val |= ((uint32_t)_i2c.read_reg(REG_TEMP_LSB)) << 4;
        val |= _i2c.read_reg(REG_TEMP_XLSB) >> 4;
        float v1, v2;
        v1 = (val / 16384.0 - _cali.T1 / 1024.0) * _cali.T2;
        v2 = (val / 131072.0 - _cali.T1 / 8192.0);
        v2 = v2 * v2 * _cali.T3;
        return (v1 + v2) / 5120.0;
    }
    float read_pressure(void)
    {
        uint32_t val = ((uint32_t)_i2c.read_reg(REG_PRESS_MSB)) << 12;
        val |= ((uint32_t)_i2c.read_reg(REG_PRESS_LSB)) << 4;
        val |= _i2c.read_reg(REG_PRESS_XLSB) >> 4;
        float v1, v2, p;
        v1 = read_temp() * 5120.0 / 2.0 - 64000.0;
        v2 = v1 * v1 * _cali.P6 / 32768.0;
        v2 = v2 + v1 * _cali.P5 * 2.0;
        v2 = v2 / 4.0 + _cali.P4 * 65536.0;
        v1 = (_cali.P3 * v1 * v1 / 524288.0 + _cali.P2 * v1) / 524288.0;
        v1 = (1.0 + v1 / 32768.0) * _cali.P1;
        if(v1 == 0.0)
            return 0.0;
        p = 1048576.0 - val;
        p = (p - v2 / 4096.0) * 6250.0 / v1;
        v1 = _cali.P9 * p * p / 2147483648.0;
        v2 = p * _cali.P8 / 32768.0;
        p = p + (v1 + v2 + _cali.P7) / 16.0;
        return p;
    }
    void init(void)
    {
        reset();
        set_mode(NORMAL);
        _cali.T1 = read_reg16(REG_T1);
        _cali.T2 = read_reg16(REG_T2);
        _cali.T3 = read_reg16(REG_T3);
        _cali.P1 = read_reg16(REG_P1);
        _cali.P2 = read_reg16(REG_P2);
        _cali.P3 = read_reg16(REG_P3);
        _cali.P4 = read_reg16(REG_P4);
        _cali.P5 = read_reg16(REG_P5);
        _cali.P6 = read_reg16(REG_P6);
        _cali.P7 = read_reg16(REG_P7);
        _cali.P8 = read_reg16(REG_P8);
        _cali.P9 = read_reg16(REG_P9);

        printf("%d %d %d %d %d %d %d %d %d %d %d %d\n", _cali.T1, _cali.T2,
            _cali.T3, _cali.P1, _cali.P2, _cali.P3, _cali.P4, _cali.P5,
            _cali.P6, _cali.P7, _cali.P8, _cali.P9);
    }

private:
    const ZI2C& _i2c;
    struct {
        uint16_t T1, P1;
        int16_t T2, T3, P2, P3, P4, P5, P6, P7, P8, P9;
    } _cali;

    uint16_t read_reg16(uint8_t reg)
    {
        return _i2c.read_reg(reg) | (((uint16_t)_i2c.read_reg(reg + 1)) << 8);
    }
};
#endif
