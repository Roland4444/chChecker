#include <iostream>
#include <dlfcn.h>
#ifndef DEF_HPP_
#define DEF_HPP_

using namespace std;

typedef struct{
	int major;
	int minor;
	int build;
} Version;

typedef struct{
	Version version;
	char *id;
	void *payment;
	int last_error;
} Session;

typedef int (*v_create_session)(Session*, char *);
typedef int (*BKKCheck)(Session *,  uint8_t * , uint64_t);


typedef struct{
	uint8_t* content;
	uint64_t sizecontent;
} ContentInfo;



#endif /* DEF_HPP_ */
