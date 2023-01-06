#include <os.h>

/* dans myos quasiment tous herite de cette classe */


/*
 *  Remplace dans s tous les a par to
 */
static void strreplace(char* s, char a, char to) {
  if (s == NULL)
    return;
  while (*s) {
    if (*s==a) {
      *s=to;
    }
    s++;
  }
}

u32  File::inode_system = 0;  /* numero d'inode de depart */

/* constructeur */
File::File(const char* n, u8 t) {
  name_ = (char*)kmalloc(strlen(n) + 1);
  memset(name_, 0, strlen(n));
  memcpy(name_, n, strlen(n));
  name_[strlen(n)]=0;
  
  checkName();
  master_ = arch.pcurrent;  //à la creation, le maitre est le processus courant
  inode_ = inode_system;
  inode_system++;
  size_ = 0;
  type_ = t;
  parent_ = NULL;
  child_ = NULL;
  next_ = NULL;
  prev_ = NULL;
  link_ = NULL;
  map_memory = NULL;
}

/* destructeur */
File::~File() {
  kfree(name_);
  
  //on modifie la liste des frere
  
  if (prev_ == NULL){
    parent_->child(next_);
    next_->prev(NULL);
  }
  else if (next_ == NULL) {
    prev_->next(NULL);
  } else if (next_ == NULL && prev_ == NULL) {
    parent_->child(NULL);
  } else {
    io.print("prec (%s) next is now %s\n",prev_->name(),next_->name());
    io.print("next (%s) prec is now %s\n",next_->name(),prev_->name());
    prev_->next(next_);
    next_->prev(prev_);
  }
  
  //on supprime les enfant (dossier)
  File* n = child_;
  File* nn = NULL;
  while (n != NULL) {
    //io.print("delete %s \n",n->name());
    nn = n->next();
    delete n;
    n = nn;
  }
}

#define CAR_REPLACE '_'


void File::checkName(){
  //Adapte le nom
  strreplace(name_, '/', CAR_REPLACE);
  strreplace(name_, '\ ', CAR_REPLACE);
  strreplace(name_, '?', CAR_REPLACE);
  strreplace(name_, ':', CAR_REPLACE);
  strreplace(name_, '>', CAR_REPLACE);
  strreplace(name_, '<', CAR_REPLACE);
  strreplace(name_, '*', CAR_REPLACE);
  strreplace(name_, '"', CAR_REPLACE);
  strreplace(name_, ':', CAR_REPLACE);
}

u32 File::add(File* n) {
  if (!n) {
    return PARAM_NULL;
  }
  n->parent(this);
  n->prev(NULL);
  n->next(child_);
  if (child_ != NULL)
    child_->prev(n);
  child_ = n;
  return RETURN_OK;
}

File* File::create(const char* n, u8 t) {
  File* fp = new File(n, t);
  this->add(fp);
  return fp;
}

File* File::parent() const {
  return parent_;
}

File* File::child() const {
  return child_;
}

File* File::next() const {
  return next_;
}

File* File::prev() const {
  return prev_;
}

File* File::link() const {
  return link_;
}

u32  File::size() const {
  return size_;
}

u32 File::inode() const {
  return inode_;
}

void File::scan(){

}

void File::type(u8 t){
  type_ = t;
}

void File::size(u32 t) {
  size_ = t;
}

void File::parent(File* n){
  parent_ = n;
}

void File::link(File* n){
  link_ = n;
}

void File::child(File* n){
  child_ = n;
}

void File::next(File* n){
  next_ = n;
}

void File::prev(File* n){
  prev_ = n;
}

void File::name(char* n) {
  kfree(name_);
  name_=(char *)kmalloc(strlen(n));
  memcpy(name_, n, strlen(n));
  checkName();
}

u8 File::type() const {
  return type_;
}

const char* File::name() const {
  return name_;
}

File* File::find(const char* n) const {
  File* fp = child_;
  while (fp != NULL) {
    if (!strcmp(fp->name(), n))
      return fp;
    fp = fp->next_;
  }
  return NULL;
}

u32  File::open(u32 flag){
  return NOT_DEFINED;
}

u32  File::close(){
  return NOT_DEFINED;
}

u32  File::read(u32 pos,u8* buffer,u32 size){
  return NOT_DEFINED;
}

u32  File::write(u32 pos,u8* buffer,u32 size){
  return NOT_DEFINED;
}

u32  File::ioctl(u32 id,u8* buffer){
  return NOT_DEFINED;
}

u32  File::remove(){
  delete this;
  return NOT_DEFINED;
}

stat_fs File::stat() const {
  stat_fs st;
  return st;
}

u32 File::mmap(u32 sizee, u32 flags, u32 offset, u32 prot) {
  if (map_memory != NULL) {
    int i = 0;
    unsigned int adress;
    struct page *pg;
    process_st* current=(arch.pcurrent)->getPInfo();
    for (i=0;i<sizee;i++){
        adress=(unsigned int)(map_memory+i*PAGESIZE);
        //io.print("mmap : %x %d\n",adress,sizee);
        pg = (struct page *) kmalloc(sizeof(struct page));
        pg->p_addr = (char*) (adress);
        pg->v_addr = (char *) (adress & 0xFFFFF000);
        list_add(&pg->list, &current->pglist);
        page_directory_add_page(pg->v_addr, pg->p_addr, PG_USER, current->pd);
    }
    return (u32)map_memory;
  }
  return -1;
}
