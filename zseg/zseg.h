#ifndef _ZSEG_H
#define _ZSEG_H

#include <cstdint>
#include <cstring>
#include <memory>

class ZSEG {
  public:
    typedef struct {
        void (*set_seg_f)(int seg, int state);
        void (*set_dig_f)(int dig, int state);
    } cbs_t;

    ZSEG(const cbs_t& cfg, int n_seg, int n_dig)
      : _cbs(cfg), _n_seg(n_seg), _n_dig(n_dig)
    {
      _vram = std::make_unique<uint8_t[]>(_n_dig);
    }
    void poll(void);

    void set_dot(int pos)
    {
      _dot_pos = pos;
    }
    void write(uint8_t* raw);
    void write(int num);

  private:
    constexpr static uint8_t masks[10] = {    //
      0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f};
    const cbs_t& _cbs;
    const int _n_seg, _n_dig;
    int _num, _mode, _dot_pos, _curr_digit;
    std::unique_ptr<uint8_t[]> _vram;
};

#endif
