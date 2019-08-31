#include "../include/browser.h"

#ifdef __cplusplus
Browser::Browser()
{}
////////////
Browser::~Browser()
{}
////////////
void Browser::open(const std::string& url) // Browser::open("https://stackoverflow.com");
{
#ifdef __windows__
	ShellExecute(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
#endif
#ifdef __gnu_linux__
	std::system(("xdg-open " + url).c_str());
#endif
}
////////////
#endif
