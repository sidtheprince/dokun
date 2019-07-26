#ifndef _TOOLTIP
#define _TOOLTIP

#include "ui.h"
#include "label.h"
#ifdef __cplusplus
#include "lua.hpp"
class Tooltip : public GUI {
public:
    Tooltip();
	Tooltip(int x, int y);
	Tooltip(int x, int y, int width, int height);
	Tooltip(const std::string& text);
	~Tooltip();
	// normal
	void draw(); static int draw(lua_State *L);
	// setters
	void set_color(int red, int green, int blue, int alpha = 255); static int set_color(lua_State *L);
	void set_color(const Vector4& color);
	void set_color(const Vector3& color);
	void set_text(const std::string& text); static int set_text(lua_State *L);
	void set_label(const Label& label); static int set_label(lua_State *L);
	void set_text_color(int red, int green, int blue, int alpha = 255); static int set_text_color(lua_State *L);
	void set_text_color(const Vector3& color);
	void set_text_color(const Vector4& color);
	// getters
	std::string get_text()const; static int get_text(lua_State *L);
	Label * get_label()const; static int get_label(lua_State *L);
	Vector4 get_color()const; static int get_color(lua_State *L);
	Vector4 get_text_color()const; static int get_text_color(lua_State *L);
	// boolean
private:
    std::string text;
	Label * label;
	Vector4 color;
	Vector4 text_color;
};
#endif
#endif