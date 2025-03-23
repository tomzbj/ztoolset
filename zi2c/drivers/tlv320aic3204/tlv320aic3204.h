#ifndef _AIC3204_H_
#define _AIC3204_H_

#include <cstdint>
#include "zi2c.h"

//I2C Slave addr.
//#define AIC3204_ADDR 0x30
#define AIC3204_OUT_GAIN_MAX    29.0
#define AIC3204_OUT_GAIN_MIN    -6.0
#define IN_GAIN_MAX     47
#define IN_GAIN_MIN     0
#define AIC3204_DAC_VOL_MAX     24.0
#define AIC3204_DAC_VOL_MIN     -63.5

class TLV320AIC3204 {

  public:
    typedef enum {
      CHANNEL_L, CHANNEL_R, CHANNEL_BOTH
    } channel_t;

    typedef enum {
      IN1_L, IN1_R, IN2_L, IN2_R, IN3_L, IN3_R, IN1_BOTH, IN2_BOTH, IN3_BOTH
    } adc_input_t;

    typedef enum {
      AIC3204_L_P = 1, AIC3204_L_N = 2, AIC3204_R_P = 4, AIC3204_R_N = 8
    } adc_routing_t;

    typedef enum {
      GAIN_IN_0db = 1, GAIN_IN_6db = 2, GAIN_IN_12db = 3, GAIN_IN_NC = 0
    } adc_gain_t;

    typedef enum {
      MICBIAS_1V04_1V25 = 0, MICBIAS_1V425_1V7 = 1, MICBIAS_2V075_2V5 = 2,
      MICBIAS_SUPPLY = 3
    } micbias_volt_t;

    typedef enum {
      MICBIAS_POWER_AVDD = 0, MICBIAS_POWER_LDOIN = 1
    } micbias_src_t;

    const int IN1_L_VALID_MASK = AIC3204_L_P | AIC3204_R_N;
    const int IN1_R_VALID_MASK = AIC3204_L_P | AIC3204_R_P;
    const int IN2_L_VALID_MASK = AIC3204_L_P | AIC3204_R_P;
    const int IN2_R_VALID_MASK = AIC3204_L_N | AIC3204_R_P;
    const int IN3_L_VALID_MASK = AIC3204_L_P | AIC3204_R_N;
    const int IN3_R_VALID_MASK = AIC3204_L_N | AIC3204_R_P;

    TLV320AIC3204(const ZI2C& i2c,    //
      void (*delay_ms_f)(volatile int nms),    //
      void (*reset_f)(int state)    //
        ) : _i2c(i2c), _delay_ms_f(delay_ms_f), _reset_f(reset_f)
    {
    }

    void init(void);
    void set_headphone_mute(channel_t ch, bool value);
    void set_line_out_mute(channel_t ch, bool value);
    void set_headphone_volume(channel_t ch, float level);
    void set_line_out_volume(channel_t ch, float level);
    void set_dac_digital_volume(channel_t ch, float level);
    void set_input_volume(channel_t ch, uint8_t level);
    void set_adc_routing(adc_input_t in_terminal, int routing_mask,
      adc_gain_t gain);
    void set_micbias(micbias_volt_t voltage, micbias_src_t src, bool value);

    void view_regs(int printf_f(const char*, ...));

  private:
    const ZI2C& _i2c;
    void (*_delay_ms_f)(volatile int nms);
    void (*_reset_f)(int state);

    void reset(void);
};

#endif
