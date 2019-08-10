#ifndef _EDIT
#define _EDIT

#include "ui.h"
#include "label.h"

#ifdef __cplusplus // if c++
#include <iostream>
#include <lua.hpp>

class Edit : public GUI {// can be a line edit or text edit or search field
    public:
	    Edit();                              static int new_(lua_State *L);
		~Edit();
		
		void draw();                   static int draw(lua_State *L);
		void append(const std::string& text); static int append(lua_State *L);// appends current text to another text
		// edit-related stuff here --
		void set_color(int red, int green, int blue, int alpha = 255); static int set_color(lua_State *L);// base_color
		void set_color(const Vector3& color);
		void set_color(const Vector4& color);
		void set_cursor(bool cursor); static int set_cursor(lua_State *L);// has text cursor?
		void set_cursor_position(double x, double y); static int set_cursor_position(lua_State *L);// 0, 0 by default0
		void set_cursor_position(const Vector2& position);
		void set_cursor_x(double x);
		void set_cursor_y(double y);
		void set_label(const Label& label); static int set_label(lua_State *L);
		void set_multilined(bool multilined);
		// text-related stuff here ---
		void set_text(const std::string& text); static int set_text(lua_State *L);// text
		void set_font(const FONT& font); static int set_font(lua_State *L);
        void set_text_color(int red, int green, int blue, int alpha = 255); static int set_text_color(lua_State *L);
	    void set_text_color(const Vector3& text_color);
	    void set_text_color(const Vector4& text_color);
		void set_text_size(int width, int height); static int set_text_size(lua_State *L);
		void set_text_style(int style); static int set_text_style(lua_State *L); // italic, higlighted, underlined, bold
		void set_text_background_color(int red, int green, int blue, int alpha = 255); static int set_text_background_color(lua_State *L);
		// getters
		Label * get_label()const;    static int get_label(lua_State *L);
		Vector4 get_color()const;     static int get_color(lua_State *L);
		int get_cursor_x() const;
		int get_cursor_y() const;
		Vector2 get_cursor_position()const;  static int get_cursor_position(lua_State *L);
		// text-related getters
		std::string get_text()const;  static int get_text(lua_State *L); 
		FONT * get_font()const;       static int get_font(lua_State *L);
		Vector4 get_text_color()const; static int get_text_color(lua_State *L);
		Vector2 get_text_size()const; static int get_text_size(lua_State *L);
		int get_text_style()const;    static int get_text_style(lua_State *L);
	    Vector4 get_text_background_color()const; static int get_text_background_color(lua_State *L);
		int get_character_limit()const;
		// boolean
		bool is_multilined() const;
		// interactions here --
		void cut();  static int cut(lua_State *L);
		void copy();  static int copy(lua_State *L);
		void paste(); static int paste(lua_State *L);
		void undo(); static int undo(lua_State *L);
		void redo(); static int redo(lua_State *L);
		void select(); static int select(lua_State *L);// (highlights specific text) or all
		void select_all(); static int select_all(lua_State *L);
		void clear(); static int clear(lua_State *L);// (std::string selected_text) or all
		void clear_all(); static int clear_all(lua_State *L);
		void zoom(int value); static int zoom(lua_State *L);// -1 (zoom out), +1 (zoom in) 
		// character count
        void set_character_limit(int limit); static int set_maximum_character(lua_State *L);// 0 = unlimited(default)
	    // hiding label =  edit->get_label()->hide() // syntax highlighting???
		// random - syntax highlighting
		void syntax(std::string keyword, int red, int green, int blue); // if keyword.contains(" \ " ");  if start_of_string = " 
		//void syntax(std::string keyword, const Vector3& color);
	private:
		// on functions
		void on_hover();
		void on_mousepress();
		void on_keypress();
		void on_enter();
		void on_backspace();
		// properties
	    Vector4 color;
	    bool   cursor;
		double cursor_x, cursor_y;
		int cursor_height;
		double zoom_factor; 
		unsigned int character_limit;
		bool multilined;
		int num_newlines;
		Label * label;
		// other
		//std::map<std::string, Vector3& color> syntax_list;
};
/*
line edit example(single line):
[________]
text edit example(multi-lined):
[________]
[________]
[________]
USAGE:
Edit * edit = new Edit();
	edit->set_position(78, 397);
*/
#endif
#endif
