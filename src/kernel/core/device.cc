#include <os.h>

Device::Device(const char* n) : File(n,TYPE_DEVICE) {
  fsm.add_file("/dev", this);
}

Device::~Device() {
}

u32 Device::open(u32 flag) {
  return NOT_DEFINED;
}

u32 Device::close() {
  return NOT_DEFINED;
}

u32 Device::read(u8* buffer, u32 size) {
  return NOT_DEFINED;
}

u32 Device::write(u8* buffer, u32 size) {
  return NOT_DEFINED;
}

u32 Device::ioctl(u32 id, u8* buffer) {
  return NOT_DEFINED;
}

u32 Device::remove() {
  delete this;
}

void Device::scan() {
}
