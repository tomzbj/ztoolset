#include "zseg.h"

void ZSEG::poll(void)
{
  for(int i = 0; i < _n_dig; i++)
    _cbs.set_dig_f(i, 1);    // shut down all digits

  for(int j = 0; j < _n_seg; j++) {
    _cbs.set_seg_f(j, (_vram[_curr_digit] & (1 << j)));
  }
  _cbs.set_dig_f(_curr_digit, 0);
  _curr_digit++;
  _curr_digit %= _n_dig;
}

void ZSEG::write(uint8_t* raw)
{
  std::memcpy(_vram.get(), raw, _n_dig);
}

void ZSEG::write(int num)
{
  for(int j = 0; j < _n_dig; j++) {
    _vram[j] = masks[num % 10];
    if(j == _dot_pos)
      _vram[j] |= 0x80;
    num /= 10;
  }
}
