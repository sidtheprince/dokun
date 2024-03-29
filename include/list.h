#ifndef _LIST
#define _LIST

#include "ui.h"
#include "box.h"

#ifdef __cplusplus
class List : public GUI // can be applied to combo_box and widget
{
public:
    List();                             static int new_(lua_State *L);
	List(int x, int y);
	List(int x, int y, int width, int height);
	~List();
		
	void draw();                        static int draw(lua_State *L);
	void add(const Box& item);       static int add(lua_State *L);// adds a new list obj
	// setters (applies to all list items)
	void set_color(int red, int green, int blue, int alpha=255);
	void set_color(const Vector3& color);
	void set_color(const Vector4& color);
	void set_text(const std::string& text);
	void set_label(const Label& label);
	void set_image(const Image& image);
	
	std::string get_text() const;
	Label * get_label() const;
	Image * get_image() const;
	Vector4 get_color() const;
	Box * get_item(int index) const;
	int get_item_count()const; // returns number of items added to list
	Box * get_selection() const;            static int get_selection(lua_State * L);
	// outline
	void set_outline(bool outline);                                                static int set_outline(lua_State * L);
	void set_outline_width(double width);                                                      static int set_outline_width(lua_State * L);
	void set_outline_color(int red, int green, int blue, int alpha = 255);         static int set_outline_color(lua_State * L);
	void set_outline_color(const Vector3& color);
	void set_outline_color(const Vector4& color);
	void set_outline_antialiased(bool antialiased);		
private:
    void set_selection(const Box& selection); static int set_selection(lua_State * L);
    // callback
	void on_item(Box * item, int index);
	// friend
	friend class Combo;
    friend class Combobox;
    std::vector<Box *> item_list; // function add() this->next = list; list.set_position(this->get_x(), this->y + this->get_height())
	Image * image;
	Label * label;
	Vector4 color;
	Box * selection;
	// outline
	bool outline;
	double outline_width;
	Vector4 outline_color;
	bool outline_antialiased;
	// gradient
	bool gradient;
	Vector4 gradient_color;	// not sure if I need this, since each list_item can set its own unique gradient_color 
};
#endif
#endif
/*
Usage:
	List * list = new List();
	list->add(* new Box());
	// lists with different sizes
	list->add(* new Box());
	list->add(* new Box());
	list->add(* new Box());
	list->add(* new Box());
	list->get_item(0)->set_size(104, 67);
	list->get_item(1)->set_size(48, 27);
	list->get_item(2)->set_size(64, 37);
	list->get_item(3)->set_size(14, 97);
	// .. and different colors
	list->get_item(0)->set_color(220, 20, 60);
	list->get_item(1)->set_color(218, 112, 214);
	list->get_item(2)->set_color(65, 105, 225);
	list->get_item(3)->set_color(110, 123, 139);
	list->get_item(4)->set_color(0,	139, 69);
	// and text
	list->get_item(0)->set_label(*new Label("A"));
	list->get_item(1)->set_label(*new Label("B"));
	list->get_item(2)->set_label(*new Label("C"));
	list->get_item(3)->set_label(*new Label("D"));
	list->get_item(4)->set_label(*new Label("E"));
	list->get_item(0)->get_label()->set_alignment("center");
	list->get_item(1)->get_label()->set_alignment("center");
	list->get_item(2)->get_label()->set_alignment("center");
	list->get_item(3)->get_label()->set_alignment("center");
	list->get_item(4)->get_label()->set_alignment("center");	
	// and image
	list->get_item(0)->set_image(*new Image(Resource::get_image("box")->get_data(), 102, 102));
    // combobox
    Combobox * combo = new Combobox();
    combo->set_position(98, 300);
    combo->set_list(*list);
    // draw Combobox (also draws the list attached to it)
    combo->draw();
*/
