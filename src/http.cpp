#include "../include/http.h"

#ifdef __cplusplus
HTTP::HTTP()
{}
////////////
HTTP::~HTTP()
{}
////////////
void HTTP::open(const std::string& url) // HTTP::open("http://facebook.com"); 
{
	#ifdef __windows__
	ShellExecute(NULL, "open", url.c_str(),
            NULL, NULL, SW_SHOWNORMAL);
	#endif		
	
	#ifdef __gnu_linux__
	    std::string browser = "firefox ";
	    system(std::string(browser + url).c_str() );
	#endif
}
////////////
#endif