
#include <os.h>



User::~User() {
}

User::User(const char* n) : File(n, TYPE_FILE) {
  fsm.add_file("/sys/usr/", this);
  unext=0;
  sys.addUserToList(this);
  type_ = USER_NORM;
  memset(password_, 0, 512);
}

u32 User::open(u32 flag) {
  return RETURN_OK;
}

u32 User::close() {
  return RETURN_OK;
}

u32 User::read(u8* buffer, u32 size) {
  return NOT_DEFINED;
}

u32 User::write(u8* buffer, u32 size) {
  return NOT_DEFINED;
}

u32 User::ioctl(u32 id, u8* buffer) {
  return NOT_DEFINED;
}

u32 User::remove() {
  delete this;
}

void User::scan() {

}

void User::password(const char* n) {
  if (n == NULL)
    return;
  memset(password_, 0, 512);
  strcpy(password_, n);
}

const char* User::password() const {
  if (password_[0] == '\0')
    return NULL;
  return password_;
}

User* User::getUNext() {
  return unext;
}

void User::setUNext(User* us) {
  unext=us;
}

void User::type(u32 t) {
  type_ = t;
}

u32 User::type() const {
  return type_;
}
