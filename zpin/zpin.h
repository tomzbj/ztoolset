#ifndef _ZPIN_H
#define _ZPIN_H

#include <stdint.h>
#include "misc.h"
#include "zpin_port.h"

typedef enum {
#if defined(GPIOA)
    PA0 = 0x00, PA1 = 0x01, PA2 = 0x02, PA3 = 0x03, PA4 = 0x04, PA5 = 0x05,
    PA6 = 0x06, PA7 = 0x07, PA8 = 0x08, PA9 = 0x09, PA10 = 0x0a, PA11 = 0x0b,
    PA12 = 0x0c, PA13 = 0x0d, PA14 = 0x0e, PA15 = 0x0f,
#endif
#if defined(GPIOB)
    PB0 = 0x10, PB1 = 0x11, PB2 = 0x12, PB3 = 0x13, PB4 = 0x14, PB5 = 0x15,
    PB6 = 0x16, PB7 = 0x17, PB8 = 0x18, PB9 = 0x19, PB10 = 0x1a, PB11 = 0x1b,
    PB12 = 0x1c, PB13 = 0x1d, PB14 = 0x1e, PB15 = 0x1f,
#endif
#if defined(GPIOC)
    PC0 = 0x20, PC1 = 0x21, PC2 = 0x22, PC3 = 0x23, PC4 = 0x24, PC5 = 0x25,
    PC6 = 0x26, PC7 = 0x27, PC8 = 0x28, PC9 = 0x29, PC10 = 0x2a, PC11 = 0x2b,
    PC12 = 0x2c, PC13 = 0x2d, PC14 = 0x2e, PC15 = 0x2f,
#endif
#if defined(GPIOD)
    PD0 = 0x30, PD1 = 0x31, PD2 = 0x32, PD3 = 0x33, PD4 = 0x34, PD5 = 0x35,
    PD6 = 0x36, PD7 = 0x37, PD8 = 0x38, PD9 = 0x39, PD10 = 0x3a, PD11 = 0x3b,
    PD12 = 0x3c, PD13 = 0x3d, PD14 = 0x3e, PD15 = 0x3f,
#endif
#if defined(GPIOE)
    PE0 = 0x40, PE1 = 0x41, PE2 = 0x42, PE3 = 0x43, PE4 = 0x44, PE5 = 0x45,
    PE6 = 0x46, PE7 = 0x47, PE8 = 0x48, PE9 = 0x49, PE10 = 0x4a, PE11 = 0x4b,
    PE12 = 0x4c, PE13 = 0x4d, PE14 = 0x4e, PE15 = 0x4f,
#endif
#if defined(GPIOF)
    PF0 = 0x50, PF1 = 0x51, PF2 = 0x52, PF3 = 0x53, PF4 = 0x54, PF5 = 0x55,
    PF6 = 0x56, PF7 = 0x57, PF8 = 0x58, PF9 = 0x59, PF10 = 0x5a, PF11 = 0x5b,
    PF12 = 0x5c, PF13 = 0x5d, PF14 = 0x5e, PF15 = 0x5f,
#endif
#if defined(GPIOG)
    PG0 = 0x60, PG1 = 0x61, PG2 = 0x62, PG3 = 0x63, PG4 = 0x64, PG5 = 0x65,
    PG6 = 0x66, PG7 = 0x67, PG8 = 0x68, PG9 = 0x69, PG10 = 0x6a, PG11 = 0x6b,
    PG12 = 0x6c, PG13 = 0x6d, PG14 = 0x6e, PG15 = 0x6f,
#endif
#if defined(GPIOH)
    PH0 = 0x70, PH1 = 0x71, PH2 = 0x72, PH3 = 0x73, PH4 = 0x74, PH5 = 0x75,
    PH6 = 0x76, PH7 = 0x77, PH8 = 0x78, PH9 = 0x79, PH10 = 0x7a, PH11 = 0x7b,
    PH12 = 0x7c, PH13 = 0x7d, PH14 = 0x7e, PH15 = 0x7f,
#endif
#if defined(GPIOI)
    PI0 = 0x80, PI1 = 0x81, PI2 = 0x82, PI3 = 0x83, PI4 = 0x84, PI5 = 0x85,
    PI6 = 0x86, PI7 = 0x87, PI8 = 0x88, PI9 = 0x89, PI10 = 0x8a, PI11 = 0x8b,
    PI12 = 0x8c, PI13 = 0x8d, PI14 = 0x8e, PI15 = 0x8f,
#endif
#if defined(GPIOJ)
    PJ0 = 0x90, PJ1 = 0x91, PJ2 = 0x92, PJ3 = 0x93, PJ4 = 0x94, PJ5 = 0x95,
    PJ6 = 0x96, PJ7 = 0x97, PJ8 = 0x98, PJ9 = 0x99, PJ10 = 0x9a, PJ11 = 0x9b,
    PJ12 = 0x9c, PJ13 = 0x9d, PJ14 = 0x9e, PJ15 = 0x9f,
#endif
#if defined(GPIOK)
    PK0 = 0xa0, PK1 = 0xa1, PK2 = 0xa2, PK3 = 0xa3, PK4 = 0xa4, PK5 = 0xa5,
    PK6 = 0xa6, PK7 = 0xa7, PK8 = 0xa8, PK9 = 0xa9, PK10 = 0xaa, PK11 = 0xab,
    PK12 = 0xac, PK13 = 0xad, PK14 = 0xae, PK15 = 0xaf,
#endif
} zpin_t;

class ZPin {
  public:
    static inline int read(zpin_t zpin)
    {
      ZPIN_PORT_TYPE port = gpio_port[zpin >> 4];
      uint16_t pin = (1 << (zpin & 0xf));
      return (ZPIN_PORT_IN(port) & pin);
    }
    static inline void toggle(zpin_t zpin)
    {
      ZPIN_PORT_TYPE port = gpio_port[zpin >> 4];
      uint16_t pin = (1 << (zpin & 0xf));
      ZPIN_PORT_OUT(port) ^= pin;
    }
    static inline void write(zpin_t zpin, int status)
    {
      ZPIN_PORT_TYPE port = gpio_port[zpin >> 4];
      uint16_t pin = (1 << (zpin & 0xf));
      if(status)
        ZPIN_PORT_BSRR(port) = pin;
      else
        ZPIN_PORT_BRR(port) = pin;
    }
    static inline void set(zpin_t zpin)
    {
      ZPIN_PORT_TYPE port = gpio_port[zpin >> 4];
      uint16_t pin = (1 << (zpin & 0xf));
      ZPIN_PORT_BSRR(port) = pin;
    }
    static inline void clear(zpin_t zpin)
    {
      ZPIN_PORT_TYPE port = gpio_port[zpin >> 4];
      uint16_t pin = (1 << (zpin & 0xf));
      ZPIN_PORT_BRR(port) = pin;
    }

  private:

    constexpr static ZPIN_PORT_TYPE gpio_port[] = {

#if defined(GPIOA)
GPIOA,
#else
      0,
#endif
#if defined(GPIOB)
GPIOB,
#else
      0,
#endif
#if defined(GPIOC)
GPIOC,
#else
      0,
#endif
#if defined(GPIOD)
GPIOD,
#else
      0,
#endif
#if defined(GPIOE)
GPIOE,
#else
      0,
#endif
#if defined(GPIOF)
GPIOF,
#else
      0,
#endif
#if defined(GPIOG)
GPIOG,
#else
      0,
#endif
#if defined(GPIOH)
GPIOH,
#else
      0,
#endif
#if defined(GPIOI)
GPIOI,
#else
      0,
#endif
#if defined(GPIOJ)
GPIOJ,
#else
      0,
#endif
#if defined(GPIOK)
GPIOK,
#else
      0,
#endif
      };
};

#endif
