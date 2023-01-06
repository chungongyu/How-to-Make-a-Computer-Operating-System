#ifndef IO_H
#define IO_H

#include <runtime/types.h>

#define RAMSCREEN  0xB8000    /* debut de la memoire video */
#define SIZESCREEN 0xFA0      /* 4000, nombres d'octets d'une page texte */
#define SCREENLIM  0xB8FA0

/** Input/output class **/
class Io {
 public:
  Io();
  Io(u32 flag);

  /** standart io color **/
  enum Colour {
    Black       = 0,
    Blue        = 1,
    Green       = 2,
    Cyan        = 3,
    Red         = 4,
    Magenta     = 5,
    Orange      = 6,
    LightGrey   = 7,
    DarkGrey    = 8,
    LightBlue   = 9,
    LightGreen  = 10,
    LightCyan   = 11,
    LightRed    = 12,
    LightMagenta= 13,
    Yellow      = 14,
    White       = 15
  };

  /** io class functions **/
  void    outb(u32 ad,u8 v);      /* output byte */
  void    outw(u32 ad,u16 v);     /* output word */
  void    outl(u32 ad,u32 v);     /* output word */

  u8      inb(u32 ad);            /* input byte */
  u16     inw(u32 ad);            /* input word */
  u32     inl(u32 ad);            /* input word */

  void    putctty(char c);              /* put a byte on the console */

  u32     read(char* buf,u32 count);    /* read a string in the console */
  void    putc(char c);                 /* put a byte on screen */
  void    setColor(char fcol,char bcol);    /* change colors */
  void    clear();                      /* clear screen */
  void    print(const char *s, ...);    /* put a string in screen */

  void    cursor(u32 x, u32 y);         /* change cursor position */
  u32     cursorX() const;
  u32     cursorY() const;

  void    switchtty();                  /* change the io interface */    
  
  /** x86 functions **/
  void    scrollup(unsigned int n);
  void    save_screen();
  void    load_screen();
  
  enum ConsoleType {
    BUFFERED,
    GETCHAR
  };

  static Io*    current_io;
  static Io*    last_io;

 private:
  /** x86 private attributes **/
  char*   real_screen_;
  char    screen_[SIZESCREEN];
  
  char    inbuf_[512];        /* console buffer */
  int     keypos_;            /* console read position */
  int     inlock_;            /* console state */
  int     keystate_;          /* console type keyboard */
  
  char    fcolor_;            /* console foreground color */
  char    bcolor_;            /* console background color */
  u32     x_;                 /* console x position */
  u32     y_;                 /* console y position */
  char    kattr_;             /* console attribut */
  static char*    vidmem_;    /* screen video memory */
};

/** standart starting io interface **/
extern Io io;

#endif
