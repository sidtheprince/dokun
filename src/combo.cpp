#include "../include/combo.h"

Combobox::Combobox() : color(106, 106, 106, 255), list(nullptr), scrollbar(nullptr), image(nullptr),
// button
button_color(0, 51, 102, 255),
button_width(20),
// outline
outline(false),
outline_width(2.0),
outline_color(0, 51, 102, 255),
outline_antialiased(false),
// highlight
highlight(true),
highlight_color(0, 51, 102, 255)
{
	set_position(0, 0);
	set_size(90, 20); // 50, 25
	set_orientation(0);
	
	label = new Label();
}
Combobox::~Combobox()
{}

void Combobox::draw()
{
	if(is_visible())
	{
        on_button();	
		// Draw combo_box
		Renderer::draw_combobox(get_x(), get_y(), get_width(), get_height(), get_angle(), get_scale().x, get_scale().y,
		    get_color().x, get_color().y, get_color().z, get_color().w,
			button_color, button_width);	
		// Draw arrow		
		if(list != nullptr)
		{
			// adjust width of list items (to equal width of combobox)
			for(int i = 0; i < list->get_item_count(); i++) {
				if(scrollbar == nullptr) list->get_item(i)->set_width(get_width() + get_button_width());
				if(scrollbar != nullptr) list->get_item(i)->set_width(get_width());
			}
			// set list right below combo_box
			list->set_position(get_position().x, get_position().y + get_height()); // item[0] is the first item so i
			// update combo properties based on 'selected' widget
	        on_select();
			// Draw list
			list->draw();
		} // list is not null
		if(scrollbar != nullptr) 
		{
			scrollbar->set_size(get_button_width(), scrollbar->get_height()); // height?
			scrollbar->set_position(get_button_x(), get_button_y() + get_button_height());
			scrollbar->draw();
		}
	    if(image != nullptr)
		{
			image->set_position(get_x() + image->get_relative_x(), get_y() + image->get_relative_y());
			image->scale_to_fit(get_width(), get_height());
			image->draw();
		}
	} // is visible
	// callback function
	on_draw();
}                            
int Combobox::draw(lua_State * L)
{
    return 0;
}
void Combobox::add(const Box& widget){}                   
int Combobox::add(lua_State * L)
{
    return 0;
}
/////////////
/////////////
/////////////
void Combobox::set_list(const List& list)
{
	this->list = &const_cast<List&>(list);
	const_cast<List&>(list).set_visible(false); // list is not visible until combobox is activated
}                      
int Combobox::set_list(lua_State * L)
{
    return 0;
}
/////////////
void Combobox::set_scrollbar(const Scrollbar& scrollbar)
{
	this->scrollbar = &const_cast<Scrollbar&>(scrollbar);
	const_cast<Scrollbar&>(scrollbar).set_visible(false);
}
int Combobox::set_scrollbar(lua_State * L)
{
    return 0;
}
/////////////
void Combobox::set_text(const std::string& text)
{
	get_label()->set_string(text);
}     
int Combobox::set_text(lua_State *L)
{
    return 0;
}
/////////////
void Combobox::set_label(const Label& label)
{
	this->label = &const_cast<Label&>(label);
}         
int Combobox::set_label(lua_State *L)
{
    return 0;
}
/////////////
void Combobox::set_image(const Image& image)
{
	if(get_image()) // delete old image
	{
		delete this->image;
	}
	this->image = &const_cast<Image&>(image);
}         
int Combobox::set_image(lua_State *L)
{
    return 0;
}
/////////////
void Combobox::set_color(int red, int green, int blue, int alpha)
{
	color = Vector4(red, green, blue, alpha);
}
void Combobox::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}       
void Combobox::set_color(const Vector4& color)
{
	this->color = color;
}
int Combobox::set_color(lua_State *L)
{
    return 0;
}	
/////////////
std::string Combobox::get_text()const
{
    return "";
}                
int Combobox::get_text(lua_State *L)
{
    return 1;
}
/////////////
Label * Combobox::get_label()const
{
	return label;
}
int Combobox::get_label(lua_State *L)
{
    return 1;
}
/////////////
Image * Combobox::get_image()const
{
	return image;
}                   
int Combobox::get_image(lua_State *L)
{
    return 1;
}
/////////////
Vector4 Combobox::get_color()const
{
	return color;
}                   
int Combobox::get_color(lua_State *L)
{
    return 4;
}
/////////////
List * Combobox::get_list() const
{
	return list;
}
int Combobox::get_list(lua_State * L)
{
    return 1;
}
/////////////
Scrollbar * Combobox::get_scrollbar()const
{
	return scrollbar;
}
int Combobox::get_scrollbar(lua_State * L)
{
    return 1;
}
/////////////
/////////////
	// button
int Combobox::get_button_x() const
{
	return get_x() + get_width();
} 
int Combobox::get_button_x(lua_State *L)
{
    return 1;
}
int Combobox::get_button_y() const
{
	return get_y();
}                  
int Combobox::get_button_y(lua_State *L)
{
    return 1;
}
int Combobox::get_button_width() const
{
	return button_width;
}              
int Combobox::get_button_width(lua_State *L)
{
    return 1;
}
int Combobox::get_button_height() const
{
	return get_height();
}             
int Combobox::get_button_height(lua_State *L)
{
    return 1;
}
/////////////
void Combobox::on_button() // show or hide list on buttonpress or buttonrelease
{
	// if mouse over button ...
	if(Mouse::is_over(get_button_x(), get_button_y(), get_button_width(), get_button_height())) 
	{
		// and if mouse is pressed ..
		if(Mouse::is_pressed(1))
		{
			if(list) list->set_visible(true);
            if(scrollbar) scrollbar->set_visible(true);				
		}
    } else //if(!Mouse::is_over(get_button_x(), get_button_y(), get_button_width(), get_button_height()))  
	{
		// if mouse is pressed elsewhere ..
		if(list) {
		if(list->is_visible())
		{
		    for(int i = 0; i < list->get_item_count(); i++)
		    {
			    Box * item = list->get_item(i);
	            if(Mouse::is_over(item->get_rect())) 
				{
	                if(Mouse::is_pressed(1))
		            {
				        list->set_selection(*item);
				    #ifdef DOKUN_DEBUG
                        std::cout << "Combo->list item " << i << " selected" << std::endl;
				    #endif	
				    }	
                }		
                if(Mouse::is_pressed(1))
		        {				
					if(list) list->set_visible(false);
		            if(scrollbar) scrollbar->set_visible(false);
				}
		    }
		}
	    }	
	} // mouse over button	
}
/////////////
void Combobox::on_select()
{
	if(list)
	{
		Box * selection = list->get_selection();
		if(selection)
		{
			set_color(selection->get_color()); 
			// Image
			if(selection->get_image()) 
			{
				set_image(* new Image(*selection->get_image()));
				if(image->get_alignment() == "left"  ) image->set_relative_position(0, 0);
                if(image->get_alignment() == "center") image->set_relative_position((get_width() - get_height()) / 2, 0);
				if(image->get_alignment() == "right" ) image->set_relative_position(get_width() - get_height(), 0);
			}  else set_image(* static_cast<Image *>(nullptr));	
            // Label	 
            if(selection->get_label())
	        {
				if(!selection->get_label()->get_string().empty())
					;// set label
				else ;
			}			
		}
	}
}
