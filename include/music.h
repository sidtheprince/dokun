#ifndef _MUSIC
#define _MUSIC
#include "audio.h"

#ifdef __cplusplus
class Music : public Audible {
    public:
        Music();                                                     static int music_new(lua_State *L);
		Music(const Music& music);
		Music(const std::string& file_name);
		Music(void * data, size_t size); 		
        ~Music();
		// normal
        //bool load(const std::string& file_name);                     static int load(lua_State * L);	
        // boolean
		static bool is_music(Audible * audible);                     static int is_music(lua_State * L);
	private:
};
#endif
#endif
