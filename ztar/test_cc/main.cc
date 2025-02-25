#include <cstdio>
#include <cstdlib>
#include "../ztar.h"
#include <sys/stat.h>

static int ztar_read(uint32_t addr, uint32_t size, void* buf);

static ZTAR::cfg_t ztar_cfg = {.read_f = ztar_read, .start_addr = 0, .size = 524288};

uint8_t file_buf[1024000UL];
ZTAR tar(ztar_cfg);

static int ztar_read(uint32_t addr, uint32_t size, void* buf)
{
  memcpy(buf, file_buf + addr, size);
  return size;
}

int main(int argc, char** argv)
{
  if(argc != 2)
    exit(1);

  FILE* f = fopen(argv[1], "rb");
  struct stat st;
  stat(argv[1], &st);
  printf("file size: %lu\n", st.st_size);
  fread(file_buf, st.st_size, 1, f);
  fclose(f);

  const auto& [addr, size] = tar.find("app.exe");
  printf("addr: %u  size: %u\n", addr, size);

  return 0;
}
