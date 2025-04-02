#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../zromfs.h"
#include "cli.h"

FILE* f;

void read_func(uint32_t offset, void* buf, uint32_t size)
{
  fseek(f, offset, SEEK_SET);
  fread(buf, size, 1, f);
}

static ZROMFS::cfg_t cfg = {.start = 0, .read_f = read_func};
ZROMFS fs(cfg);

int main(int argc, char* argv[])
{
  if(argc != 2) {
    puts("Usage: cli img_file");
    exit(1);
  }
  f = fopen(argv[1], "rb");

  int ret = fs.mount();
  printf("%d\n", ret);
  if(ret < 0)
    exit(1);

  while(1) {
    char buf[256] = {0};
    CLI_Prompt();
    fgets(buf, 254, stdin);

    if(strcasecmp(buf, "exit\n") == 0)
      break;
    CLI_Parse(buf, 254, 0);
  }
  fclose(f);
  return 0;
}
