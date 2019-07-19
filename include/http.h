#ifndef _HTTP
#define _HTTP
#include "platform.h"

#ifdef __cplusplus
#include <iostream>

class HTTP { // deals with internet browser related stuff
public:
    HTTP();
	~HTTP();
	static void open(const std::string& url); // open a link in a browser
	static void display(const std::string& site); // displays a site in some sort of canvas
	
	std::string get();
};
#define HTTPS HTTP
#endif
#endif