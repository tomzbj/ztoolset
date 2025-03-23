#include "cst226se.h"

#define CST2xx_REG_STATUS           (0x00)
#define CST226SE_BUFFER_NUM         (28)

void CST226SE::reset()
{
  /* ZPin::write(PA4, 0); _delay_us(30000); ZPin::write(PA4, 1); _delay_us(30000);*/
}

void CST226SE::poll(int (*printf_f)(const char*, ...))
{
  unsigned char buf[8];
  _i2c.read_reg_data(0xd000, buf, 8);    // 0105000c, 1key, 5tx, 12rx
  for(int i = 0; i < 8; i++)
    printf_f("%02x ", buf[i]);
  printf_f("\n");
}

bool CST226SE::ispressed(void)
{
  unsigned char val = _i2c.read_reg(0xd000);
  return (val == 0x06);
}

void CST226SE::getxy(int* x, int* y)
{
  unsigned char buf[4];
  _i2c.read_reg_data(0xd000, buf, 4);    // 0105000c, 1key, 5tx, 12rx
  *x = buf[1] * 16 + (buf[3] >> 4);
  *y = buf[2] * 16 + (buf[3] & 0xf);
}

void CST226SE::test(int (*printf_f)(const char*, ...))
{
  reset();
  _delay_us(200000UL);
  _i2c.write_reg(0xd1, 0x01);
  unsigned long val;
  printf_f("%02x\n", _i2c.addr());
  _i2c.read_reg_data(0xd1f4, &val, 4);    // 0105000c, 1key, 5tx, 12rx
  printf_f("%08lx\n", val);
  _i2c.read_reg_data(0xd1f8, &val, 4);    // 01e000de, resolution: 480x222
  printf_f("%08lx\n", val);
  _i2c.read_reg_data(0xd1fc, &val, 4);    // caca5fdc, firmware checksum,
  printf_f("%08lx\n", val);
  _i2c.read_reg_data(0xd204, &val, 4);    // 00a8495d, ic type & project id
  printf_f("%08lx\n", val);
  _i2c.read_reg_data(0xd208, &val, 4);    // 01000001, firmware version 1.00 build 0001
  printf_f("%08lx\n", val);
  _i2c.write_reg(0xd1, 0x09);
}

void CST226SE::init(void)
{
  reset();
  _delay_us(200000UL);
  _i2c.write_reg(0xd1, 0x09);
}
