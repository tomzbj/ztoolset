#ifndef __ZROMFS_H
#define __ZROMFS_H

#include <stdint.h>

// configurations
#define ENDIAN_LITTLE        // x86, avr, arm
//#define ZR_ENDIAN_BIG         // 51, stm8
#define MAX_VOLUMNS 2
#define MAX_OPENED_FILES 2

class ZROMFS {
  public:
    enum {
      OK = 0, NO_FILESYSTEM = -1, DISK_ERR = -2, FILE_NOT_FOUND = -3,
      DIR_NOT_FOUND = -4, NO_FILE = -5, NOT_A_DIR = -6,
      FILETYPE_NOT_SUPPORTED = -7, FILE_NOT_OPENED = -8,
      OPENED_FILE_EXCEED = -9, VOLUME_NOT_MOUNTED = -10, VOLUME_NUM_EXCEED = -11
    };
    enum {
      FTYPE_HARDLINK, FTYPE_DIR, FTYPE_REGULAR, FTYPE_SYMBOL_LINK,
      FTYPE_BLOCK_DEV, FTYPE_CHAR_DEV, FTYPE_SOCKET, FTYPE_FIFO
    };

    typedef struct {
        uint32_t start;
        void (*read_f)(uint32_t offset, void* buf, uint32_t size);
    } cfg_t;

    typedef struct {
        uint32_t offset;
    } dir_t;

    typedef struct {
        char fname[16];
        uint32_t fsize;
        uint32_t spec;
        uint32_t offset;
        uint32_t next;
        uint32_t ftype;
    } finfo_t;

    ZROMFS(const cfg_t& cfg) : cfg_(cfg)
    {
    }

    int mount(void);                                  // mount a volume
    int select_volume(int volume_id);           // select current volume
    int open(const char* path);                              // open a file
    int close(int fd);                            // close a opened file
    int read(int fd, void* buff, uint32_t nbytes);    // read data from a file
    int lseek(int fd, uint32_t offset, int seek_opt);    // move current read position to offset
    uint32_t tell(int fd);              // return current read position of fd
    int stat(const char* path, finfo_t* finfo);     // get file status
    int opendir(dir_t* dir, const char* path);     // open a directory
    int readdir(dir_t* dir, finfo_t* finfo);    // read a directory item

  private:

    typedef struct {
        uint32_t word0;
        uint32_t word1;
        uint32_t size;
        uint32_t checksum;
    } super_block_t;

    typedef struct {
        uint32_t next;
        uint32_t spec;
        uint32_t size;
        uint32_t checksum;
    } inode_t;

    const cfg_t& cfg_;
    struct {
        uint32_t size, curr_pos, offset;
    } fds_[MAX_OPENED_FILES + 3];     // keep 0, 1, 2
    uint32_t size_;

    uint32_t __skip_name(uint32_t offset);
    int __seek_fname(uint32_t offset, const char* path);
    int __find_free_fd(void);
    uint32_t __checksum(void);
};

#endif 
