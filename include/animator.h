#ifndef _ANIMATOR
#define _ANIMATOR

#include "timer.h"
#include "sprite.h"

// actions represent an animation such as, walk, run, fight, or jump
// each action can contain a number of frames to make animation possible
// a single image, or frame is not considered valid for animation

// [NOTE]: actions and frames start at a 0 index!!

#ifdef __cplusplus
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <lua.hpp>

class Animator { // supports keyframe/frame-by-frame animation, sprite_sheet animation will be supported later
    public:
        Animator();    static int new_(lua_State *L);
        ~Animator();
        // loads animation
        bool load(const std::string& file_name, int action_num, int frame); 
		static bool load(Animator &animator, std::string filename, int action_num, int frame); 
		bool load_rect(const std::string& file_name, int x, int y, int width, int height, int action_num, int frame);
		bool load_rect(const std::string& file_name, const Vector2& position, const Vector2& size, int action_num, int frame);
		bool load_rect(const std::string& file_name, const Vector4& rect, int action_num, int frame);
		static int load(lua_State *L); // coming soon load_frame("boy_spritesheet.png", 32, 0, 32, 32) // <== cuts a square from a texture
        //bool load_frame();
		// starts the animation
		void animate(Sprite *sprite, int action_num); /* start an action(animation)*/ /* called inside loop*/
        static void animate(Animator &animator, Sprite *sprite, int action_num);
        static int animate(lua_State *L);
        // set animation speed
		void set_speed(double speed);
		static void set_speed(Animator &animator, double speed);
		static int set_speed(lua_State *L);
		// return number of frames in an action
        int get_frames(int action)const; // returns # of frames in per action(one animation)
		static int get_frames(Animator &animator, int action);
		static int get_frames(lua_State *L);
		// return number of actions in an animator object
		int get_actions()const;
		static int get_actions(Animator &animator);
		static int get_actions(lua_State *L);
		// return animation speed
		double get_speed();
		static double get_speed(Animator & animator);
		static int get_speed(lua_State *L);
    private:
        int frames;
		int actions;
		double speed;/*
		double update_time;
		double last_time;
		double delta_time;*/
		double old_time = 0; 
        double new_time = (clock() / (double)CLOCKS_PER_SEC);
        double delta_time;
	    double fps_desired = 60;
        //std::vector< std::vector<std::string> > action_list;
		std::vector< std::vector<Texture *> > action_list;
		
        void set_current_frame(std::string filename);
		void set_current_texture(Texture &texture);
};
#endif
#endif
