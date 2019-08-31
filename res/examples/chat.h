#ifndef _CHATBOX
#define _CHATBOX

#include "box.h"
#include "edit.h"
#include "scrollbar.h"

#ifdef __cplusplus
#include <string>
#include <vector>
class Chatbox { // can also act as a text displayer
public:
    Chatbox();
    ~Chatbox();
    void draw();
    void write(const std::string& text); // adds a string to the box without the need for a text_edit
    void write_from_file(const std::string& file_name); // adds a string from a file to the box without the need for a text_edit
    void execute(const std::string& command, lua_State *L = nullptr); // executes a lua command and draws the command string to the box without the need for a text_edit // or lua_dostring(command) (use String::execute(command));
    
    Box * get_box() const;
    Edit * get_edit() const;
    Label * get_label(int index) const;
    Scrollbar * get_scrollbar() const;
private:
    // callback
    void on_enter();
    void on_scroll();
    // properties
    Box * box;
    Edit * edit; // if you will not be using the edit, call chatbox->get_edit()->hide() to hide the edit
    std::vector<Label *> label_list;
    Scrollbar * scrollbar;
    std::vector<std::string> discarded_string_list; // strings that are hidden from the chatbox (should have a capacity, max_step)
};
#endif
#endif
