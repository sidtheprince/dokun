#include "../include/list.h"

List::List() : color(106, 106, 106, 255), selection(nullptr), image(nullptr),
// outline
outline(true),
outline_width(1.0),
outline_color(0, 0, 0, 255),
outline_antialiased(false)
{
	set_position(0, 0);
	set_size(80, 20);
	set_orientation(0);
	
	label = new Label();
}
List::List(int x, int y) : color(106, 106, 106, 255), selection(nullptr), image(nullptr),
// outline (default)
outline(true),
outline_width(1.0),
outline_color(0, 0, 0, 255),
outline_antialiased(false)
{
	set_position(x, y);
	set_size(80, 20);
	set_orientation(0);
	
	label = new Label();
}
List::List(int x, int y, int width, int height) : color(106, 106, 106, 255), selection(nullptr), image(nullptr),
// outline
outline(true),
outline_width(1.0),
outline_color(0, 0, 0, 255),
outline_antialiased(false)
{
	set_position(x, y);
	set_size(width, height);
	set_orientation(0);
	
	label = new Label();	
}
int List::new_(lua_State *L)
{}
List::~List()
{}
		
void List::draw()
{
	if(is_visible())
	{
	    for(int i = 0; i < item_list.size(); i++)
	    {
			Widget * item = item_list[i];
			if(i == 0) item->set_position(get_position()); // first item is set at position of list (this)
		    if(i != 0) {item->previous = item_list[i - 1];  item->set_position(get_position().x, item->previous->get_position().y + item->previous->get_height());} // item[0] is the first item so it won't have any previous
            on_item(item, i);
			// Draw list
		    Renderer::draw_box(item->get_x(), item->get_y(), item->get_width(), item->get_height(), 
			    get_angle(), get_scale().x, get_scale().y, 
			    item->get_color().x, item->get_color().y, item->get_color().z, item->get_color().w,
			    item->get_radius(), false, false, 0, color, false, false, false, item->get_title_bar_button_close_color(),
			    item->has_outline(), item->outline_width, item->outline_color, item->outline_antialiased,
			    item->has_border(), 0, color,
			    item->has_gradient(), color,
			    item->has_shadow()
			);
			// Draw image
			Image * image = item->get_image();
            if(image != nullptr)
			{
				int image_width  = (image->is_resized() == false) ? image->get_width () : image->get_new_width ();
			    int image_height = (image->is_resized() == false) ? image->get_height() : image->get_new_height();					
				if(image->get_alignment() == "left"  ) image->set_relative_position(0, 0);
				if(image->get_alignment() == "center") image->set_relative_position((item->get_width() - item->get_height()) / 2, 0);
				if(image->get_alignment() == "right" ) image->set_relative_position(item->get_width() - item->get_height(), 0);		
				image->set_position(item->get_x() + image->get_relative_x(), item->get_y());
				image->scale_to_fit(get_width(), get_height());
                image->draw();				
			}	
	    }
	}
	on_draw(); // callback for all gui
}
int List::draw(lua_State *L)
{}
void List::add(const Widget& item)
{
	item_list.push_back(&const_cast<Widget&>(item));
	if(item_list.size() == 1) const_cast<Widget&>(item).set_position(get_position());
	const_cast<Widget&>(item).set_size(get_width(), get_height());
	const_cast<Widget&>(item).set_outline(outline);
	const_cast<Widget&>(item).set_outline_width(outline_width);
	const_cast<Widget&>(item).set_outline_color(outline_color);
	const_cast<Widget&>(item).set_outline_antialiased(outline_antialiased);
}
int List::add(lua_State *L)
{}
////////////////
void List::set_label(const Label& label)
{
	this->label = &const_cast<Label&>(label);
}
void List::set_color(int red, int green, int blue, int alpha)
{
	color = Vector4(red, green, blue, alpha);
}
void List::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}
void List::set_color(const Vector4& color)
{
	this->color = color;
}
void List::set_selection(const Widget& selection)
{
	this->selection = &const_cast<Widget&>(selection);
}
int List::set_selection(lua_State * L)
{}
///////////////
Label * List::get_label() const
{
	return label;
}
Vector4 List::get_color() const
{
	return color;
}
Widget * List::get_item(int index) const
{
	if(item_list.size() < index + 1)
	{
		Logger("Attempt to access invalid item in get_item() | list.cpp (162)");
		return nullptr;
	}
	return item_list[index];
}
int List::get_item_count()const // returns number of items added to list
{
	return item_list.size();
}
Widget * List::get_selection() const
{
	return selection;
}
int List::get_selection(lua_State * L)
{}
/////////////////
	// outline
void List::set_outline(bool outline)
{
	this->outline = outline;
}
int List::set_outline(lua_State * L)
{}
void List::set_outline_width(double width)
{
	this->outline_width = width;
}
int List::set_outline_width(lua_State * L)
{}
void List::set_outline_color(int red, int green, int blue, int alpha)
{
	this->outline_color = Vector4(red, green, blue, alpha);
}
int List::set_outline_color(lua_State * L)
{}
void List::set_outline_color(const Vector3& color)
{
	set_outline_color(color.x, color.y, color.z);
}
void List::set_outline_color(const Vector4& color)
{
	set_outline_color(color.x, color.y, color.z, color.w);	
}
void List::set_outline_antialiased(bool antialiased)
{
	outline_antialiased = antialiased;
}
//////////////////
void List::on_item(Widget* item, int index) // will not work under list->is_visible
{
    if(Mouse::is_over(item->get_rect()))
	{
		// highlight item on hover
		item->set_outline_width(3.0);
		//if mouse is pressed, set the selected item
		if(Mouse::is_pressed(1))
		{
			set_selection(*item);
			#ifdef DOKUN_DEBUG	
				std::cout <<"Item " << index << " selected.\n";
			#endif    
		}
	} else {item->set_outline_width(1.0);} // remove highlight	
}
