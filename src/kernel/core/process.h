#ifndef PROC_H
#define PROC_H

#include <core/file.h>
#include <runtime/list.h>
#include <archprocess.h>  /* definition de process_st */

#include <core/signal.h>

#include <runtime/buffer.h>

#include <api/dev/proc.h>

#define ZOMBIE  PROC_STATE_ZOMBIE
#define CHILD  PROC_STATE_RUN

struct openfile {
  u32        mode;  /* Mode d'ouverture */
  u32        ptr;  /* Pointeur de lecture/ecriture */
  File*      fp;    /* Fichier ouvert */
};

class Process : public File {
 public:
  Process(const char* n);
  ~Process();

  u32   open(u32 flag);
  u32   close();
  u32   read(u32 pos, u8* buffer, u32 size);
  u32   write(u32 pos, u8* buffer, u32 size);
  u32   ioctl(u32 id, u8* buffer);
  u32   remove();
  void  scan();

  u32   create(char* file, int argc, char **argv);
  void  sendSignal(int sig);
  u32   wait();

  u32   addFile(File* fp,u32 m);
  File* getFile(u32 fd);
  void  deleteFile(u32 fd);
  openfile*  getFileInfo(u32 fd);

  void  exit();
  int   fork();

  u8    status() const;
  void  status(u8 st);
  void  setFile(u32 fd,File* fp,u32 ptr, u32 mode);
  u32   pid() const;
  void  pid(u32 st);

  void  setPNext(Process* p);

  Process*    schedule();
  Process*    getPNext();
  Process*    getPParent();
  process_st* getPInfo();
  void      setPParent(Process*p);

  void      reset_pinfo();

  process_st  info;

  File*  cwd() const;
  void   cwd(File* f);

 protected:
  static u32  proc_pid;
  
  u32       pid_;
  u8        state_;
  Process*  pparent;
  Process*  pnext;
  openfile  openfp_[CONFIG_MAX_FILE];
  proc_info ppinfo;
  File*     cwd_;
  
  Buffer*   ipc_;
  
  static char*  default_tty;
};

#endif  // PROC_H
