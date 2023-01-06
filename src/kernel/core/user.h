#ifndef USER_H
#define USER_H

#include <core/file.h>
#include <runtime/list.h>

enum {
  USER_ROOT,  //root
  USER_NORM  //utilisateur normal
};

class User : public File {
 public:
  User(const char* n);
  ~User();

  u32   open(u32 flag);
  u32   close();
  u32   read(u8* buffer, u32 size);
  u32   write(u8* buffer, u32 size);
  u32   ioctl(u32 id, u8* buffer);
  u32   remove();
  void  scan();

  void  password(const char *n);
  const char* password() const;
  
  User* getUNext();
  void  setUNext(User* us);
  
  void  type(u32 t);
  u32   type() const;

 protected:
  u32   type_;

  User* unext;
  char  password_[512];
};

#endif  // USER_H
