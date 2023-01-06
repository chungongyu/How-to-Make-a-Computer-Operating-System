
#include <os.h>
#include <api/dev/proc.h>

/* Definis un process (/dev/proc) */

char* Process::default_tty = "/dev/tty";

u32 Process::proc_pid = 0;

Process::Process(const char* n) : File(n, TYPE_PROCESS) {
  fsm.add_file("/proc/", this);

  pparent=arch.pcurrent;
  pid_ = proc_pid++;
  if (pparent!=NULL)
    cwd_ = pparent->cwd();
  else
    cwd_ = fsm.root();
    
  arch.add_process(this);
  info.vinfo=(void*)this;
  for (int i = 0; i < CONFIG_MAX_FILE; ++i) {  //open files
    openfp_[i].fp = NULL;
  }
  ipc_ = new Buffer();  //ipc buffer
}

Process::~Process() {
  delete ipc_;
  arch.change_process_father(this,pparent);  //on change le pere des enfants  
}

u32 Process::open(u32 flag) {
  return RETURN_OK;
}

u32 Process::close() {
  return RETURN_OK;
}

Process* Process::getPParent() {
  return pparent;
}

void Process::setPParent(Process* p) {
  pparent=p;
}

u32 Process::read(u32 pos, u8* buffer, u32 sizee) {
  u32 ret=RETURN_OK;
  arch.enable_interrupt();
  while (ipc_->empty());
  ret=ipc_->get(buffer,sizee);
  
  arch.disable_interrupt();
  return ret;
}

u32 Process::write(u32 pos, u8* buffer, u32 sizee) {
  ipc_->add(buffer,sizee);
  return size_;
}

u32 Process::ioctl(u32 id, u8* buffer) {
  u32 ret;
  switch (id){
    case API_PROC_GET_PID:
      ret = pid_;
      break;
      
    case API_PROC_GET_INFO:
      reset_pinfo();
      memcpy((char*)buffer,(char*)&ppinfo,sizeof(proc_info));
      break;
      
      
    default:
      ret=NOT_DEFINED;
      break;
  }
  
  return ret;
}

int  Process::fork() {
  /*Process* p=new Process("fork_child");
  return arch.fork(p->getPInfo(),&info);*/
  if (pparent!=NULL)
    pparent->sendSignal(SIGCHLD);  
  return 0;
}

u32  Process::wait() {
  arch.enable_interrupt();
  while (is_signal(info.signal, SIGCHLD) == 0);
  clear_signal(&(info.signal), SIGCHLD);
  arch.destroy_all_zombie();
  arch.disable_interrupt();
  return 1;
}

u32 Process::remove() {
  delete this;
  return RETURN_OK;
}

void Process::scan() {

}

void Process::exit() {
  status(ZOMBIE);
  if (pparent!=NULL)
    pparent->sendSignal(SIGCHLD);
}

void Process::setPNext(Process* p){
  pnext=p;
}

process_st* Process::getPInfo(){
  return &info;
}

Process* Process::getPNext(){
  return pnext;
}

u32 Process::create(char* file, int argc, char **argv){
  int ret=arch.create_process(&info,file,argc,argv);
  if (ret == 1)
    status(CHILD);
  else
    status(ZOMBIE);
    
  //stdin stdout et stderr du parent
  if (pparent!=NULL){
    memcpy((char*)&openfp_[0],(char*)pparent->getFileInfo(0),sizeof(openfile));
    memcpy((char*)&openfp_[1],(char*)pparent->getFileInfo(1),sizeof(openfile));
    memcpy((char*)&openfp_[2],(char*)pparent->getFileInfo(2),sizeof(openfile));
    addFile(this,0);
  }
  else{
    addFile(fsm.path(default_tty),0);
    addFile(fsm.path(default_tty),0);
    addFile(fsm.path(default_tty),0);
    
  }
  
  return RETURN_OK;
}

void Process::setFile(u32 fd,File* fp,u32 ptr, u32 mode){
  if (fd<0 || fd>CONFIG_MAX_FILE)
    return;
  openfp_[fd].fp=fp;
  openfp_[fd].ptr=ptr;
  openfp_[fd].mode=mode;
}

u32 Process::addFile(File* f,u32 m){
  int i;
  for (i=0;i<CONFIG_MAX_FILE;i++){
    if (openfp_[i].fp==NULL && f!=NULL){
      //io.print("%s:  add %s in %d\n",name,f->getName(),i);
      openfp_[i].fp=f;
      openfp_[i].mode=m;
      openfp_[i].ptr=0;
      return i;
    }
  }
}

File* Process::getFile(u32 fd){
  if (fd<0 || fd>CONFIG_MAX_FILE)
    return NULL;
  return openfp_[fd].fp;
}

openfile* Process::getFileInfo(u32 fd){
  if (fd<0 || fd>CONFIG_MAX_FILE)
    return NULL;
  return &openfp_[fd];
}

void Process::deleteFile(u32 fd){
  if (fd<0 || fd>CONFIG_MAX_FILE)
    return;
  openfp_[fd].fp=NULL;
  openfp_[fd].mode=0;
  openfp_[fd].ptr=0;
}

Process* Process::schedule(){
  Process* n=this;
  int out=1;
  n=n->getPNext();
  while (out){
    
    if (n==NULL){
      n=arch.plist;
    }
    //io.print("testing %s\n",n->getName());
    
    
    if (n->status() != ZOMBIE) {
      out=0;
    }
    else{
      n=n->getPNext();
    }
    
  }
  
  arch.pcurrent=n;
  
  return n;
}


File* Process::cwd() const {
  return cwd_;
}

void Process::cwd(File* f) {
  cwd_ = f;
}

void Process::status(u8 st) {
  state_ = st;
}

u8  Process::status() const {
  return state_;
}


void Process::pid(u32 st) {
  pid_ = st;
}

u32  Process::pid() const {
  return pid_;
}

void Process::sendSignal(int sig){
  set_signal(&(info.signal),sig);
}

void Process::reset_pinfo() {
  strncpy(ppinfo.name, name_, 32);
  ppinfo.pid = pid_;
  ppinfo.tid = 0;
  ppinfo.state = state_;
  ppinfo.vmem = 10*1024*1024;
  ppinfo.pmem = 10*1024*1024;
}
