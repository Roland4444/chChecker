
#include "def.hpp"

class summer{
public:
	int count;
	void inc_and_print()
	{
		std::cout<< count++;
	}
	summer()
	{
		count = 0;
	}
};


class ContentInfo{
	uint8_t* content;
	uint64_t size;
};

class Checker
{
public:
	void* handles[2];
	Session* sessions[2];
	BKKCheck v_check;
	BKKCheck i_check;

	int checkFile_(Checker* self, char* filename)
	{
	  ContentInfo* ci = self->loadContent(filename);
	  if (ci == NULL)
	      return -1;
	  printf("SIZE CONTENT==%d\n\n\n", ci->size);
	  if (strstr(filename, "wav")!=NULL){
	    printf("CHECKING WAV FILE %s", filename);
	    if (!self->v_check(self->sessions[soundindex], ci->content, ci->size))
	    {
		delete(ci);
		printf("Check failed!\n");
		return -3;
	    }
	    else
	    {
	      printf("Checking passed\n");
	      delete(ci);
	      return 0;
	    }
	  }
	  printf("CHECKING photo FILE %s", filename);
	  if (!self->i_check(self->sessions[photoindex], ci->content, ci->size))
	  {
	      printf("Check failed!\n");
	      delete(ci);
	      return -3;
	  }
	  else
	  {
	      printf("Checking passed\n");
	      delete(ci);
	      return 0;
	  }
	  delete(ci);
	  return -9;

	}

	Checker()
	{
	  this->handles[soundindex] = dlopen(soundso, RTLD_LAZY);
	  this->handles[photoindex] = dlopen(photoso, RTLD_LAZY);
	  if (!this->handles[soundindex])
		    printf("error loading sound so");
	  else
		    printf("Load success sound so");

	  if (!this->handles[photoindex])
		    printf("error loading photo so");
		  else
		    printf("Load success photo so");

	  this->initSessions();
	  this->loadcheckers();
	  printf("\n\n\nVERSION===>\n\n%s\n\n\n\n",  getVersion());


	};
    private:

	int read_file_content(const char* file_path, uint8_t** content, size_t* content_size)
	{
	  FILE* fd = fopen(file_path, "rb");
	  if (fd == NULL)
	  {
	    fprintf(stderr, "file \"%s\" not found\n", file_path);
	    return 0;
	  }
	  fseek(fd, 0L, SEEK_END);
	  (*content_size) = (size_t) ftell(fd);
	  rewind(fd);
	  (*content) = (uint8_t*) calloc(1, (*content_size));
	  fread((*content), (*content_size), 1, fd);
	  fclose(fd);
	  return 1;
	}

	ContentInfo* loadContent(char * filename)
	{
	  ContentInfo* ci=new ContentInfo();
	  if (read_file_content(filename, &ci ->content, &ci->size))
	    return ci;
	  delete(ci);
	  return NULL;
	}

	char* getVersion()
	{
	  v_session_configuration_version get_sess_version = (v_session_configuration_version)(dlsym(self->handles[soundindex],"v_session_configuration_version"));
	  return get_sess_version(sessions[soundindex]);

	}

	void loadcheckers(){
	  this->i_check = (BKKCheck)(dlsym(handles[photoindex],"i_check_format"));
	  this->v_check = (BKKCheck)(dlsym(handles[soundindex],"v_check"));
	  if (!i_check)
	    printf("error load i_check");
	  if (!v_check)
	    printf("error load v_check");

	}

	Session* initSession(void* handle, char* symbol, char* config)
	{
	  Session* sess = (Session*)malloc(sizeof(Session));
	  create_session load = (create_session)(dlsym(handle, symbol));
	  if (!load){
	    printf("error loading %s\n\n\n", symbol );
	    sess=NULL;
	  }
	  sess->id="my_session";
	  if (!load(sess, config)){
	    printf("error create session\n\n");
	    sess=NULL;
	    return sess;
	  }
	  printf("Session creates succesfully\n\n");
	  printf("\n\nVERSION=>>\nMajor %d, minor %d, build %d\n\n\n", sess->version.major, sess->version.minor, sess->version.build);
	  return sess;
	}

	void initSessions()
	{
	  this->sessions[photoindex]=initSession(handles[photoindex], "i_create_session", photoconfig);
	  this->sessions[soundindex]=initSession(handles[soundindex], "v_create_session", soundconfig);
	  int i;
	  for (i=0;i<2; i++){
	    if (sessions[i]==NULL)
	      printf("Error create session #%d\n", i);
	  }


	}


};
summer* sum;
Checker* ca;
extern "C"
{
void init()
{
	sum = new summer();
	ca = new Checker();
}

void call()
{
	sum -> inc_and_print();
}

int check(ContentInfo * ci)
{
	return ca -> checkFile(ci);
}
}
