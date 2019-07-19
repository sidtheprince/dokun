#ifndef _TAB
#define _TAB

#include "ui.h"
#include "widget.h"
#ifdef __cplusplus
class Tab : public GUI
{
public:
    Tab();
    ~Tab();

	void draw();                               static int draw(lua_State * L);
    void add(const Tab& tab);                  static int add(lua_State * L);
	
	void set_text(const std::string& text);    static int set_text(lua_State *L);
	void set_label(const Label& label);        static int set_label(lua_State *L);
	void set_image(const Image& image);        static int set_image(lua_State *L);
	void set_color(int red, int green, int blue, int alpha = 225);
	void set_color(const Vector3& color);      static int set_color(lua_State *L);
	void set_color(const Vector4& color);
	
    std::string get_text()const;               static int get_text(lua_State *L);
	Label * get_label()const;                  static int get_label(lua_State *L);
	Image * get_image()const;                  static int get_image(lua_State *L);
	Vector4 get_color()const;                  static int get_color(lua_State *L);	
private:
    Widget * selection;
    Label * label;
	Image * image;
    Vector4 color;	
	// outline
	bool outline;
	double outline_width;
	Vector4 outline_color;
	bool outline_antialiased;	
};
#endif
#endif