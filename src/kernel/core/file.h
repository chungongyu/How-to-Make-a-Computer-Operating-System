#ifndef FILE_H
#define FILE_H

#include <runtime/types.h>

enum {
  TYPE_FILE,
  TYPE_DIRECTORY,
  TYPE_DEVICE,
  TYPE_PROCESS,
  TYPE_LINK
};

class File {
 public:
  File(const char* n, u8 t);
  ~File();
  
  virtual u32   open(u32 flag);
  virtual u32   close();
  virtual u32   read(u32 pos, u8* buffer, u32 size);
  virtual u32   write(u32 pos, u8* buffer, u32 size);
  virtual u32   ioctl(u32 id, u8* buffer);
  virtual u32   remove();
  virtual void  scan();
  
  void  checkName();
  
  File* create(const char* n, u8 t);
  u32   add(File* n);
  File* find(const char* n) const;
  u32   mmap(u32 sizee, u32 flags, u32 offset, u32 prot);
  
  void  name(char* n);
  void  size(u32 t);
  void  type(u8 t);
  void  parent(File* n);
  void  child(File* n);
  void  next(File* n);
  void  prev(File* n);
  void  link(File* n);
  
  const char* name() const;
  File*       parent() const;
  File*       child() const;
  File*       next() const;
  File*       prev() const;
  File*       link() const;
  u8          type() const;
  u32         size() const;
  u32         inode() const;
  
  stat_fs     stat() const;
 
 protected:
  static u32 inode_system;
  
  char*  map_memory;  /* to mmap */
  
  char*  name_;    /* Nom du fichier  */
  u32    size_;    /* Taille du fichier */
  u8     type_;    /* Type de fichier */
  u32    inode_;   /* Inode du fichier */
  File*  dev_;     /* the master device, example : /dev/hda */
  File*  link_;    /* the real file, if this file is a link */
  
  
  File*  master_;  /* processus maitre ou NULL */
  
  File*  parent_;
  File*  child_;
  File*  next_;
  File*  prev_;
  
  File*  device_;  /* This file is the device master of the current file */
};

#endif  // FILE_H
