#ifndef _LPS22HH_I2C_H
#define _LPS22HH_I2C_H

#include <cstdint>
#include "zi2c.h"

/* i2c addr = 0x5c / 0x5d (by SA0) */
class LPS22HH {
  public:
    LPS22HH(const ZI2C& i2c) : _i2c(i2c)
    {
    }
    uint8_t read_id(void)
    {
      return _i2c.read_reg(REG_WHOAMI);
    }
    void init(void)
    {
    }
    uint8_t read_status(void)
    {
      return _i2c.read_reg(REG_STATUS);
    }
    float read_pres(void)
    {
      uint32_t data = _i2c.read_reg(REG_PRES_H);
      data = (data << 8) | _i2c.read_reg(REG_PRES_L);
      data = (data << 8) | _i2c.read_reg(REG_PRES_XL);
      return data / 4096.0;
    }
    float read_temp(void)
    {
      uint16_t data = _i2c.read_reg(REG_TEMP_H);
      data = (data << 8) | _i2c.read_reg(REG_TEMP_L);
      return ((int16_t)data) / 100.0;
    }
    void one_shot(void)
    {
      uint8_t tmp = _i2c.read_reg(REG_CTL2);
      tmp |= 0x1;    // one shot
      _i2c.write_reg(REG_CTL2, tmp);
    }
    void calibrate(int16_t data)
    {
      data *= 16;
      _i2c.write_reg(REG_RPDS_H, data >> 8);
      _i2c.write_reg(REG_RPDS_L, data & 0xff);
    }

  private:
    enum {
      REG_INTCFG = 0x0b, REG_THS_PL = 0x0c, REG_THS_PH = 0x0d, REG_IFCTL = 0x0e,
      REG_WHOAMI = 0x0f, REG_CTL1 = 0x10, REG_CTL2 = 0x11, REG_CTL3 = 0x12,
      REG_FIFOCTL = 0x13, REG_FIFOWTM = 0x14, REG_REF_PL = 0x15,
      REG_REF_PH = 0x16, REG_RPDS_L = 0x18, REG_RPDS_H = 0x19,
      REG_INTSRC = 0x24, REG_FIFO_STA1 = 0x25, REG_FIFO_STA2 = 0x26,
      REG_STATUS = 0x27, REG_PRES_XL = 0x28, REG_PRES_L = 0x29,
      REG_PRES_H = 0x2a, REG_TEMP_L = 0x2b, REG_TEMP_H = 0x2c,
      REG_FIFO_PRES_XL = 0x78, REG_FIFO_PRES_L = 0x79, REG_FIFO_PRES_H = 0x7a,
      REG_FIFO_TEMP_L = 0x7b, REG_FIFO_TEMP_H = 0x7c
    };

    const ZI2C& _i2c;
};
#endif
