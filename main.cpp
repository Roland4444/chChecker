
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
summer* sum;

class Checker_Audio
{
	public:
		Session * session;
		int ResultCreating;
		void * handle;
        char * config = "./cv_configuration.json";

        int checkFile(ContentInfo * ci){
          BKKCheck v_check = (BKKCheck)(dlsym(handle,"v_check"));
          if (!v_check)
        	  return -2;

          if (!v_check(this->session, ci ->content, ci ->sizecontent)){
        	  std::cout<<"Check failed!";
              return -3;
          }
          else return 0;
        }


		int checkFile(char * filename){
		    std::cout<<"\n\nchecking =>"<<filename<<endl;
		    ContentInfo * ci;
			if (loadContent(filename) == NULL)
                return -1;
            ci = loadContent(filename);
            BKKCheck v_check = (BKKCheck)(dlsym(handle,"v_check"));
            if (!v_check)
                return -2;

            if (!v_check(this->session, ci ->content, ci ->sizecontent)){
                std::cout<<"Check failed!";
                return -3;
            }
            else return 0;
		}

		Checker_Audio()
		{
			this -> session = (Session*)malloc(sizeof(Session));
			std::cout<<"Loads "<<sizeof(Session)<<"  mamory\n";
			this -> handle = dlopen("./libcv.so", RTLD_LAZY);
			ResultCreating=0;
			if (handle == NULL)
			{
				std::cout<<"\n\nerror loading\n\n\n";
				ResultCreating = -1;
			}
			initSession();
		};
    private:
        ContentInfo * loadContent(char * filename){
            ContentInfo * ci=new ContentInfo;
            read_file_content(filename, &ci ->content, &ci->sizecontent);
            return ci;
        }
        void initSession(){
            Session * session = (Session*)malloc(sizeof(Session));
            v_create_session load = (v_create_session)(dlsym(this->handle, "v_create_session"));
            if (!load){
                std::cout<<"error loading v_create_session";
                this->session=NULL;
            }
            session->id="my_session";
            if (!load(session, config)){
                std::cout<<"error create session";
                this->session=NULL;
                return;
            }
            std::cout<<"Session creates succesfully";
            this->session=session;
        }

        bool read_file_content(const char *file_path, uint8_t **content, size_t *content_size) {
            FILE *fd = fopen(file_path, "rb");
            if (fd == NULL) {
                fprintf(stderr, "file \"%s\" not found\n", file_path);
                return false;
            }
            fseek(fd, 0L, SEEK_END);
            (*content_size) = (size_t) ftell(fd);
            rewind(fd);
            (*content) = (uint8_t *) calloc(1, (*content_size));
            fread((*content), (*content_size), 1, fd);
            fclose(fd);
            return true;
        }

};

extern "C"
{
void init()
{
	sum = new summer();
}

void call()
{
	sum -> inc_and_print();
}
}

