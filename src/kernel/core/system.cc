
#include <os.h>

/**
 *	10/04/06 (Samy Pessé) : creation du fichier
 *							ajout gestion login et variable d'environnement
 *	10/04/07 (Samy Pessé) : la classe System gere maintenant une liste chainé des utilisateur (User)
 **/
 


/*
Cette classe organise le systeme en lui meme : utilisateur, variable, ...
*/

System::System(){
	
}

System::~System(){

}


void System::init(){
	var=fsm.path("/sys/env/");

	/** System user **/
	root=new User("root");
	root->type(USER_ROOT);
	
	actual=new User("liveuser");

	
	/** Environnement variable **/
	uservar=new Variable("USER","liveuser");
	new Variable("OS_NAME",KERNEL_NAME);
	new Variable("OS_VERSION",KERNEL_VERSION);
	new Variable("OS_DATE",KERNEL_DATE);
	new Variable("OS_TIME",KERNEL_TIME);
	new Variable("OS_LICENCE",KERNEL_LICENCE);
	new Variable("COMPUTERNAME",KERNEL_COMPUTERNAME);
	new Variable("PROCESSOR_IDENTIFIER",KERNEL_PROCESSOR_IDENTIFIER);
	new Variable("PROCESSOR_NAME",arch.detect());
	new Variable("PATH","/bin/");
	new Variable("SHELL","/bin/sh");
}

//fonction de login
int	System::login(User* us,char* pass){
	if (us==NULL)
		return ERROR_PARAM;
	if (us->password() != NULL){	//si il ya un password
		
		if (pass==NULL)	//si on a passé un code
			return PARAM_NULL;
			//
		if (strncmp(pass,us->password(),strlen(us->password())))	//test password
			return RETURN_FAILURE;
		//io.print("login %s with %s (%s)\n",us->name(),pass,us->password());
	}
		
	uservar->write(0,(u8*)us->name(),strlen(us->name()));
	actual=us;
	return RETURN_OK;
}

/* ne pas oubliez de faire un free de la variable */
char* System::getvar(char* name){
	char* varin;
	File* temp=var->find(name);
	if (temp==NULL){
		return NULL;
	}
	varin = (char *)kmalloc(temp->size());
	memset(varin, 0, temp->size());
	temp->read(0, (u8*)varin, temp->size());
	return varin;
}

User* System::getUser(char* nae){
	User* us=listuser;
	while (us!=NULL){
		//io.print("test '%s' with '%s'\n",nae,us->name());
		if (!strncmp(nae,us->name(),strlen(us->name())))
			return us;
		us=us->getUNext();
	}
	return NULL;
}

void System::addUserToList(User* us){
	if (us==NULL)
		return;
	us->setUNext(listuser);
	listuser=us;
}

u32 System::isRoot(){
	if (actual!=NULL){
		if (actual->type() == USER_ROOT)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}
