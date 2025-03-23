#include "tlv320aic3204.h"

void TLV320AIC3204::set_headphone_mute(channel_t ch, bool value)
{
  uint8_t tmp;
  _i2c.write_reg(0x00, 0x01);    //Select Page 1
  if(ch == CHANNEL_L) {
    tmp = _i2c.read_reg(0x10);    //Read the HPL gain
    tmp = value ? tmp | 0x40 : tmp & 0x3f;
    _i2c.write_reg(0x10, tmp);    //Set the HPL gain
  }
  else if(ch == CHANNEL_R) {
    tmp = _i2c.read_reg(0x11);    //Read the HPR gain
    tmp = value ? tmp | 0x40 : tmp & 0x3f;
    _i2c.write_reg(0x11, tmp);    //Set the HPR gain
  }
  else {
    tmp = _i2c.read_reg(0x10);    //Read the HPL gain
    tmp = value ? tmp | 0x40 : tmp & 0x3f;
    _i2c.write_reg(0x10, tmp);    //Set the HPL gain
    tmp = _i2c.read_reg(0x11);    //Read the HPR gain
    tmp = value ? tmp | 0x40 : tmp & 0x3f;
    _i2c.write_reg(0x11, tmp);    //Set the HPR gain
  }
}

void TLV320AIC3204::set_line_out_mute(channel_t ch, bool value)
{
  uint8_t tmp;
  _i2c.write_reg(0x00, 0x01);    //Select Page 1
  if(ch == CHANNEL_L) {
    tmp = _i2c.read_reg(0x12);    //Read the LOL gain
    tmp = value ? tmp | 0x40 : tmp & 0x3f;
    _i2c.write_reg(0x12, tmp);    //Set the LOL gain
  }
  else if(ch == CHANNEL_R) {
    tmp = _i2c.read_reg(0x13);    //Read the LOR gain
    tmp = value ? tmp | 0x40 : tmp & 0x3f;
    _i2c.write_reg(0x13, tmp);    //Set the LOR gain
  }
  else {
    tmp = _i2c.read_reg(0x12);    //Read the LOL gain
    tmp = value ? tmp | 0x40 : tmp & 0x3f;
    _i2c.write_reg(0x12, tmp);    //Set the LOL gain
    tmp = _i2c.read_reg(0x13);    //Read the LOR gain
    tmp = value ? tmp | 0x40 : tmp & 0x3f;
    _i2c.write_reg(0x13, tmp);    //Set the LOR gain
  }
}

void TLV320AIC3204::set_headphone_volume(channel_t ch, float level)
{
  level = saturate(level, AIC3204_OUT_GAIN_MIN, AIC3204_OUT_GAIN_MAX);
  uint8_t value;
  uint8_t tmp;
  //Register setting value is expressed by 2's complement, 6-bit
  value = ((int8_t)level) & 0x3f;    //e.g. : -6db = 0b11 1010
  _i2c.write_reg(0x00, 0x01);    //Select Page 1
  if(ch == CHANNEL_L) {
    tmp = _i2c.read_reg(0x10);    //Read the HPL gain
    tmp = value ? tmp | 0x40 : tmp & 0x3f;
    _i2c.write_reg(0x10, value);    //Set the HPL gain
  }
  else if(ch == CHANNEL_R) {
    tmp = _i2c.read_reg(0x11);    //Read the HPR gain
    tmp = value ? tmp | 0x40 : tmp & 0x3f;
    _i2c.write_reg(0x11, value);    //Set the HPR gain
  }
  else {
    tmp = _i2c.read_reg(0x10);    //Read the HPL gain
    tmp = value ? tmp | 0x40 : tmp & 0x3f;
    _i2c.write_reg(0x10, value);    //Set the HPL gain
    tmp = _i2c.read_reg(0x11);    //Read the HPR gain
    tmp = value ? tmp | 0x40 : tmp & 0x3f;
    _i2c.write_reg(0x11, value);    //Set the HPR gain
  }
}

void TLV320AIC3204::set_line_out_volume(channel_t ch, float level)
{
  level = saturate(level, AIC3204_OUT_GAIN_MIN, AIC3204_OUT_GAIN_MAX);

  uint8_t value;
  uint8_t tmp;
  //Register setting value is expressed by 2's complement, 6-bit
  value = ((int8_t)level) & 0x3f;    //e.g. : -6db = 0b11 1010
  _i2c.write_reg(0x00, 0x01);    //Select Page 1
  if(ch == CHANNEL_L) {
    tmp = _i2c.read_reg(0x12);    //Read the LOL gain
    tmp = value ? tmp | 0x40 : tmp & 0x3f;
    _i2c.write_reg(0x12, value);    //Set the LOL gain
  }
  else if(ch == CHANNEL_R) {
    tmp = _i2c.read_reg(0x13);    //Read the LOR gain
    tmp = value ? tmp | 0x40 : tmp & 0x3f;
    _i2c.write_reg(0x13, value);    //Set the LOR gain
  }
  else {
    tmp = _i2c.read_reg(0x12);    //Read the LOL gain
    tmp = value ? tmp | 0x40 : tmp & 0x3f;
    _i2c.write_reg(0x12, value);    //Set the LOL gain
    tmp = _i2c.read_reg(0x13);    //Read the LOR gain
    tmp = value ? tmp | 0x40 : tmp & 0x3f;
    _i2c.write_reg(0x13, value);    //Set the LOR gain
  }
}

void TLV320AIC3204::set_dac_digital_volume(channel_t ch, float level)
{
  uint8_t value;
  level = saturate(level, AIC3204_DAC_VOL_MIN, AIC3204_DAC_VOL_MAX);
  level = level * 2.0;
  value = (int8_t)level;

  _i2c.write_reg(0x00, 0x00);    //Select Page 0
  if(ch == CHANNEL_L) {
    _i2c.write_reg(0x41, value);    //Set the LOL gain
  }
  else if(ch == CHANNEL_R) {
    _i2c.write_reg(0x42, value);    //Set the LOR gain
  }
  else {
    _i2c.write_reg(0x41, value);
    _i2c.write_reg(0x42, value);
  }
}

void TLV320AIC3204::set_input_volume(channel_t ch, uint8_t level)
{
  level = saturate(level, IN_GAIN_MIN, IN_GAIN_MAX)*2;

  _i2c.write_reg(0x00, 0x01);    //Select Page 1
  if(ch == CHANNEL_L) {
    _i2c.write_reg(0x3B, level);    //Set the LOL gain
  }
  else if(ch == CHANNEL_R) {
    _i2c.write_reg(0x3C, level);    //Set the LOR gain
  }
  else {
    _i2c.write_reg(0x3B, level);
    _i2c.write_reg(0x3C, level);
  }
}

void TLV320AIC3204::set_adc_routing(adc_input_t in_terminal, int routing_mask,
  adc_gain_t gain)
{
  if(in_terminal >= IN1_BOTH)    //this function does not accept AIC3204_xx_BOTH
    return;    // ESP_ERR_INVALID_ARG;

  _i2c.write_reg(0x00, 0x01);    //Select Page 1
  uint8_t tmp;
  if(in_terminal == IN1_L) {
    if((routing_mask & (~IN1_L_VALID_MASK)) != 0)
      return;    // ESP_ERR_INVALID_ARG;

    tmp = _i2c.read_reg(0x34);    //Read current value
    tmp &= 0x3f;
    if(routing_mask & AIC3204_L_P)
      tmp |= (gain << 6);
    _i2c.write_reg(0x34, tmp);

    tmp = _i2c.read_reg(0x39);    //Read current value
    tmp &= 0xcf;
    if(routing_mask & AIC3204_R_N)
      tmp |= (gain << 4);
    _i2c.write_reg(0x39, tmp);
  }

  if(in_terminal == IN1_R) {
    if((routing_mask & (~IN1_R_VALID_MASK)) != 0)
      return;    //ESP_ERR_INVALID_ARG;

    tmp = _i2c.read_reg(0x34);    //Read current value
    tmp &= 0xfc;
    if(routing_mask & AIC3204_L_P)
      tmp |= gain;
    _i2c.write_reg(0x34, tmp);

    tmp = _i2c.read_reg(0x37);    //Read current value
    tmp &= 0x3f;
    if(routing_mask & AIC3204_R_P)
      tmp |= (gain << 6);
    _i2c.write_reg(0x37, tmp);
  }

  if(in_terminal == IN2_L) {
    if((routing_mask & (~IN2_L_VALID_MASK)) != 0)
      return;    //ESP_ERR_INVALID_ARG;

    //Read current value
    tmp = _i2c.read_reg(0x34);
    tmp &= 0xcf;
    if(routing_mask & AIC3204_L_P)
      tmp |= (gain << 4);
    _i2c.write_reg(0x34, tmp);

    tmp = _i2c.read_reg(0x37);    //Read current value
    tmp &= 0xfc;
    if(routing_mask & AIC3204_R_P)
      tmp |= gain;
    _i2c.write_reg(0x37, tmp);
  }

  if(in_terminal == IN2_R) {
    if((routing_mask & (~IN2_R_VALID_MASK)) != 0)
      return;    //ESP_ERR_INVALID_ARG;

    tmp = _i2c.read_reg(0x36);    //Read current value
    tmp &= 0xcf;
    if(routing_mask & AIC3204_L_N)
      tmp |= (gain << 4);
    _i2c.write_reg(0x36, tmp);

    tmp = _i2c.read_reg(0x37);    //Read current value
    tmp &= 0xcf;
    if(routing_mask & AIC3204_R_P)
      tmp |= (gain << 4);
    _i2c.write_reg(0x37, tmp);
  }

  if(in_terminal == IN3_L) {
    if((routing_mask & (~IN3_L_VALID_MASK)) != 0)
      return;    //ESP_ERR_INVALID_ARG;

    tmp = _i2c.read_reg(0x34);    //Read current value
    tmp &= 0xf3;
    if(routing_mask & AIC3204_L_P)
      tmp |= (gain << 2);
    _i2c.write_reg(0x34, tmp);

    //Read current value
    tmp = _i2c.read_reg(0x39);
    tmp &= 0xf3;
    if(routing_mask & AIC3204_R_N)
      tmp |= (gain << 2);
    _i2c.write_reg(0x39, tmp);
  }

  if(in_terminal == IN3_R) {
    if((routing_mask & (~IN3_R_VALID_MASK)) != 0)
      return;    //ESP_ERR_INVALID_ARG;

    //Read current value
    tmp = _i2c.read_reg(0x36);
    tmp &= 0xf3;
    if(routing_mask & AIC3204_L_N)
      tmp |= (gain << 2);
    _i2c.write_reg(0x36, tmp);

    //Read current value
    tmp = _i2c.read_reg(0x37);
    tmp &= 0xf3;
    if(routing_mask & AIC3204_R_P)
      tmp |= (gain << 2);
    _i2c.write_reg(0x37, tmp);
  }
}

void TLV320AIC3204::set_micbias(micbias_volt_t volt, micbias_src_t src,
  bool val)
{
  _i2c.write_reg(0x00, 0x01);    //Select Page 1
  uint8_t tmp = 0;
  if(val == true)
    tmp |= 0x40;
  tmp |= ((uint8_t)volt) << 4;
  tmp |= ((uint8_t)src) << 3;
  _i2c.write_reg(0x33, tmp);    //Write Settings
}

void TLV320AIC3204::reset(void)
{
//  gpio_set_level((gpio_num_t)SUCODEC_CODEC_RESET, 0);
  _reset_f(0);
  _delay_ms_f(100);
//  vTaskDelay(100 / portTICK_PERIOD_MS);
//  gpio_set_level((gpio_num_t)SUCODEC_CODEC_RESET, 1);
  _reset_f(1);
  _delay_ms_f(10);
}

void TLV320AIC3204::init(void)
{
  reset();
  _i2c.write_reg(0x00, 0x00);    //Page 0
  _i2c.write_reg(0x01, 0x01);    //Software Reset
//  vTaskDelay(10 / portTICK_PERIOD_MS);
  _delay_ms_f(10);
  /*---Clock Settings---*/
//  _i2c.write_reg(0x19, 0x00);    //Select CODEC_CLKIN = MCLK
  _i2c.write_reg(0x19, 0x03);    //Select CODEC_CLKIN = PLL
  _i2c.write_reg(0x1a, 0x81);    //Set CLKOUT_M Divider Active
  _i2c.write_reg(0x0b, 0x81);    //Power up the NDAC divider with value 1
  _i2c.write_reg(0x0c, 0x82);    //Power up the MDAC divider with value 2
  _i2c.write_reg(0x12, 0x81);    // Power up NADC divider with value 1
  _i2c.write_reg(0x13, 0x82);    // Power up MADC divider with value 2
  _i2c.write_reg(0x0d, 0x00);
  _i2c.write_reg(0x0e, 0x80);    //Program the OSR of DAC to 128
  _i2c.write_reg(0x14, 0x80);    // Program OSR for ADC to 128

  /*---Digital Section---*/
  //_i2c.write_reg(0x1b,0x00); //Set I2S, 16bit Data Word, BCLK and WCLK is input
  _i2c.write_reg(0x3b, 0x00);    //Set I2S, 32bit Data Word, BCLK and WCLK is input
  _i2c.write_reg(0x3c, 0x08);    //Set the DAC Mode to PRB_P8
  _i2c.write_reg(0x3d, 0x01);    // Select ADC PRB_R1

  /*---Power and Analog---*/
  _i2c.write_reg(0x00, 0x01); /* Select Page 1 */

  //Disable Internal Crude AVdd in presence of external AVdd supply or before
  _i2c.write_reg(0x01, 0x08);    //powering up internal AVdd LDO
  _i2c.write_reg(0x02, 0x01);    //Enable Master Analog Power Control
  _i2c.write_reg(0x7b, 0x01);    //Set the REF charging time to 40ms
  //HP soft stepping settings for optimal pop performance at power up
  //Rpop used is 6k with N = 6 and soft step = 20usec. This should work with 47uF coupling
  //capacitor. Can try N=5,6 or 7 time constants as well. Trade-off delay vs “pop” sound.
  _i2c.write_reg(0x14, 0x25);
  //Set the Input Common Mode to 0.9V and Output Common Mode for Headphone to
  _i2c.write_reg(0x0a, 0x00);    //Input Common Mode

  /*---Analog Routing---*/
  _i2c.write_reg(0x0c, 0x08);    //Route Left DAC to HPL
  _i2c.write_reg(0x0d, 0x08);    //Route Right DAC to HPR
  _i2c.write_reg(0x0e, 0x08);    //Route Left DAC to LOL
  _i2c.write_reg(0x0f, 0x08);    //Route Right DAC to LOR
  _i2c.write_reg(0x03, 0x00);    //Set the DAC PTM mode to PTM_P3/4
  _i2c.write_reg(0x04, 0x00);
  _i2c.write_reg(0x3d, 0x00);    // Select ADC PTM_R4
  _i2c.write_reg(0x10, 0x3a);    //Set the HPL gain to -6dB
  _i2c.write_reg(0x11, 0x3a);    //Set the HPR gain to -6dB
  _i2c.write_reg(0x12, 0x3a);    //Set the LOL gain to -6dB
  _i2c.write_reg(0x13, 0x3a);    //Set the LOR gain to -6dB
  _i2c.write_reg(0x09, 0x3c);    //Power up HPL and HPR, LOL and LOR drivers
  //Wait for 2.5 sec for soft stepping to take effect
  //Else read Page 1, Register 63d, D(7:6). When = “11” soft-stepping is complete
//  vTaskDelay(2500 / portTICK_PERIOD_MS);
  _delay_ms_f(500);
  _i2c.write_reg(0x47, 0x32);    // Set MicPGA startup delay to 3.1ms
  _i2c.write_reg(0x7b, 0x01);    // Set the REF charging time to 40ms

  /*
   _i2c.write_reg(0x34, 0x80);    // Route IN1L to LEFT_P with 20K input impedance
   _i2c.write_reg(0x36, 0x80);    // Route Common Mode to LEFT_M with impedance of 20K
   _i2c.write_reg(0x37, 0x80);    // Route IN1R to RIGHT_P with input impedance of 20K
   _i2c.write_reg(0x39, 0x80);    // Route Common Mode to RIGHT_M with impedance of 20K
   */
  _i2c.write_reg(0x37, 0xc0);    // IN1R - RIGHT_P,  20K Zin
  _i2c.write_reg(0x39, 0x30);    // IN1L - RIGHT_N,  20K Zin

  _i2c.write_reg(0x34, 0x30);    // IN2L - LEFT_P, 20K Zin
  _i2c.write_reg(0x36, 0x30);    // IN2R - LEFT_N, 20K Zin

  // Unmute Left MICPGA, Gain selection of 6dB to make channel gain 0dB
  _i2c.write_reg(0x3b, 0x0c);    // Register of 6dB with input impedance of 20K => Channel Gain of 0dB
  // Unmute Right MICPGA, Gain selection of 6dB to make channel gain 0dB
  _i2c.write_reg(0x3c, 0x0c);    // Register of 6dB with input impedance of 20K => Channel Gain of 0dB
  _i2c.write_reg(0x00, 0x00);    //Select Page 0
  //Power up the Left and Right DAC Channels with route the Left Audio digital data to
  //Left Channel DAC and Right Audio digital data to Right Channel DAC
  _i2c.write_reg(0x3f, 0xd6);
  _i2c.write_reg(0x51, 0xc0);    // Power up Left and Right ADC Channels
  _i2c.write_reg(0x40, 0x00);    //Unmute the DAC digital volume control and enable syncronized control (use L-ch)
  _i2c.write_reg(0x52, 0x00);    // Unmute Left and Right ADC Digital Volume Control.
}

void TLV320AIC3204::view_regs(int printf_f(const char*, ...))
{
  for(int i = 0; i < 256; i++) {
    if(i % 16 == 0)
      printf_f("%02x: ", i);
    printf_f("%02x ", _i2c.read_reg(i));
    if(i % 16 == 15)
      printf_f("\n");
  }
  printf_f("\n");
}
