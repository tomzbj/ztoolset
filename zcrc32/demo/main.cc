#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <sys/stat.h>

#define ZCRC32_LUT
#include "../zcrc32.h"

int main(int argc, char* argv[])
{
  if(argc != 2) {
    printf("Usage: %s test_file\n", argv[0]);
    exit(1);
  }

  FILE* f = fopen(argv[1], "rb");
  if(f == nullptr) {
    printf("Failed to open file %s.\n", argv[1]);
    exit(1);
  }
  struct stat st;
  fstat(fileno(f), &st);
  if(st.st_size > 100 * 1024LL * 1024LL) {
    printf("File too large.\n");
    exit(1);
  }
  auto buf = malloc(st.st_size);
  if(buf == nullptr) {
    printf("Failed to allocate memory.\n");
    exit(1);
  }
  fread(buf, st.st_size, 1, f);
  fclose(f);

  uint32_t crc = ~ZCRC32::calc(buf, st.st_size);

  printf("%08x\n", crc);

  return 0;
}
