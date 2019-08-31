#include "../include/chat.h"

////////////
Chatbox::Chatbox()
{  
    // box
    box = new Box();
    box->set_size(500, 100); // update size in loop (in case of changes to edit size)
    box->set_color(0, 51, 102, 255); // dokun blue
    box->set_gradient(true);
    // edit
	edit = new Edit();
	edit->set_character_limit(50);
	edit->set_size(box->get_width(), 20);
	//edit->set_position(700, 600);
	Label * edit_label = new Label();
	edit_label->set_color(49, 39, 19, 255);
	edit_label->set_relative_position(0, 4);
	edit->set_label(* edit_label);      
    // label_list
    label_list.reserve(box->get_height() / round(edit_label->get_height())); // can only hold (chat_height / height_of_each_label)
    std::cout << "Labels reserved for chat_box: " << label_list.capacity() << std::endl;
    // scrollbar
    scrollbar = new Scrollbar();
    scrollbar->set_size(10, box->get_height());
    scrollbar->set_handle_size(box->get_height()); // scrollbar is same height as chatbox
    //scrollbar->set_parent(*box);
    // maybe I should add a "Enter" button?
}
////////////
Chatbox::~Chatbox() {}
////////////
void Chatbox::draw()
{
    on_scroll();
    on_enter();
    // update label_list capacity (in case chatbox's height increases, which means more labels can be appended, or added to it)
    label_list.reserve(box->get_height() / round(edit->get_label()->get_height())  ); //std::cout << "edit_label->get_height() = " << edit->get_label()->get_height() << std::endl; // 19=each glyphs height    
    // update geometry
    edit->set_size(box->get_width(), edit->get_height()); // update size in loop (in case of changes to edit size)
    edit->set_position(box->get_x(), box->get_y() + box->get_height()); // update position in loop (in case of changes to edit position)
    scrollbar->set_size(scrollbar->get_width(), box->get_height());
    scrollbar->set_position(box->get_x() + (box->get_width() - scrollbar->get_width()), box->get_y());//scrollbar->set_relative_position(box->get_width() - scrollbar->get_width(), 0);//scrollbar->set_position(box->get_x() + box->get_width(), 0);
    scrollbar->set_step( label_list.capacity() + discarded_string_list.size()); // 5 steps    std::cout << "Scrollbar step increased" << ". Total steps: " << scrollbar->get_maximum_value() << std::endl;std::cout << "scrollbar value " << scrollbar->get_value() << std::endl;
    // update character limit
    edit->set_character_limit(edit->get_width() / 10);
    // update visibility (depending on if box is hidden or iconified)
    edit->set_visible((box->is_iconified() == false) && (box->is_visible() == true));
    scrollbar->set_visible((box->is_iconified() == false) && (box->is_visible() == true));
    // draw edit and box and scrollbar
    edit->draw();
    box->draw ();
    if(discarded_string_list.size() > 0) scrollbar->draw(); // draw scrollbar only when box is full of text
    // draw labels
    for(int i = 0; i < label_list.size(); i++) 
    {
        if(i == 0) label_list[0]->set_position( box->get_x() + 1, box->get_y() + (box->get_height() - (label_list[0]->get_height() * label_list.size()) ) ); // first label in list will determine the position of the rest
        if(i != 0) { label_list[i]->set_position( label_list[i - 1]->get_x(), label_list[i - 1]->get_y() + label_list[i - 1]->get_height() ); } // set label position to the position of its previous - height    
        label_list[i]->set_visible((box->is_iconified() == false) && (box->is_visible() == true));
        label_list[i]->draw();
    }
}
////////////
void Chatbox::write(const std::string& text) // writes a message to box without the need for an edit 
{
    // if text (in edit) is empty, exit function (cannot submit empty text)_
    if(text.empty()) return;
    // add / append new label to chat_box (with a copy of edit_label's string)
    if(label_list.size() < label_list.capacity())  // if capacity has not yet been reached
    {
        label_list.push_back(new Label(text));
    } // label_less
    else if(label_list.size() >= label_list.capacity()) {// capacity has been reached
        // store string that will be discarded (or hidden from chatbox) for later use
        //std::cout << "discarded from chatbox: " << label_list[0]->get_string() << std::endl;
        discarded_string_list.push_back(label_list[0]->get_string());
        // try to change the label string without removing anything
        label_list[0]->set_string( label_list[1]->get_string() ); // first element copies second element's string
        label_list[1]->set_string( label_list[2]->get_string() ); // second element copies third element's string and so on ...
        label_list[2]->set_string( label_list[3]->get_string() );
        label_list[3]->set_string( label_list[4]->get_string() );
        // last element becomes/copies the edit_label's string
        label_list[4]->set_string( text );
        // scrollbar_handle goes the opposite way of the text (text goes up, scrollbar_handle goes down)
        // TIP: scrollbars always start from the top
        if(discarded_string_list.size() > 0)//if(label_list.size() > label_list.capacity()) // if label capacity is surpassed // lets say the chatbox's height increases
        {
            scrollbar->set_handle_size( box->get_height() - edit->get_label()->get_height() ); // reduce handle size by label_height
            scrollbar->set_value( scrollbar->get_value() + 1 ); // bring scrollbar down as text goes up (increase scrollbar_value) // when a new label is appended to chat box, the scrollbar comes down (but you can scroll up to see the label at the top of the chatbox)  
        }    
    } // label_greater_or_equal
}
////////////
void Chatbox::execute(const std::string& command, lua_State *L)
{
    // if text (in edit) is empty, exit function (cannot submit empty text)_
    if(command.empty()) return;   
    // add / append new label to chat_box (with a copy of edit_label's string)
    if(label_list.size() < label_list.capacity())  // if capacity has not yet been reached
    {
        label_list.push_back(new Label(command));
        // execute Lua or system commands
        if(L) { if(luaL_dostring(L, command.c_str()) != 0) {std::cerr << "luaL_dostring : failed to execute Lua command";} }
        else  { system(command.c_str()); } // ShellExecute(0, "open", "cmd.exe", "/C ipconfig > out.txt", 0, SW_HIDE);
    } // label_less
    else if(label_list.size() >= label_list.capacity()) {// capacity has been reached
        // store string that will be discarded (or hidden from chatbox) for later use
        //std::cout << "discarded from chatbox: " << label_list[0]->get_string() << std::endl;
        discarded_string_list.push_back(label_list[0]->get_string());
        // try to change the label string without removing anything
        label_list[0]->set_string( label_list[1]->get_string() ); // first element copies second element's string
        label_list[1]->set_string( label_list[2]->get_string() ); // second element copies third element's string and so on ...
        label_list[2]->set_string( label_list[3]->get_string() );
        label_list[3]->set_string( label_list[4]->get_string() );
        // last element becomes/copies the edit_label's string
        label_list[4]->set_string( command );
        // execute Lua or system commands
        if(L) { if(luaL_dostring(L, command.c_str()) != 0) {std::cerr << "luaL_dostring : failed to execute Lua command";} }
        else  { system(command.c_str()); } // ShellExecute(0, "open", "cmd.exe", "/C ipconfig > out.txt", 0, SW_HIDE);
        // scrollbar_handle goes the opposite way of the text (text goes up, scrollbar_handle goes down)
        // TIP: scrollbars always start from the top
        if(discarded_string_list.size() > 0)//if(label_list.size() > label_list.capacity()) // if label capacity is surpassed // lets say the chatbox's height increases
        {
            scrollbar->set_handle_size( box->get_height() - edit->get_label()->get_height() ); // reduce handle size by label_height
            scrollbar->set_value( scrollbar->get_value() + 1 ); // bring scrollbar down as text goes up (increase scrollbar_value) // when a new label is appended to chat box, the scrollbar comes down (but you can scroll up to see the label at the top of the chatbox)  
        }    
    } // label_greater_or_equal
}
////////////
void Chatbox::write_from_file(const std::string& file_name) // not yet tested // writes a message to box from a file without the need for an edit 
{
    // open file
	std::ifstream file(file_name.c_str());
	if(!file.is_open()) {
	    std::cerr << "Could not open: " << file_name << std::endl;
		return;
	}	
	std::stringstream stream;
	stream << file.rdbuf(); // dump file contents into stringstream	
	std::vector<std::string> content_from_file = String::split(stream.str(), "\n"); // split each line
	// write to console box continously in loop (only 5 lines at a time)
	for(int i = 0; i < 5; i++) write(content_from_file[(content_from_file.size() - 1) - i]);//chat_box->write(content_from_file[0]);
	// close file
	file.close();
}
////////////
////////////
Box * Chatbox::get_box() const
{
    return box;
}
////////////
Edit * Chatbox::get_edit() const
{
    return edit;
}
////////////
Label * Chatbox::get_label(int index) const
{
    return label_list[index];
}
////////////
Scrollbar * Chatbox::get_scrollbar() const
{
    return scrollbar;
}
////////////
////////////
void Chatbox::on_scroll() // needs some work ...
{
    // use keyboard_input for now ...
    // if user scrolls up
    if(Keyboard::is_pressed(DOKUN_KEY_UP)) 
    {
        scrollbar->set_value(scrollbar->get_value() - 1);
        // show the discarded words
        //for(int i = 0; i < discarded_string_list.size(); i++)
        //label_list[ i ]->set_string( discarded_string_list[ i ] );
        // first element becomes/copies the last string from "discarded_string_list" // line below will cause crash if index is invalid
        /*label_list[ 0 ]->set_string( discarded_string_list[ discarded_string_list.size()-1 ] );
        label_list[ 1 ]->set_string( discarded_string_list[ discarded_string_list.size()-2 ] );
        label_list[ 2 ]->set_string( discarded_string_list[ discarded_string_list.size()-3 ] );
        label_list[ 3 ]->set_string( discarded_string_list[ discarded_string_list.size()-4 ] );
        label_list[ 4 ]->set_string( discarded_string_list[ discarded_string_list.size()-5 ] );*/
        //for(int i = 0; i < discarded_string_list.size(); i++)
        //{
            //std::string prev_str = label_list[ i ]->get_string();
            //label_list[ i ]->set_string( discarded_string_list[ i ] );
            //if(i != (discarded_string_list.size()-1)) std::string next_str = label_list[ i + 1 ]->get_string();
        //}             
    }
    //if user scrolls down
    if(Keyboard::is_pressed(DOKUN_KEY_DOWN)) 
    {
        scrollbar->set_value(scrollbar->get_value() + 1);
        // hide discarded words
    }
}
////////////
void Chatbox::on_enter()
{
    // if text (in edit) is empty, exit function (cannot submit empty text)_
    if(edit->get_label()->get_string().empty()) return;
    // if the "Enter" key is pressed
    if(Keyboard::is_pressed(DOKUN_KEY_RETURN)) {
         // add / append new label to chat_box (with a copy of edit_label's string)
        if(label_list.size() < label_list.capacity())  // if capacity has not yet been reached
        {
            label_list.push_back(new Label(edit->get_label()->get_string()));
            // clear edit_label, and reset cursor_x
            edit->get_label()->set_string("");
            edit->set_cursor_x(0); // reset cursor_x
            // no need to change scrollbar if label has not yet reached its capacity
        } // label_less
        else if(label_list.size() >= label_list.capacity()) {// capacity has been reached
            // store string that will be discarded (or hidden from chatbox) for later use
            //std::cout << "discarded from chatbox: " << label_list[0]->get_string() << std::endl;
            discarded_string_list.push_back(label_list[0]->get_string());
            // try to change the label string without removing anything
            label_list[0]->set_string( label_list[1]->get_string() ); // first element copies second element's string
            label_list[1]->set_string( label_list[2]->get_string() ); // second element copies third element's string and so on ...
            label_list[2]->set_string( label_list[3]->get_string() );
            label_list[3]->set_string( label_list[4]->get_string() );
             // last element becomes/copies the edit_label's string
            label_list[4]->set_string( edit->get_label()->get_string() );
            // clear edit_label, and reset cursor_x
            edit->get_label()->set_string("");
            edit->set_cursor_x(0); // reset cursor_x
            // scrollbar_handle goes the opposite way of the text (text goes up, scrollbar_handle goes down)
            // TIP: scrollbars always start from the top
            if(discarded_string_list.size() > 0)//if(label_list.size() > label_list.capacity()) // if label capacity is surpassed // lets say the chatbox's height increases
            {
                scrollbar->set_handle_size( box->get_height() - edit->get_label()->get_height() ); // reduce handle size by label_height
                scrollbar->set_value( scrollbar->get_value() + 1 ); // bring scrollbar down as text goes up (increase scrollbar_value) // when a new label is appended to chat box, the scrollbar comes down (but you can scroll up to see the label at the top of the chatbox)  
            }    
       } // label_greater_or_equal  
   } // enter pressed
}
////////////
////////////
