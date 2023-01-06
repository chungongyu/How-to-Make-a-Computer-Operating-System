#include <os.h>

Io* Io::last_io=&io;        /* definis la derniere io avant switch */
Io* Io::current_io=&io;        /* interface actuel (clavier redirigé vers celle ci) */

/* Video memory */
char* Io::vidmem_ = (char *)RAMSCREEN;

/* Constructor */
Io::Io() {
  real_screen_ = (char *)RAMSCREEN;
}

/* Destructor */
Io::Io(u32 flag) {
  real_screen_ = (char *)screen_;
}

/* output byte */
void Io::outb(u32 ad, u8 v) {
  asmv("outb %%al, %%dx" :: "d" (ad), "a" (v));;
}
/* output word */
void Io::outw(u32 ad, u16 v) {
  asmv("outw %%ax, %%dx" :: "d" (ad), "a" (v));
}
/* output word */
void Io::outl(u32 ad, u32 v) {
  asmv("outl %%eax, %%dx" : : "d" (ad), "a" (v));
}
/* input byte */
u8 Io::inb(u32 ad) {
  u8 _v;       \
  asmv("inb %%dx, %%al" : "=a" (_v) : "d" (ad)); \
  return _v;
}
/* input word */
u16 Io::inw(u32 ad) {
  u16 _v;            \
  asmv("inw %%dx, %%ax" : "=a" (_v) : "d" (ad));    \
  return _v;
}
/* input word */
u32 Io::inl(u32 ad){
  u32 _v;            \
  asmv("inl %%dx, %%eax" : "=a" (_v) : "d" (ad));    \
  return _v;
}

/* renvoie la position x du curseur */
u32 Io::cursorX() const {
  return x_;
}

/* renvoie la position y du curseur */
u32 Io::cursorY() const {
  return y_;
}

/* x86 scroll up screen */
void Io::scrollup(unsigned int n) {
  for (unsigned char* video = (unsigned char *)real_screen_;
      video < (unsigned char *)SCREENLIM; video += 2) {
    unsigned char* tmp = (unsigned char *) (video + n * 160);

    if (tmp < (unsigned char *)SCREENLIM) {
      *video = *tmp;
      *(video + 1) = *(tmp + 1);
    } else {
      *video = 0;
      *(video + 1) = 0x07;
    }
  }

  y_ -= n;
  if (y_ < 0) {
    y_ = 0;
  }
}

/* sauvegarde la memoire video */
void Io::save_screen(){
  memcpy(screen_, (char *)RAMSCREEN, SIZESCREEN);
  real_screen_ = (char *)screen_;
}

/* charge la memoire video */
void Io::load_screen(){
  memcpy((char *)RAMSCREEN, screen_, SIZESCREEN);
  real_screen_ = (char *)RAMSCREEN;
}

/* switch tty io */
void Io::switchtty() {
  if (current_io != this) {
    current_io->save_screen();
    load_screen();
    last_io = current_io;
    current_io = this;
  }
}

/* put a byte on screen */
void Io::putc(char c){
  kattr_ = 0x07;
  unsigned char* video = (unsigned char *)(real_screen_ + (x_ + 80 * y_) * 2);
  if (c == '\n') {
    x_ = 0;
    ++y_;
  } else if (c == '\b') {
    if (x_ > 0) {
      *(video + 1) = 0x0;
      --x_;
    }
  } else if (c == '\t') {
    x_ += 8 - x_ % 8;
  } else if (c == '\r') {
    x_ = 0;
  } else {
    *video = c;
    *(video + 1) = kattr_;

    ++x_;
    if (x_ > 79) {
      x_ = 0;
      ++y_;
    }
  }
  if (y_ > 24)
    scrollup(y_ - 24);
}

/* change colors */
void Io::setColor(char fcolor, char bcolor) {
  fcolor_ = fcolor;
  bcolor_ = bcolor;
}

/* change cursor position */
void Io::cursor(u32 x, u32 y) {
  x_ = x;
  y_ = y;
}

/* clear screen */
void Io::clear(){
  x_ = 0;
  y_ = 0;
  memset((char *)RAMSCREEN, 0, SIZESCREEN);
}

/* put a string in screen */
void Io::print(const char* s, ...) {
  va_list ap;

  char buf[16];
  int size, buflen, neg;

  unsigned char c;
  int ival;
  unsigned int uival;

  va_start(ap, s);

  while ((c = *s++)) {
      size = 0;
      neg = 0;

      if (c == '\0')
          break;
      else if (c == '%') {
          c = *s++;
          if (c >= '0' && c <= '9') {
            size = c - '0';
            c = *s++;
          }

          if (c == 'd') {
            ival = va_arg(ap, int);
            if (ival < 0) {
              uival = 0 - ival;
              neg++;
            } else
              uival = ival;
            itoa(buf, uival, 10);

            buflen = strlen(buf);
            if (buflen < size)
              for (int i = size, j = buflen; i >= 0; i--, j--)
                buf[i] = (j >= 0) ? buf[j] : '0';

            if (neg)
              print("-%s", buf);
            else
              print(buf);
          } else if (c == 'u') {
            uival = va_arg(ap, int);
            itoa(buf, uival, 10);

            buflen = strlen(buf);
            if (buflen < size)
              for (int i = size, j = buflen; i >= 0; i--, j--)
                buf[i] = (j >= 0) ? buf[j] : '0';

            print(buf);
          } else if (c == 'x' || c == 'X') {
            uival = va_arg(ap, int);
            itoa(buf, uival, 16);

            buflen = strlen(buf);
            if (buflen < size)
              for (int i = size, j = buflen; i >= 0; i--, j--)
                  buf[i] = (j >= 0) ? buf[j] : '0';

            print("0x%s", buf);
          } else if (c == 'p') {
              uival = va_arg(ap, int);
              itoa(buf, uival, 16);
              size = 8;

              buflen = strlen(buf);
              if (buflen < size)
                  for (int i = size, j = buflen; i >= 0;
                       i--, j--)
                      buf[i] =
                          (j >=
                           0) ? buf[j] : '0';

              print("0x%s", buf);
          } else if (c == 's') {
              print((char *) va_arg(ap, int));
          } 
      } else {
        putc(c);
      }
  }

  return;
}

/* put a byte on the console */
void Io::putctty(char c) {
  if (keystate_ == BUFFERED) {
    if (c == 8) {        /* backspace */
      if (keypos_ > 0) {
        inbuf_[keypos_--] = 0;
      }
    }
    else if (c == 10) {    /* newline */
      inbuf_[keypos_++] = c;
      inbuf_[keypos_] = 0; 
      inlock_ = 0;
      keypos_ = 0;
    }
    else {
      inbuf_[keypos_++] = c; 
    }
  }
  else if (keystate_ == GETCHAR) {
    inbuf_[0] = c;
    inbuf_[1] = 0;
    inlock_ = 0;
    keypos_ = 0;
  }
}

/* read a string in the console */
u32 Io::read(char* buf, u32 count) {
  if (count > 1) {
    keystate_ = BUFFERED;
  } else {    //getchar
    keystate_ = GETCHAR;
  }
  asm("sti");
  inlock_ = 1;
  while (inlock_ == 1);
  asm("cli");
  strncpy(buf, inbuf_, count);
  return strlen(buf);
}
