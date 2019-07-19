#ifndef _PROCESS
#define _PROCESS
#include "platform.h"
#include "utility.h"
#include "logger.h"

#ifdef __cplusplus
#include <iostream>

class Process
{
public:
    Process();
	Process(const std::string& program, const std::string& argument);
	~Process();
	
    void * open();
	bool create(const std::string& program, const std::string& argument);
	bool terminate();
	static bool terminate(Process * process);
	void exit(int code = 0);
	
	void * get_handle();
	static void * get_active();
private:
    void * handle;
}; // Process process("glue.exe", " srlua.exe test.lua test.exe");
#endif
#endif