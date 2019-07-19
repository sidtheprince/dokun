#ifndef _MUSIC
#define _MUSIC
#include "audio.h"

#ifdef __cplusplus
class Music : public Audio {
    public:
        Music();                                                     static int new_(lua_State *L);
		Music(const Music& music);
		Music(const std::string& file_name);
		Music(void * data, size_t size); 		
        ~Music();
		// normal
        //bool load(const std::string& file_name);                     static int load(lua_State * L);	
        // boolean
		static bool is_music(Audio * audio);
	private:
};
#endif
#endif