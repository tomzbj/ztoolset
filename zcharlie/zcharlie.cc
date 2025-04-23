#include "zcharlie.h"
#include <cstdint>

void ZCHARLIE::poll_pin(void)
{
  ++_poll_pin_count %= _num_pins;
  const auto& count = _poll_pin_count;
  for(int y = 0; y < _num_pins; y++) {
    _set_dir_f(y, (count == y) ? L : IN);
  }
  for(int i = 0; i < _num_leds; i++) {
    int x = _leds[i].x - 1;
    int y = _leds[i].y - 1;
    if(y != count)
      continue;
    if(_vram[i / 8] & (1 << (i % 8))) {
      _set_dir_f(x, H);
    }
  }
}

void ZCHARLIE::poll_led(void)
{
  ++_poll_led_count %= _num_leds;
  const auto& count = _poll_led_count;
  _set_dir_f(_prev_x, IN);
  _set_dir_f(_prev_y, IN);
  int x = _leds[count].x - 1;
  int y = _leds[count].y - 1;
  if(_vram[count / 8] & (1 << (count % 8))) {
    _set_dir_f(x, H);
    _set_dir_f(y, L);
  }
  _prev_x = x;
  _prev_y = y;
}

void ZCHARLIE::write(int n)
{
  for(int i = 0; i < _num_digits; i++) {
    _vram[i] = _mask[n % 10];
    n /= 10;
  }
}

void ZCHARLIE::write(uint8_t* data, int count)
{
  if(count > _num_digits)
    count = _num_digits;
  for(int i = 0; i < count; i++) {
    _vram[i] = data[i];
  }
}
