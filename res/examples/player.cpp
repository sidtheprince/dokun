#include "../include/player.h"
////////////
Player::Player() : current(nullptr), total_duration(0), repeat_type(0), played_once(false)
{
    // box
    box = new Box();
    box->set_size(300, 60);
    // slider
    slider = new Slider();
    slider->set_parent(* box);
    slider->set_size(slider->get_width(), 5);
    slider->set_ball_size(5);
    slider->set_relative_position(10, (box->get_height() - slider->get_height()) - slider->get_height()); // 5=slider_height is toppadding
    // timer
    timer = new Timer();
    // label_time
    label_duration = new Label();
    label_duration->set_parent(* box);
    label_duration->set_string("00:00:00");
    label_duration->set_relative_position((slider->get_relative_x() + slider->get_width()) + 16, (box->get_height() - 14));
    // label_title
    label_title = new Label();
    label_title->set_parent( * box);
    label_title->set_string("Title");
    label_title->set_relative_position(10, 5); // 5 is toppadding
    // label_artist
    label_artist = new Label();
    label_artist->set_parent( * box);
    label_artist->set_string("Artist");
    label_artist->set_relative_position(label_title->get_relative_x(), label_title->get_relative_y() + label_title->get_height());
    // label_genre
    //label_ = new Label();
    //label_->set_parent( * box);
    //label_->set_relative_position  
}
////////////
Player::~Player()
{}
////////////
void Player::draw()
{
    on_loop();
    box->draw(); // draw the box, child will be drawn automatically
}
////////////
void Player::add(const Audible& audible) //adds an audible to song_list
{
    song_list.push_back(&const_cast<Audible&>(audible));
    const_cast<Audible&>(audible).set_loop(false); // make sure all audibles' loop is set to false (to make it easier to use "repeat_one" and "repeat_all")
    if(!current) set_current(audible ); // set newly added song as current (ONLY if no current song exists)
    total_duration = total_duration + const_cast<Audible&>(audible).get_duration(); // increment total_duration
}
////////////
void Player::start()
{
    play();
}
////////////
void Player::play()
{
    if(!current) return; // no song found
    if(!in_player(*current)) {std::cout << "Song is not in player"; return;}
    if(!played_once) { if(repeat_type == 1) {set_current(*song_list[song_list.size()-1]);}} // repeat_one=1, set current to last song in song_list (ONLY if play() has never been called)
    on_play(); played_once = true;// callback
    current->play();
}
////////////
void Player::stop()
{
    if(!current) return; // no song found
    if(!in_player(*current)) {std::cout << "Song is not in player"; return;}
    on_stop(); // callback
    current->stop();
}
////////////
void Player::pause()
{
    if(!current) return; // no song found
    if(!in_player(*current)) {std::cout << "Song is not in player"; return;}
    on_pause(); // callback
    current->pause();
}
////////////
void Player::rewind()
{
    if(!current) return; // no song found
    if(!in_player(*current)) {std::cout << "Song is not in player"; return;}
    on_rewind();
    current->rewind();
}
////////////
void Player::fast_forward()
{
    if(!current) return; // no song found
    if(!in_player(*current)) {std::cout << "Song is not in player"; return;}
    on_forward();
}
////////////
////////////
// setters
void Player::set_current(const Audible& audible)
{
    for(int i = 0; i < song_list.size(); i++)
    {
        Audible * audio_obj = static_cast<Audible *>(song_list[i] ); 
        if(audio_obj == &audible)
        {
            current = audio_obj;
        }
    }
}
////////////
void Player::set_current(int index)
{
    if(index > (song_list.size()-1)){ std::cout << "Index " << index << " does not exist" << std::endl; return;}
    current = song_list[index];
}
////////////
void Player::set_repeat(int repeat)
{
    set_repeat_type(repeat);
}
////////////
void Player::set_repeat_type(int repeat_type) // 0:"repeat_none" 1:"repeat_one", 2:"repeat_all"
{
    this->repeat_type = repeat_type;
}
////////////
////////////
// getters
Audible * Player::get_current_song() const
{
    return current;
}
////////////
int Player::get_current_index() const
{
    for(int i = 0; i < song_list.size(); i++)
    {
        Audible * audio_obj = static_cast<Audible *>(song_list[i] ); 
        if(current == audio_obj)
        {
            return i;
        }
    }
    return -1;
}
////////////
int Player::get_total_duration() const
{
    return total_duration;
}
////////////
////////////
Box * Player::get_box() const
{
    return box;
}
////////////
Slider * Player::get_slider() const
{
    return slider;
}
////////////
////////////
// callbacks
void Player::on_loop()
{
    if(!current) return; // no current found  
    // if current song has stopped (is done playing)
    if(current->is_stopped()) { // put this in on_done_playing() function
       if(repeat_type != 1) 
       {
           // move on to the next song (default) ("repeat_one" must be off so it is not repeating one song continuously)
           if(song_list.size() > (get_current_index() + 1)) { current = song_list[get_current_index() + 1]; timer->reset(); play();}// may get a SEGMENT_FAULT for accessing invalid index (edit:FIXED)
           // repeat all songs ([repeat_all=2], repeat from the beginning (first song in Player.song_list))
           else if(get_current_index() == (song_list.size()-1)) { if(repeat_type == 2) { set_current(*song_list[0]); timer->reset(); play();} } //std::cout << current->get_title() << " is the last song in song_list\n";
       }
       // repeat one song ([repeat_one=1], will continuously play the first song added to the Player.song_list)
       if(repeat_type == 1) { timer->reset(); play(); } // [repeat_one=1], repeat current song (continuously in loop) // is looped, repeat_one=1 (current song will play continuously in a loop) // good
    }     // SEGMENT_FAULT
    // set maximum value to duration of "current" song
    slider->set_maximum_value(current->get_duration()); // in case the current changes guardia.ogg = 190secs, Oniku.ogg = 17secs
    // move the slider based on duration of song
    if(repeat_type == 0) { // not looped, [repeat_none=0] (each song will play ONLY "one time") // good // slider is reset to 0, so that seconds can be updated when song is done playing
        if(timer->increment() >= current->get_duration()) { timer->stop(); slider->reset(); } // if music has reached its end // this time, you do not re-start the timer (since song is not in a loop)
    }
    // increment slider value as long as audible is being played
    if(timer->increment() < current->get_duration() && current->is_playing()) { slider->set_value(timer->increment());} // if music has not reached its end, increment slider
    //std::cout << "Slider value: " << slider->get_value() << ", Timer(secs): " << timer->increment() << std::endl;  
    // update label_duration
    double seconds = (seconds >= 60 && (timer->increment() >= current->get_duration())) ? 0 : round(slider->get_value());            String seconds_string((int)seconds);      if(seconds_string.size() < 2) seconds_string = "0" + seconds_string.str(); // rounded so it does not produce a decimal number but rather, a whole number
    double minutes = (seconds < 60) ? 0 : seconds / 60;     String minutes_string((int)minutes);      if(minutes_string.size() < 2) minutes_string = "0" + minutes_string.str(); //double hours   = (seconds < 3600) ? 0 : seconds / 3600; String hours_string  ((int)hours  );      if(hours_string.size  () < 2) hours_string   = "0" + hours_string.str  ();
    if(seconds >= 60 && (timer->increment() >= current->get_duration())) seconds = 0; // make sure seconds does not go above 60 (1min = 60secs)
    label_duration->set_string(minutes_string.str() + ":" + seconds_string.str());//("00:00:00"); // hours_string not in use for now  /*hours_string.str() + ":" + */
    // update label title
    if(!label_title->get_string().empty()) label_title->set_string(current->get_title());
    if(label_title->get_string ().empty()) label_title->set_string(current->get_file ());
    // update label_artist
    label_artist->set_string(current->get_artist());
}
////////////
void Player::on_play()
{
    timer->start(); // start timer the moment song starts playing
}
////////////
void Player::on_stop()
{
    timer->stop(); // stop timer the moment song stops playing
}
////////////
void Player::on_pause()
{
    //timer->stop();//std::this_thread::sleep_for(std::chrono::seconds(2));//?? 
}
////////////
void Player::on_rewind() // skip to start
{
    //timer->reset();//?
}
////////////
void Player::on_forward() // skip to end
{}
////////////
////////////
// boolean
////////////
bool Player::in_player(const Audible& audible) const
{
    for(int i = 0; i < song_list.size(); i++)
    {
        Audible * audio_obj = static_cast<Audible *>(song_list[i]);
        if(audio_obj == &audible)
        {
            return true;
        }
    }
    return false;
}
////////////
////////////
////////////
