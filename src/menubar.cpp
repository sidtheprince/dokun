#include "../include/menubar.h"

Menubar::Menubar() : color(106, 106, 106, 255), submenu_color(106, 106, 106, 255),
// outline
outline(false),
outline_width(1.0),
outline_color(0, 0, 0, 255),
outline_antialiased(false),
// highlight
highlight(true),
highlight_color(0, 51, 102, 255)
{
	set_position(0, 0);
	set_size(Renderer::window_width, 20); //Logger("Menubar width is " + String::to_string(get_width()) + " on creation");// menubar_width / menu_width = how_many_menu_can_fit  
	set_orientation(0);	                  // If you want a vertical menubar then just use a combo_box
}                             
Menubar::Menubar(int x, int y)
{
	set_position(x, y);
	set_size(Renderer::window_width, 20);
	set_orientation(0);	
}
Menubar::Menubar(int x, int y, int width, int height)
{
	set_position(x, y);
	set_size(width, height);
	set_orientation(0);
}
int Menubar::menu_new(lua_State *L)
{
    return 1;
}
Menubar::~Menubar(){}
		
void Menubar::draw() // because Menubar is parent to all the menus, its menu children will be automatically drawn unless "visible" is set to false
{
	if(is_visible())
	{
		// Do not draw menubar - Imaginary menubar that holds menus and does not actually exist. It is just a concept (only the menus are visible)
		 for(int i = 0; i < menu_list.size(); i++)
	    {
			Widget * menu = menu_list[i];
			if(get_orientation() == 0) 
			{  // HORIZONTAL MENUBAR
			    // adjust and update size of each menu according to menubar size
			    menu->set_width(this->get_width() / menu_list.size()); // 0 - when horizontal each menu's width is equal to the menubar's width divided by the number of menus // menu_bar_width / number_of_menus =  1280 / 1 = 1280 (1 menu means the single menu will be the entire width of the screen;) | 1280 / 2 =  640 (2 menus would mean each menu would be half the width of the screen) 
			    // calculate position and size of previous menus
		        if(i != 0) {menu_list[i]->previous = menu_list[i - 1];  menu_list[i]->set_position(menu_list[i]->previous->get_position().x + menu_list[i]->previous->get_width(), get_position().y);} // menu[0] is the first menu so it won't have any previous
		        if(i != menu_list.size()-1) menu_list[i]->next = menu_list[i + 1]; // assign next_menu if not last
		        // Draw menus (children of parent menubar will be automatically drawn with the 'on_draw' function)
            }
		    if(get_orientation() == 1) 
			{ 	// VERTICAL MENUBAR
			    // adjust and update size of each menu according to menubar size
			    menu->set_height(this->get_height() / menu_list.size()); // 1 - when horizontal each menu's height is equal to the menubar's height divided by the number of menus
			    // calculate position and size of previous menus
		        if(i != 0) {menu_list[i]->previous = menu_list[i - 1];  menu_list[i]->set_position(get_position().x, menu_list[i]->previous->get_position().y + menu_list[i]->previous->get_height());} // menu[0] is the first menu so it won't have any previous
		        if(i != menu_list.size()-1) menu_list[i]->next = menu_list[i + 1]; // assign next_menu if not last
		        // Draw menus (children of parent menubar will be automatically drawn with the 'on_draw' function)
            }
			// Draw label (for menus - automatically drawn if object is a widget)
			// Draw image (for menus - automatically drawn if object is a widget)
			/////////////////////
			for(int j = 0; j < menu->sub.size(); j++)
			{
				Widget * sub = menu->sub[j];
				if(Mouse::is_over(menu->get_rect()))
	            {
					std::cout << "Mouse over menu " << menu->get_label()->get_string() << std::endl;
		            if(Mouse::is_pressed(1)) {sub->set_visible(true);} else if(Mouse::is_pressed(1)) sub->set_visible(false);
	            }
				if(sub->is_visible())
		        {
				    menu->sub[0]->set_position(menu->sub[0]->previous->get_position().x, menu->previous->get_position().y + menu->sub[0]->previous->get_height());
				    if(j != 0) {sub->previous = menu_list[i]->sub[j - 1];sub->set_position(sub->previous->get_position().x, sub->previous->get_position().y + sub->previous->get_height());}
			        if(j != menu->sub.size()-1) sub->next = menu->sub[j + 1];
					// draw sub_menu
			       /* Renderer::draw_box(sub->get_position().x, sub->get_position().y, sub->get_width(),
			            sub->get_height() , get_angle(), get_scale().x, get_scale().y, 
			            sub->get_color().x, sub->get_color().y, sub->get_color().z, sub->get_color().w,
			            0, false, false, 0, color, false, false, false,	sub->get_title_bar_button_close_color(),			
			            sub->has_outline(), sub->outline_width, sub->outline_color, sub->outline_antialiased,
			            sub->has_border(), 0, color,
			            sub->has_gradient(), color,
			            sub->has_shadow());*/
			        // Draw label (for menus - automatically drawn if object is a widget)
				    // Draw image (for menus - automatically drawn if object is a widget)
				    // On hover
				    /*if(highlight)
				    {
				        if(Mouse::is_over(sub->get_rect()))
	                    {
						    sub->set_color(highlight_color);
 						    if(Mouse::is_pressed(1)) 
					        {
							#ifdef DOKUN_DEBUG0	
						        std::cout <<"Submenu " << j << " at index " << i << " selected.\n";
					        #endif
							}
					    } else sub->set_color(submenu_color);
				    }*/
				}
			}
		}
	}
	on_draw(); // draws all children of menubar
}                           
int Menubar::draw(lua_State *L)
{
    return 0;
}
void Menubar::add(const Widget& menu)
{
	menu_list.push_back(&const_cast<Widget&>(menu));                     //std::cout << "number of menus = " << menu_list.size() << std::endl; // get number of menus after creation
	const_cast<Widget&>(menu).set_size(get_width() / menu_list.size(), get_height()); // window_width / num_of_menu, 20
	const_cast<Widget&>(menu).set_parent(*this);
	const_cast<Widget&>(menu).set_color(get_color());
	const_cast<Widget&>(menu).set_outline(true);
	const_cast<Widget&>(menu).set_outline_width(2.0);
	const_cast<Widget&>(menu).set_outline_color(32, 32, 32);
}          
void Menubar::add(const std::string& menu_name)
{
	Widget * menu = new Widget();
	Label * label = menu->get_label();
	label->set_string(menu_name); // set menu's label string
	add(*menu);
}
int Menubar::add(lua_State * L)
{
    return 0;
}
void Menubar::sub(const Widget& submenu, int index)
{
    if(menu_list.size() < index + 1) // if menu at index does not exist
		Logger("Attempt to access invalid location in sub() | menubar.cpp (180)");
	const_cast<Widget&>(submenu).set_size(menu_list[index]->get_width(), get_height());
	const_cast<Widget&>(submenu).set_position(menu_list[index]->get_position().x, menu_list[index]->get_position().y + menu_list[index]->get_height());
	const_cast<Widget&>(submenu).set_color(submenu_color);
	const_cast<Widget&>(submenu).set_outline(true);
	const_cast<Widget&>(submenu).set_outline_width(2.0);
	const_cast<Widget&>(submenu).set_outline_color(32, 32, 32);
	const_cast<Widget&>(submenu).set_visible(false);
	const_cast<Widget&>(submenu).previous = menu_list[index]; // set menu as its previous
	menu_list[index]->sub.push_back(&const_cast<Widget&>(submenu)); // &const_cast<Widget&>(submenu)
	
	/*
    if(sub_list.size() < index + 1) // if row at index does not exist, create it
	{
        sub_list.push_back(std::vector<Widget *>()); // create empty row
	}
    if(menu_list.size() < index + 1) // if menu at index does not exist
		Logger("Attempt to access invalid location in sub() | menubar.cpp (180)");	
	sub_list[index].push_back(&const_cast<Widget&>(submenu));
	const_cast<Widget&>(submenu).set_size(50, get_height());
	const_cast<Widget&>(submenu).set_position(menu_list[index]->get_position().x, menu_list[index]->get_position().y + menu_list[index]->get_height());
	const_cast<Widget&>(submenu).set_color(submenu_color);
	const_cast<Widget&>(submenu).set_outline(true);
	const_cast<Widget&>(submenu).set_outline_width(2.0);
	const_cast<Widget&>(submenu).set_outline_color(32, 32, 32);
	const_cast<Widget&>(submenu).set_visible(false);
	if(sub_list[index].size() == 1) // if first sub_item has been added
	{
		const_cast<Widget&>(submenu).previous = menu_list[index]; // set menu as its previous
	}*/
}
void Menubar::sub(const std::string& menu_name, const std::string& sub_name) // menu must exist
{
	// sub_menu
	Widget * submenu = new Widget();
	submenu->get_label()->set_string(sub_name);
	for(int j = 0; j < menu_list.size(); j++)
	{
		if(menu_list[j]->get_text() == menu_name)
		{
			//std::cout << "submenu added at index [" << j << "]\n";
			sub(*submenu, j);
			break;
		}
	}
}

void Menubar::set_color(int red, int green, int blue, int alpha)
{}
void Menubar::set_color(const Vector3& color)
{}
void Menubar::set_color(const Vector4& color)
{}
// outline
void Menubar::set_outline(bool outline)
{} 
int Menubar::set_outline(lua_State *L)
{
    return 0;
}
void Menubar::set_outline_width(double width)
{} 
int Menubar::set_outline_width(lua_State *L)
{
    return 0;
}
void Menubar::set_outline_color(int red, int green, int blue, int alpha)
{} 
int Menubar::set_outline_color(lua_State *L)
{
    return 0;
}
void Menubar::set_outline_color(const Vector3& color)
{}
void Menubar::set_outline_color(const Vector4& color)
{}
void Menubar::set_outline_antialiased(bool antialised)
{} 
int Menubar::set_outline_antialiased(lua_State *L)
{
    return 0;
}



Widget * Menubar::get_menu(int index)
{
#ifdef DOKUN_DEBUG	
	if(menu_list.size() < index + 1) Logger("Attempt to access invalid location | menubar.cpp (234)");
#endif	
	return menu_list[index];
}
int Menubar::get_menu(lua_State * L)
{
    return 1;
}
Widget * Menubar::get_submenu(int menu_index, int sub_index)
{
#ifdef DOKUN_DEBUG	
	if(menu_list.size() < menu_index + 1) Logger("Attempt to access invalid location | menubar.cpp (243)");
	if(sub_list.size() < sub_index + 1) Logger("Attempt to access invalid location | menubar.cpp (244)");
#endif	
	return sub_list[menu_index][sub_index];
}
int Menubar::get_submenu(lua_State * L)
{
    return 1;
}
int Menubar::get_menu_count() // returns number of menus
{
	return menu_list.size();
}
int Menubar::get_menu_count(lua_State * L)
{
    return 1;
}
int Menubar::get_submenu_count(int index) // returns number of submenus at index
{
#ifdef DOKUN_DEBUG		
	if(sub_list.size() < index + 1) Logger("Attempt to access invalid location | menubar.cpp (229)");
#endif	
	return sub_list[index].size();
}
int Menubar::get_submenu_count(lua_State * L)
{
    return 1;
}
Vector4 Menubar::get_color()
{
	return color;
}
bool Menubar::has_sub(int menu_index)
{
	return (sub_list.size() > menu_index);
}
void Menubar::on_hover(void)
{}
void Menubar::on_menu_press(Widget * menu, int index)
{
	if(Mouse::is_over(menu->get_rect()))
	{
		//if(Mouse::is_pressed(1)) {sub->set_visible(true);} 
		//else if(Mouse::is_pressed(1)) sub->set_visible(false);
	}
	/*
	if(sub_list.size() > index)
	{	
		for(int j = 0; j < sub_list[index].size(); j++) // number of menus with subs
		{
			Widget * sub  = sub_list[index][j];
			
			if(menu->is_pressed()) {sub->set_visible(true);} 
			else 
			{
				if(Mouse::is_pressed(1)) 
				{
				    sub->set_visible(false);
				}
			}
		}
	}*/
}
