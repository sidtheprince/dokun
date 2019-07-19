#include "../include/process.h"

Process::Process()
{}
Process::Process(const std::string& program, const std::string& arg)
{
    if(!create(program, arg))
	{
		Logger("Process creation failed");
	}
}
Process::~Process()
{
	terminate();
}
	
void * Process::open()
{
#ifdef __windows__
	this->handle = static_cast<void *>(OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId()));
	return this->handle;
#endif	
#ifdef __gnu_linux__
#endif
}
bool Process::create(const std::string& program, const std::string& argument)
{
#ifdef __windows__
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory (&si, sizeof(si));
    si.cb = sizeof (si);
    ZeroMemory (&pi, sizeof(pi));
       // static_cast<LPSTR>(const_cast<char *>(program.c_str()))
	std::string cmd = program + argument;   
    // Start the child process. 
    if(CreateProcess( 
	    NULL,//static_cast<LPCTSTR>("glue.exe"),   // No module name (use command line)
        static_cast<LPSTR>(const_cast<char *>(cmd.c_str())),//static_cast<LPCTSTR>("glue.exe srlua.exe main.lua main.exe"),        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        CREATE_NEW_CONSOLE,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi )           // Pointer to PROCESS_INFORMATION structure
    == 0) 
    {
        return false;
    }
    // Wait until child process exits.
    WaitForSingleObject (pi.hProcess, INFINITE);
    // Close process and thread handles. 
    CloseHandle (pi.hProcess);
    CloseHandle (pi.hThread );
#endif	
#ifdef __gnu_linux__
    pid_t child_pid; 
    child_pid = fork (); 
    if (child_pid != 0) {
		//this->handle = &static_cast<void *>(child_pid);//pid_t;
	}		
    else {
	  std::vector<std::string> arg_split = String::split(argument, " ");
	  char * arg_list[arg_split.size()];
	  for(int i = 0; i < arg_split.size(); i++)
	  {
		  arg_list[i] = static_cast<char *>(const_cast<char *>(arg_split[i].c_str()));
		  std::cout << arg_list[i] << std::endl;
	  }
	  arg_list[arg_split.size()] = nullptr; // arg_list must end with a nullptr
    /* Now execute PROGRAM, searching for it in the path.  */ 
    execvp (static_cast<char *>(const_cast<char *>(program.c_str())), arg_list); 
    /* The execvp  function returns only if an error occurs.  */ 
    fprintf (stderr,  "an error occurred in execvp\n"); 
    abort (); 
  } 	
#endif
	return true;
}
bool Process::terminate()
{
#ifdef __windows__
	return (TerminateProcess(static_cast<HANDLE>(this->handle), 0) != 0);
#endif	
#ifdef __gnu_linux__
#endif	
}
bool Process::terminate(Process * process)
{
#ifdef __windows__
	return (TerminateProcess(static_cast<HANDLE>(process->get_handle()), 0) != 0);
#endif
#ifdef __gnu_linux__
    //kill(static_cast<pid_t>(handle), SIGTERM);
#endif	
}
void Process::exit(int code)
{
#ifdef __windows__
	ExitProcess(code);
#endif	
#ifdef __gnu_linux__
#endif	
}
void * Process::get_handle()
{
	return this->handle;
}
void * Process::get_active()
{
#ifdef __windows__
	return static_cast<void *>(GetCurrentProcess());
#endif
#ifdef __gnu_linux__
#endif	
}