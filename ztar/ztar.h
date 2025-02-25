/* Author: tomzbj  Updated: 2024-04-18 Rev01 */

#ifndef _ZTAR_H
#define _ZTAR_H

#include <cstdint>
#include <cstring>
#include <tuple>

class ZTAR {
  public:
    typedef struct {
        int (*read_f)(uint32_t addr, uint32_t size, void* buf);
        uint32_t start_addr, size;
    } cfg_t;

    ZTAR(const cfg_t& cfg) : cfg_(cfg)
    {
    }
    std::tuple<uint32_t, uint32_t> find(const char* fname)
    {
      struct {
          char name[100];
          char reserved_1[24];
          char size[12];
      } header;

      uint32_t addr = cfg_.start_addr, size;
      while(addr - cfg_.start_addr < cfg_.size) {
        cfg_.read_f(addr, sizeof(header), &header);
        addr += 512;    //sizeof(header);
        if(strlen(header.name) == 0)
          return std::tuple<uint32_t, uint32_t>(-1, -1);
        sscanf(header.size, "%lo", (unsigned long*)&size);
        if(strncmp(header.name, fname, sizeof(header.name)) == 0) {
          return std::tuple<uint32_t, uint32_t>(addr, size);
        }
        if(size % 512 != 0)
          size += (512 - (size % 512));
        addr += size;
      }
      return std::tuple<uint32_t, uint32_t>(-1, -1);
    }

  private:

    const cfg_t& cfg_;
};

#endif
