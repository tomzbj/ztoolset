#ifndef _LIS3DHTR_I2C_H
#define _LIS3DHTR_I2C_H

#include <cstdint>
#include "zi2c.h"

class LIS3DHTR {

public:
    typedef struct {
        int16_t x, y, z;
    } acc_t;

    LIS3DHTR(const ZI2C& i2c) : _i2c(i2c)
    {
    }

    acc_t read_acc(void)
    {
        acc_t a;
        uint8_t buf[6];
        for(int i = 0; i < 6; i++)
            buf[i] = _i2c.read_reg(0x28 + i);
        a.x = buf[0] + buf[1] * 256;
        a.y = buf[2] + buf[3] * 256;
        a.z = buf[4] + buf[5] * 256;
        return a;
    }

    void read_regs(uint8_t reg0, uint8_t* data, int count)
    {
        for(int i = 0; i < count; i++) {
            data[i] = _i2c.read_reg(reg0 + i);
        }
    }

    uint8_t read_id(void)
    {
        return _i2c.read_reg(REG_WHOAMI);
    }
    void init(void)
    {
        _i2c.read_reg(0x20);
        _i2c.write_reg(REG_CTRL_REG1, 0x17);    // 1Hz, XYZ enabled
        _i2c.write_reg(REG_CTRL_REG4, 0x08);    // high-resolution enabled
    }

private:
    enum {
        REG_WHOAMI = 0x0f, REG_CTRL_REG1 = 0x20, REG_CTRL_REG4 = 0x23
    };

    const ZI2C& _i2c;
};
#endif
