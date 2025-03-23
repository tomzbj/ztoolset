#include "zi2c.h"

class CST226SE
{
  public:
    CST226SE(const ZI2C& i2c) : _i2c(i2c)
    {
    }
    void init(void);
    void test(int (*printf_f)(const char*, ...));
    void poll(int (*printf_f)(const char*, ...));
    bool ispressed(void);
    void getxy(int* x, int* y);

  private:
    void reset(void);
    const ZI2C& _i2c;
};
