#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <runtime/types.h>
#include <core/file.h>


class Filesystem {
 public:
  Filesystem();
  ~Filesystem();

  void  init();
  void  mknod(char* module,char* name,u32 flag);

  File* path(const char* p) const;
  File* path_parent(const char* p, char *fname) const;

  u32   add_file(const char* dir, File* fp);
  u32   link(const char* target, const char* name);

  File* root() const;
 private:
  File* root_;
  File* dev_;
  File* var_;
};

extern Filesystem fsm;

#endif  // FILESYSTEM_H
