#include <os.h>
#include <runtime/buffer.h>

Buffer::Buffer(char* n,u32 siz) {
  map_ = (char *)kmalloc(siz);
  size_ = siz;
  memcpy(map_, n, siz);
}

Buffer::Buffer() {
  size_ = 0;
  map_ = NULL;
}

Buffer::~Buffer() {
  if (map_ != NULL)
    kfree(map_);
}

void Buffer::add(const u8* c, u32 s) {
  char* old = map_;
  map_ = (char *)kmalloc(size_ + s);
  memcpy(map_, old, size_);
  kfree(old);
  memcpy((char *)(map_ + size_), (char *)c, s);
  size_ += s;
}

u32 Buffer::get(u8* c, u32 s) {
  if (s > size_)
    s = size_;
  memcpy((char *)c, (char*)(map_ + (size_ - s)), s);
  char* old = map_;
  map_ = (char *)kmalloc(size_ - s);
  memcpy(map_, old, (size_ - s));
  kfree(old);
  size_ -= s;
  return s;
}

u32 Buffer::empty() const {
  return size_ == 0;
}

void Buffer::clear() {
  size_ = 0;
  if (map_ != NULL)
    kfree(map_);  
}

Buffer &Buffer::operator>>(char* c) {
  memcpy(c, map_, size_);
  return *this;
}
