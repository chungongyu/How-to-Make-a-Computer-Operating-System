#ifndef BUFFER_H
#define BUFFER_H

class Buffer {
 public:
  Buffer(char* n, u32 siz);
  Buffer();
  ~Buffer();
  
  void  add(const u8* c, u32 s);
  u32   get(u8* c, u32 s);
  void  clear();
  u32   empty() const;
  
  
  Buffer &operator>>(char* c);
  
 private:
  u32    size_;
  char*  map_;
};

#endif  // BUFFER_H
