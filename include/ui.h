#ifndef _UI
#define _UI

#include "platform.h"
#include "utility.h"
#include "vector.h"
#include "factory.h"
#include "entity.h"
#include "renderer.h"
#include "mouse.h"
#include "resource.h"

#ifdef __cplusplus
#include <iostream>
#include <lua.hpp>

class GUI: public Entity  
{
	public:
	    // constructers
	    GUI();
		// destructor
		virtual ~GUI();        
        // normal functions
		template<class Base>
		static inline Base * create()
		{
			// Base must be a class
			if(std::is_class<Base>::value) 
	        {
				// create a dummy_ptr
			    Base dummy_ptr;
				// dummy_ptr must be gui or derived gui object e.g Button*, Label*
			    if(!dokun::instanceof<GUI>(&dummy_ptr)) 
			    {
				    std::cerr << "Invalid gui" << std::endl;
				    return nullptr;
			    }
			}
		    return new Base();
		}
		void show();                                    static int show(lua_State *L); // shows child ui as well (child ui is always on top of the parent ui)
		void hide();                                    static int hide(lua_State *L); // hides child ui as well (child ui is always on top of the parent ui)
		static void show_all();                         static int show_all(lua_State *L);
		static void hide_all();                         static int hide_all(lua_State *L);
		// test function
		static void connect(const GUI& a, int signal, const GUI& b, std::function<void(void)> slot);
		// setters
		virtual void set_width(int width);                      static int set_width(lua_State *L); // label has its own kind of width x height based on its font
		virtual void set_height(int height);                    static int set_height(lua_State *L);
		virtual void set_size(int width, int height);           static int set_size(lua_State *L);
		virtual void set_size(const Vector2& size);
		void set_position(double x, double y);          static int set_position(lua_State *L);	
		void set_position(const Vector2& position);     static int set_relative_position(lua_State *L);	
		void set_relative_position(double x, double y);
		void set_relative_position(const Vector2& position);
		void set_orientation(int orientation);          static int set_orientation(lua_State *L); // 0 = horizontal, 1 = vertical, 2 diagonal
		void set_parent(const GUI& gui);                static int set_parent(lua_State *L);
		// state
		void set_visible(bool visible);                 static int set_visible(lua_State *L); // visible = true, invisible = false
		void set_active(bool active);                   static int set_active(lua_State *L); // active = true, disabled = false
		void set_focused(bool focused);                 static int set_focused(lua_State *L);
		// interaction
		void set_draggable (bool draggable);            static int set_draggable(lua_State *L);
		void set_droppable (bool droppable);            static int set_droppable(lua_State *L);
		void set_resizeable(bool resizeable);           static int set_resizeable(lua_State *L);
		void set_sortable(bool sortable);               static int set_sortable(lua_State *L);
		// getters
		virtual int get_width() const;                  static int get_width(lua_State *L); // label width and height is applied in a different way based on its font
		virtual int get_height() const;                 static int get_height(lua_State *L);
		virtual Vector2 get_size() const;               static int get_size(lua_State *L);
		double get_x() const;                           static int get_x(lua_State *L);
		double get_y() const;                           static int get_y(lua_State *L);
		Vector2 get_position() const;                   static int get_position(lua_State *L);
		Vector2 get_relative_position() const;          static int get_relative_position(lua_State *L);
        double get_relative_x() const;                  static int get_relative_x(lua_State *L);
		double get_relative_y() const;                  static int get_relative_y(lua_State *L);
		int get_orientation() const;                    static int get_orientation(lua_State *L);
		GUI * get_parent() const;                       static int get_parent(lua_State *L);// returns the gui which self is on top of(or related to)	
	    std::vector<GUI *> get_children();              static int get_children(lua_State *L); // returns all guis sitting on top of parent gui
	    Vector4 get_rect()const;                        static int get_rect(lua_State *L);
		// conditions (boolean)
		bool is_visible();                              static int is_visible(lua_State *L); // returns if visible or not
		bool is_active();                               static int is_active(lua_State *L);// returns if active or disabled
		//bool is_focused();                              static int is_focused(lua_State *L);
		bool is_draggable();                            static int is_draggable(lua_State * L);
		bool is_droppable();                            static int is_droppable(lua_State * L);
		bool is_resizeable();                           static int is_resizeable(lua_State * L);
		bool is_sortable();                             static int is_sortable(lua_State * L);
	    bool is_gui();                                  static int is_gui(lua_State *L);
		bool has_parent();                              static int has_parent(lua_State * L); // returns true if current ui is child
	    bool is_child_of(const GUI& gui);               static int is_child_of(lua_State *L);
	    bool is_parent_of(const GUI& gui);              static int is_parent_of(lua_State *L);
	//protected:
	    void set_scale(double sx, double sy);           static int set_scale(lua_State *L);
		Vector2 get_scale()const;                       static int get_scale(lua_State *L);
		void set_angle(double angle);                   static int set_angle(lua_State *L);
		double get_angle()const;                        static int get_angle(lua_State *L);
	    // callbacks
		virtual void on_create();
		virtual void on_draw();
		// interaction checks
	    bool is_hovered(); static int is_hovered(lua_State * L);// mouse over
	    bool is_pressed(); static int is_pressed(lua_State * L); // executes many times in loop
		bool is_selected(); //static int (lua_State * L); // executes once in loop
		bool is_released(); //static int (lua_State * L);
	    bool is_focused(); //static int (lua_State * L); // has focus
		bool is_dragged(); //static int (lua_State * L);
		bool is_dropped(); //static int (lua_State * L);
		bool is_resized(); //static int (lua_State * L);
		bool is_sorted(); //static int (lua_State * L);
    private:
	    void generate(void);
	    // friend
		friend class Widget;
	    // properties
        int width;
        int height;	
		double x, y;
		int orientation; // hor=0, vert=1, diag=2
		GUI * parent;
		double scale_x, scale_y;
		double angle;
		Vector2 relative; // position relative to parent
		////////////////
		// states
		bool visible; // if gui is visible or not
		bool active; // if gui is disabled or not
		static GUI * focused; // if gui has main focus
		////////////////
		// interaction
		bool draggable; // if gui can be dragged
		bool droppable; // if gui can be dropped
		bool resizeable; // can gui be resized?
		bool sortable; // Reorder elements in a list or grid
		// shader
		static Shader * shader;
}; // widget=0, label=1, button=2, edit=3, progress_bar=4
/*
    Button * button = GUI::create<Button>(); // return new Button()
	Sprite * sprite = GUI::create<Sprite>(); // return nullptr
	
	if a ui element's parent is hidden, then the child is hidden as well
	if shown, the child will be shown as well.
*/
#endif
#endif
