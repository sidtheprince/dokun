#ifndef _SOUND
#define _SOUND

#include "audio.h"

#ifdef __cplusplus // if c++
class Sound : public Audio {
    public:
        Sound();                                              static int new_(lua_State *L);
		Sound(const Sound& sound);
		Sound(const std::string& file_name);
		Sound(void * data, size_t size);
        ~Sound();
		// normal
        bool load(const std::string& file_name);                     static int load(lua_State *L);		
        // boolean
        bool is_sound();
		static bool is_sound(Audio * audio);
    private:
};
#endif
#endif