
#include <iostream>

class summer{
public:
	int count = 0;
	void inc_and_print(){
		std::cout<< count++;
	}
};


extern "C"{

	summer* sum;
	void init(){
	sum = new summer { };
	}

}
