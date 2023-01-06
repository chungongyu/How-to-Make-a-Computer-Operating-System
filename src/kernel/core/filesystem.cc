#include <os.h>

/*
Cette classe organise les fichiers entre eux
*/

Filesystem::Filesystem() {
}

void Filesystem::init() {
  root_ = new File("/", TYPE_DIRECTORY);
  
  dev_ = root_->create("dev", TYPE_DIRECTORY);  //dossier contenant les peripherique
  root_->create("proc", TYPE_DIRECTORY);        //dossier contenant les processus tournant
  root_->create("mnt", TYPE_DIRECTORY);         //dossier contenant les points de montages des disques
  File* sysd = root_->create("sys", TYPE_DIRECTORY);  //dossier contenant toutes les infos du systemes
  var_ = sysd->create("env", TYPE_DIRECTORY);   //dossier contenant toutes les variables d'environnement
  sysd->create("usr", TYPE_DIRECTORY);          //dossier contenant tous les utilisateurs
  sysd->create("mods", TYPE_DIRECTORY);         //dossier contenant tous les modules disponiles
  sysd->create("sockets", TYPE_DIRECTORY);      //dossier contenant tous les sockets actuels
}

Filesystem::~Filesystem() {
  delete root_;
}

void Filesystem::mknod(char* module, char* name, u32 flag){
  modm.createDevice(name, module, flag);
}

File* Filesystem::root() const {
  return root_;
}

File* Filesystem::path(const char* pathname) const {
  if (!pathname)
    return NULL;
    
  File* fp = root_;
  if (pathname[0] != '/') {
    if (arch.pcurrent != NULL) {    /* prend de le dossier actuel du fichier */
      fp = arch.pcurrent->cwd();
    }
  }

  char* name = NULL;
  const char* p = pathname;
  while (*p == '/')
    ++p;
  const char* q = p + 1;
  
  while (*p != '\0') {
    if (fp->type() != TYPE_DIRECTORY){
      return NULL;
    }
    while (*q != '\0' && *q != '/')
      ++q;
    name = (char *)kmalloc(q - p + 1);
    memcpy(name, p, q - p);
    name[q - p] = '\0';

    if (strcmp("..", name) == 0) {    // '..' 
      fp = fp->parent();
    } else if (strcmp(".", name) == 0) {  // '.' 
    
    } else {
      fp->scan();
      if (!(fp = fp->find(name))) {
        kfree(name);
        return NULL;
      }
      
      if (fp->type() == TYPE_LINK && (fp->link() != NULL)) {
        fp = fp->link();
      }
    }

    p = q;
    while (*p == '/')
      ++p;
    q = p + 1;

    kfree(name);
  }
  
  return fp;
}

File* Filesystem::path_parent(const char* p, char *fname) const {
  if (!p)
    return NULL;

  File* ofp;
  char* name;
  
  File* fp = root_;
  if (p[0] == '/')
    fp = root_;
    
  const char* beg_p = p;
  while (*beg_p == '/')
    beg_p++;
  const char* end_p = beg_p + 1;
  
  while (*beg_p != 0) {
    if (fp->type() != TYPE_DIRECTORY)
      return NULL;

    while (*end_p != 0 && *end_p != '/')
      end_p++;
    name = (char *) kmalloc(end_p - beg_p + 1);
    memcpy(name, beg_p, end_p - beg_p);
    name[end_p - beg_p] = 0;


    if (strcmp("..", name) == 0) {    // '..' 
      fp = fp->parent();
    } else if (strcmp(".", name) == 0) {  // '.' 
    
    } else {
      ofp=fp;
      
      if (fp->type()==TYPE_LINK && (fp->link()!=NULL)){
        fp=fp->link();
      }
      
      if (!(fp = fp->find(name))) {
        strcpy(fname,name);
        kfree(name);
        return ofp;
      }
    
    }

    beg_p = end_p;
    while (*beg_p == '/')
      beg_p++;
    end_p = beg_p + 1;

    kfree(name);
  }
  
  return fp;
}

u32 Filesystem::link(const char* target_name, const char* link_name) {
  File* tolink = path(target_name);
  if (tolink == NULL)
    return -1;

  char* nname = (char *)kmalloc(255);
  File* par = path_parent(link_name, nname);
  File* fp = new File(nname, TYPE_LINK);
  fp->link(tolink);
  par->add(fp);
  return RETURN_OK;
}

u32 Filesystem::add_file(const char* dir, File* fp) {
  File* fdir=path(dir);
  if (fdir == NULL)
    return ERROR_PARAM;
  return fdir->add(fp);
}
