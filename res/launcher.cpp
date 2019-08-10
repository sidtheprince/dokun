#include <lua.hpp>
//#include "../include/dokun.h"

lua_State * L;
///////////////
static bool load_script(const std::string& filename) 
{
    if(luaL_dofile(L,filename.c_str()) != 0) 
        return false;
    return true;  // load success!
}
///////////////
static void lua_execute(const std::string& command) // executes any line or function in lua
{
	luaL_dostring(L, command.c_str());
}
///////////////
///////////////
int main(int argc, char * argv[])
{
    L = luaL_newstate();
	luaL_openlibs(L);
	
	std::string filename = "";
    std::cout << "argc = " << argc << std::endl; 
    for(int i = 0; i < argc; i++) 
    {
        filename = argv[i];
        std::cout << "argv[" << i << "] = " << filename << std::endl; 
      	
        if(!load_script(filename))//if(!load_script("luac.out")) 
        {
	        printf("load failed!\n");
            lua_error(L);	 
        }
    }    	
    std::cin.get();
	lua_close(L);
    return 0;
} // use luac to compile multiple lua files into one
