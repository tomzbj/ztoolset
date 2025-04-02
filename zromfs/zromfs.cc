#include "zromfs.h"

#include <stdio.h> // debug
#include <string.h>

#define __ftype(inode) (__le(inode.next) & 0x7)
#define __next(inode) (__le(inode.next) & (~0xf))

static uint32_t __le(uint32_t v)
{
#ifdef ENDIAN_LITTLE
  return (v >> 24) | (v << 24) | ((v >> 8) & 0xff00) | ((v << 8) & 0xff0000);
#else
  return v;
#endif
}

uint32_t ZROMFS::__checksum(void)
{
  uint32_t buf[128];
  uint32_t sum = 0;
  uint32_t chksum_size = size_ >= 512 ? 512 : size_;
  cfg_.read_f(cfg_.start, buf, chksum_size);
  for(uint32_t i = 0; i < chksum_size / 4; i++)
    sum += __le(buf[i]);
  return sum;
}

int ZROMFS::mount(void)
{
  super_block_t super;

  cfg_.read_f(cfg_.start, &super, sizeof(super));
  if(memcmp(&super, "-rom1fs-", 8) != 0)
    return NO_FILESYSTEM;
  size_ = __le(super.size);
  if(__checksum() != 0)
    return DISK_ERR;

  return OK;
}

uint32_t ZROMFS::__skip_name(uint32_t offset)
{
  char buf[16];
  do {
    cfg_.read_f(offset, buf, 16);
    offset += 16;
  } while(buf[15] != '\0');
  return offset;
}

int ZROMFS::__seek_fname(uint32_t offset, const char* path)
{
  while(path[0] == '/')
    path++;
  if(strlen(path) == 0)
    return offset;

  while(1) {
    inode_t inode;
    char fname[16];

    cfg_.read_f(offset, &inode, sizeof(inode));
    cfg_.read_f(offset + 16, fname, sizeof(fname));    // fname, only 16 chars available
    if(strcmp(path, fname) == 0) {    // path found
      if(__ftype(inode) == FTYPE_REGULAR || __ftype(inode) == FTYPE_DIR)
        return offset;
      else if(__ftype(inode) == FTYPE_HARDLINK) {
        return __le(inode.spec);
      }
      else
        return FILETYPE_NOT_SUPPORTED;
    }
    if(strstr(path, fname) == path && path[strlen(fname)] == '/') {

      if(__ftype(inode) == FTYPE_HARDLINK) {    //hard link to . or ..
        return FILETYPE_NOT_SUPPORTED;
      }
      else if(__ftype(inode) == FTYPE_DIR) {    //directory
        inode_t t;

        cfg_.read_f(offset, &t, sizeof(t));
        cfg_.read_f(offset + 16, fname, sizeof(fname));    // fname, only 16 chars available
        offset = __seek_fname(__le(t.spec) & ~0xf, path + strlen(fname) + 1);
        if(offset > 0)
          return offset;
      }
      else if(__ftype(inode) == FTYPE_SYMBOL_LINK) {    //symbolic link to a directory
        return FILETYPE_NOT_SUPPORTED;
      }
    }
    offset = __le(inode.next);
    offset &= ~0xf;
    if(offset == 0)
      return FILE_NOT_FOUND;    // not found
  }
}

int ZROMFS::opendir(ZROMFS::dir_t* dir, const char* path)
{
  int32_t offset;
  inode_t inode;

  dir->offset = cfg_.start + 16;
  dir->offset = __skip_name(dir->offset);
  offset = __seek_fname(dir->offset, path);
  if(offset == -1)
    return DIR_NOT_FOUND;
  cfg_.read_f(offset, &inode, sizeof(inode));

  if((__le(inode.next) & 0x7) != FTYPE_DIR)
    return NOT_A_DIR;

  dir->offset = __le(inode.spec);    //offset;
  return OK;
}

int ZROMFS::stat(const char* path, ZROMFS::finfo_t* finfo)
{
  inode_t inode;
  int32_t offset = cfg_.start + 16;
  offset = __skip_name(offset);
  offset = __seek_fname(offset, path);
  if(offset < 0)
    return FILE_NOT_FOUND;
  cfg_.read_f(offset, &inode, sizeof(inode));
  cfg_.read_f(offset + 16, finfo->fname, sizeof(finfo->fname));

  finfo->fsize = __le(inode.size);
  finfo->spec = __le(inode.spec);
  finfo->offset = offset;
  finfo->next = __le(inode.next) & (~0xf);
  finfo->ftype = __le(inode.next) & 0x7;

  return OK;
}

int ZROMFS::readdir(dir_t* dir, finfo_t* finfo)
{
  inode_t inode;

  if(dir->offset == cfg_.start)
    return NO_FILE;
  cfg_.read_f(dir->offset, &inode, sizeof(inode));
  cfg_.read_f(dir->offset + 16, finfo->fname, sizeof(finfo->fname));

  finfo->fsize = __le(inode.size);
  finfo->spec = __le(inode.spec);
  finfo->offset = dir->offset;
  finfo->next = __le(inode.next) & (~0xf);
  finfo->ftype = __le(inode.next) & 0x7;
  dir->offset = __le(inode.next) & (~0xf);

  return OK;
}

int ZROMFS::__find_free_fd(void)
{
  int i;
  for(i = 3; i < MAX_OPENED_FILES + 3; i++) {
    if(fds_[i].offset == 0)
      return i;
  }
  return OPENED_FILE_EXCEED;
}

int ZROMFS::open(const char* path)
{
  int fd;
  ZROMFS::finfo_t finfo;
  int ret = ZROMFS::stat(path, &finfo);
  if(ret != OK)
    return ret;

  fd = __find_free_fd();
  if(fd < 0)
    return fd;
  fds_[fd].size = finfo.fsize;
  fds_[fd].offset = finfo.offset + 16;
  fds_[fd].offset = __skip_name(fds_[fd].offset);
  fds_[fd].curr_pos = 0;
  return fd;    //skips system FDs
}

int ZROMFS::close(int fd)
{
  if(fds_[fd].offset == 0)
    return FILE_NOT_OPENED;
  fds_[fd].curr_pos = 0;
  fds_[fd].offset = 0;
  return OK;
}

int ZROMFS::read(int fd, void* buf, uint32_t nbytes)
{
  if(fds_[fd].offset == 0)
    return FILE_NOT_OPENED;

  uint32_t max_to_read = fds_[fd].size - fds_[fd].curr_pos;
  if(max_to_read < 0)
    return OK;
  if(max_to_read > nbytes)
    max_to_read = nbytes;
  cfg_.read_f(fds_[fd].offset + fds_[fd].curr_pos, buf, max_to_read);
  fds_[fd].curr_pos += max_to_read;

  return max_to_read;
}

uint32_t ZROMFS::tell(int fd)
{
  return fds_[fd].curr_pos;
}

int ZROMFS::lseek(int fd, uint32_t offset, int seek_opt)
{
  if(fds_[fd].size >= offset)
    fds_[fd].offset = fds_[fd].size - 1;
  else
    fds_[fd].curr_pos = offset;
  return OK;
}
