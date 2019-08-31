#ifndef _AUDIO_PLAYER
#define _AUDIO_PLAYER

#include "box.h"
#include "slider.h"
#include "audio.h"
#include "timer.h"
#ifdef __cplusplus // classes are only supported in C++

#define REPEAT_NONE 0 // plays each song one time
#define REPEAT_ONE  1 // repeats current song in a continuous loop
#define REPEAT_ALL  2 // repeats all songs in a continuous loop

class Player {// audio player GUI class
public:
    Player();
    ~Player();
    void draw();
    void add(const Audible& audible); //adds an audible to song_list
    void start();
    void play();
    void stop();
    void pause();
    void rewind();
    void fast_forward();
    // setters
    void set_current(const Audible& audible);
    void set_current(int index);
    void set_repeat(int repeat);
    void set_repeat_type(int repeat_type); // 0:"repeat_none" 1:"repeat_one", 2:"repeat_all"
    // getters
    Audible * get_current_song() const;
    int get_current_index() const;
    int get_total_duration() const; // returns total_duration of all songs in song_list (in secs)
    Box * get_box() const;
    Slider * get_slider() const;
    // boolean
    bool in_player(const Audible& audible) const; // returns true if song is found in song_list otherwise returns false
private:
    //GUI: box(parent), slider(child - slider->set_size(slider->get_width(), 5), slider->set_ball_width(5)), label0(song_title, child), label1(song_artist)
    // load audio from directory based on format: ogg, flac, etc.; load audio from playlist file
    // get active song (selected), whether its playing or not
    // get playing song, make sure no other audio_obj is playing
    // get songs from song_list
    // on_press play_button, play_audio and change texture to pause_button
    // on press foward_button, set value to 0 or starting point
    // on press backward_button, set value to end point
    void on_loop();
    void on_play();
    void on_stop();
    void on_pause();
    void on_rewind();
    void on_forward();
    int total_duration; // duration of all songs combined
    int repeat_type; // 2:repeats_all, 1:repeat_one, 0:repeat_off
    bool played_once; // checks if Player::play() has been called at least once
    Audible * current; // current or selected audible
    std::vector<Audible *> song_list;
    Box * box;
    Slider * slider;
    Timer * timer;
    Label * label_duration;
    Label * label_title;
    Label * label_artist;
    //Label * label_;
};
#endif
#endif
// player.cpp -ldokun
/*
Usage:

	Player * player = new Player();
	player->get_box()->set_position(window.get_client_width()/2, 186);
	player->add(* new Music("res/momentum.wav")); // 68s //("res/guardia.ogg"));
	player->add(* new Music("res/Oniku.ogg")); // 17s
	player->add(* new Music("res/cup_of_lartey.ogg")); // 51s
	player->set_repeat_type(0); // 0=repeat_none, 1=repeat_one, 2=repeat_all
	player->play();
	
	
	std::cout << "Total duration of all songs in Player: " << player->get_total_duration() << "s" << std::endl;
    std::cout <<"The title is: " << player->get_current_song()->get_title() << std::endl;
    std::cout <<"The artist is: " << player->get_current_song()->get_artist() << std::endl;
    std::cout <<"The genre is: " << player->get_current_song()->get_genre() << std::endl;
    std::cout <<"The album is: " << player->get_current_song()->get_album() << std::endl;
    std::cout <<"The track_number is: " << player->get_current_song()->get_track_number() << std::endl;
    std::cout <<"The date is: " << player->get_current_song()->get_date() << std::endl;
    //std::cout <<"The cover_art is: " << String((unsigned char *)player->get_current_song()->get_cover()) << std::endl;
    
    
    player->draw();

struct Playlist
{
    std::string title;
    std::string artist;
    std::string genre;
    double length;
    std::vector<Song*> songList;
    void addSong( Audio * audio ) {
        songList.push_back( song );
        length += song->length;
    }
};
// maybe combine music and sound into a single class called, noise or audible. audio_object, audio_engine, audio_player (GUI)
*/
