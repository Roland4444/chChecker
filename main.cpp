
#include "def.hpp"

class ContentInfo{
public:
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

	int checkFileVoice(uint8_t* content,	uint64_t size)
	{
		std::cout<<"SIZE="<<size;
		return 0;
	}

	void lastErroeresult(int result)
	{
		printf("LAST ERROR RESULT = %d", result);
	};

	void loadresultSessions()
	{

	};

	int checkFile(char* filename)
	{
	  int result = 5555;
	  ContentInfo* ci = loadContent(filename);
	  if (ci == NULL)
	      return -1;
	  printf("SIZE CONTENT==%d\n\n\n", ci->size);
	  if (strstr(filename, "wav")!=NULL){
	    printf("CHECKING WAV FILE %s", filename);
	    if (!v_check(sessions[soundindex], ci->content, ci->size))
	     	printf("Check failed!\n");
		else
	      printf("Checking passed\n");
	   result = sessions[soundindex]->last_error;
	   lastErroeresult(result);
	   delete(ci);
	   return result;
	  }

	  printf("CHECKING photo FILE %s", filename);
	  if (!i_check(sessions[photoindex], ci->content, ci->size))
		  printf("Check failed!\n");
	  else
		  printf("Checking passed\n");
	  result = sessions[photoindex]->last_error;
	  lastErroeresult(result);
	  delete(ci);
	  return result;

	}

	Checker()
	{
	  printf("USING C++ Version OOP");
	  this->handles[soundindex] = dlopen(soundso, RTLD_LAZY);
	  this->handles[photoindex] = dlopen(photoso, RTLD_LAZY);
	  if (!this->handles[soundindex])
		    printf("\n\nerror loading sound so\n\n");
	  else
		    printf("\n\nLoad success sound so\n\n");

	  if (!this->handles[photoindex])
		    printf("\n\nerror loading photo so\n\n");
		  else
		    printf("\n\nLoad success photo so\n\n");

	  this->initSessions();
	  this->loadcheckers();
	  this->loadresultSessions();
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
	  v_session_configuration_version get_sess_version = (v_session_configuration_version)(dlsym(handles[soundindex],"v_session_configuration_version"));
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

int main(int argc, char* argv[])
{
  printf("USING C++ Version OOP");
  Checker* ca = new Checker();
  for (int i=1; i<argc; i++)
    ca->checkFile(argv[i]);

}

extern "C"
{
Checker* ca;

void init()
{
	ca = new Checker();
}

int check(char* filename)
{
	return ca -> checkFile(filename);
}
}
