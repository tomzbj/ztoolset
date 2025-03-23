#ifndef _ADXL345_I2C_H
#define _ADXL345_I2C_H

//#include "misc.h"
#include <cstdint>
#include "zi2c.h"

class ADXL345 {

public:
    ADXL345(const ZI2C& i2c) : _i2c(i2c)
    {
    }
    uint8_t read_id(void)
    {
        return _i2c.read_reg(REG_DEVID);
    }

    uint8_t read_int_reg(void)
    {
        return _i2c.read_reg(REG_INT_SOURCE);
    }

    void read_xyz(int16_t* x, int16_t* y, int16_t* z)
    {
        uint8_t buf[6];
        _i2c.read_regs(REG_DATAX0, buf, 6);
        *x = buf[1] * 256UL + buf[0];
        *y = buf[3] * 256UL + buf[2];
        *z = buf[5] * 256UL + buf[4];
    }

    void init(void)
    {
        uint8_t ret;
        ret = _i2c.read_reg(REG_DEVID);
        if(ret != 0xe5)
            return;

        _i2c.write_reg(REG_DATA_FORMAT, 0x28);    // 禁用自测, 低电平有效, 全分辨率, 右对齐, 测量范围+/-2G
        _i2c.write_reg(REG_BW_RATE, 0x0a);    // 输出速率100Hz
        _i2c.write_reg(REG_POWER_CTL, 0x08);    // 链接禁止, 测量模式
        _i2c.write_reg(REG_INT_MAP, 0x00);    // 全部发送给INT1
        _i2c.write_reg(REG_THRESH_ACT, 10);    // active阈值
        _i2c.write_reg(REG_ACT_INACT_CTL, 0xc0);    // x使能, 交流耦合
//    _i2c.write_reg(REG_TAP_AXES, 0x07);   // XYZ使能

        _i2c.write_reg(REG_FIFO_CTL, 0x80 | 0x1f);
        _i2c.write_reg(REG_INT_ENABLE, 0x10);    // active中断, TAP中断
        _i2c.write_reg(REG_OFSX, 0);    // X轴偏移0
        _i2c.write_reg(REG_OFSY, 0);    // Y轴偏移0
        _i2c.write_reg(REG_OFSZ, 0);    // Z轴偏移0
    }

private:

    enum {
        REG_DEVID = 0x00, REG_THRESH_TAP = 0x1d, REG_OFSX = 0x1e,
        REG_OFSY = 0x1f, REG_OFSZ = 0x20, REG_DUR = 0x21, REG_LATENT = 0x22,
        REG_WINDOW = 0x23, REG_THRESH_ACT = 0x24, REG_THRESH_INACT = 0x25,
        REG_TIME_INACT = 0x26, REG_ACT_INACT_CTL = 0x27, REG_THRESH_FF = 0x28,
        REG_TIME_FF = 0x29, REG_TAP_AXES = 0x2a, REG_ACT_TAP_STATUS = 0x2b,
        REG_BW_RATE = 0x2c, REG_POWER_CTL = 0x2d, REG_INT_ENABLE = 0x2e,
        REG_INT_MAP = 0x2f, REG_INT_SOURCE = 0x30, REG_DATA_FORMAT = 0x31,
        REG_DATAX0 = 0x32, REG_DATAX1 = 0x33, REG_DATAY0 = 0x34,
        REG_DATAY1 = 0x35, REG_DATAZ0 = 0x36, REG_DATAZ1 = 0x37,
        REG_FIFO_CTL = 0x38, REG_FIFO_STATUS = 0x39,
    };

    const ZI2C& _i2c;

};
#endif
