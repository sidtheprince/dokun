#include "../include/video.h"

Video::Video()
{}
///////////
Video::Video(const std::string& file_name)
{
	if(!load(file_name))
		Logger("Could not load video!");
}
///////////
Video::~Video()
{}
///////////
bool Video::load(std::string file_name, int vidfmt)
{
	return load_ogg(file_name, vidfmt);
}
///////////
int Video::load(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Video * video = *static_cast<Video **>(lua_touserdata(L, -1));
		video->load(lua_tostring(L, 2), lua_tonumber(L, 3));
	}
	return 0;
}
///////////
void Video::play()
{}
int Video::play(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Video * video = *static_cast<Video **>(lua_touserdata(L, -1));
		video->play();
	}
	return 0;
}
///////////
void Video::pause()
{}
int Video::pause(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Video * video = *static_cast<Video **>(lua_touserdata(L, -1));
		video->pause();
	}
	return 0;
}
///////////
void Video::stop()
{}
int Video::stop(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Video * video = *static_cast<Video **>(lua_touserdata(L, -1));
		video->stop();
	}
	return 0;
}
///////////
void Video::record(int source)
{
	switch(source)
	{
		case 0:
		    record_screen();
			break;
		case 1:
		    record_camera();
			break;
		default:
            record_screen();
            break;
	}
}
int Video::record(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Video * video = *static_cast<Video **>(lua_touserdata(L, -1));
		video->record(lua_tonumber(L, 2));
	}
	return 0;
}
///////////
void Video::screenshot(int window_width, int window_height, std::string file_name)
{
    unsigned char * data = new unsigned char[window_width * window_height * 4]; // 4 channels per pixel (RGBA)
    if(data == nullptr) {
		Logger("Could not capture window");
		return;
	}
#ifdef DOKUN_OPENGL
    glReadPixels(static_cast<GLint>(0), static_cast<GLint>(0),
		static_cast<GLsizei>(window_width), static_cast<GLsizei>(window_height),
		GL_RGBA, GL_UNSIGNED_BYTE, static_cast<GLvoid *>(data));
#endif
    FILE * file = fopen("out.png", "wb");
    if(file == nullptr)
	{
		Logger("Could not capture window");
		delete data;
		return;
	}
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr); // write_struct
    if (!png_ptr) {
		Logger("Could not capture window");
		delete data;
		fclose(file);
		return;
	}
    png_infop info_ptr = png_create_info_struct(png_ptr); // info_struct
    if (!info_ptr) {
		Logger("Could not capture window");
        png_destroy_write_struct(&png_ptr, nullptr);
        delete data;
        fclose(file);
        return;
	}
    png_infop end_ptr = png_create_info_struct(png_ptr);
    if (!end_ptr) {
		Logger("Could not capture window");
        png_destroy_write_struct(&png_ptr, nullptr);
        delete data;
        fclose(file);
		return;
	}
    if (setjmp(png_jmpbuf(png_ptr)))  // png error stuff
	{
		Logger("Could not capture window");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        delete data;
        fclose(file);
		return;
    }
	// init png writing
	png_init_io(png_ptr, file);
    // write header - store color, depth, etc.
	int bit_depth  = 8;
	int color_type = 6;
    png_set_IHDR(png_ptr, info_ptr, window_width, window_height,
        bit_depth, color_type, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	// write all image info
	png_write_info(png_ptr, info_ptr);
	//--------------------------
	// Row size in bytes.
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	// row pointer - create output data
	png_bytep * data_p = new png_bytep[window_height];
	if (!data_p)
	{
		Logger("Could not capture window");
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(file);
	    return;
    }
	for(int i=0; i < window_height; ++i)
	{
		data_p[window_height - 1 - i] = (png_byte*)data + i * rowbytes;
	}
	//---------------------------
	png_write_image(png_ptr, (png_bytepp)data_p); // write byte data
	png_write_end(png_ptr, nullptr); // stop writing
    if (png_ptr && info_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);	// destroy write struct
	}
	Logger("Screenshot taken");
	delete data;
	fclose(file); // close file
}
int Video::screenshot(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Video * video = *static_cast<Video **>(lua_touserdata(L, -1));
		video->screenshot(lua_tonumber(L, 2), lua_tonumber(L, 3));
	}
	return 0;
}
///////////
///////////
int Video::get_width()
{
	return width;
}
int Video::get_width (lua_State *L)
{
    return 1;
}
///////////
int Video::get_height()
{
	return height;
}
int Video::get_height(lua_State *L)
{
    return 1;
}
///////////
void * Video::get_data()
{
	return &data[0];
}
void * Video::get_audio_data()
{
    return nullptr;
}
///////////
double Video::get_fps()
{
	return fps;
}
int Video::get_fps(lua_State *L)
{
    return 1;
}
///////////
int Video::get_format()
{
	return format;
}
int Video::get_format(lua_State *L)
{
    return 1;
}
///////////
///////////
bool Video::load_ogg(const std::string& file_name, int vidfmt)
{
/*	
    //TheoraVideoManager *mgr=new TheoraVideoManager();
	const THEORAPLAY_VideoFrame  * video = 0;
    const THEORAPLAY_AudioPacket * audio = 0;
	THEORAPLAY_Decoder * decoder = THEORAPLAY_startDecodeFile(file_name.c_str(),
        20, // maxframes
        static_cast<THEORAPLAY_VideoFormat>(vidfmt));
	while(THEORAPLAY_isDecoding(decoder)) {
		video = THEORAPLAY_getVideo(decoder);
		if(video != nullptr)
		{
			Logger("Retrieved video frame " + std::to_string(video->playms) + "ms"); // 120 frames
			Logger(std::to_string(video->fps) + "= fps\n"    +  // sample.ogg is 30 frames per second
			    std::to_string(video->width ) + "= width\n"  +  // 320
				std::to_string(video->height) + "= height\n" +  // 240
				std::to_string(video->format) + "= format\n" +  // 0
			    String(pixels).str() + "\n"
				);
			// get width, height, frames, etc.    3966ms(last)
			playms = video->playms;
			fps    = video->fps;
			width  = video->width ;
			height = video->height;
			format = video->format;
			pixels = video->pixels;
			data.push_back(video->pixels); // 120 = size
			std::cout << data.size() << "= size of data\n";
			// free the video
			THEORAPLAY_freeVideo(video);
		} // if
        audio = THEORAPLAY_getAudio(decoder);
        if(audio != nullptr)
        {
			Logger("Retrieved " + std::to_string(audio->frames) + " frames of audio " + std::to_string(audio->playms) + "ms");
            THEORAPLAY_freeAudio(audio);
        } // if
		if (!video && !audio)
            dokun::sleep(10);
	} // while

	if (THEORAPLAY_decodingError(decoder))
	{
		Logger("Error decoding theora!");
		return false;
	}
	THEORAPLAY_stopDecode(decoder);
*/	
	return true;
}
///////////
void Video::record_screen()
{}
void Video::record_camera()
{}
///////////
void Video::png_to_theora()
{}
///////////
///////////
int Video::new_(lua_State *L)
{
	std::string file_name;
	if(lua_type(L, -1) == LUA_TSTRING)
	{
		file_name = lua_tostring(L, -1);
	}
	lua_settop(L, 0); // clear stack

	lua_createtable(L, 0, 0); // create table
	lua_getglobal(L, "Video");
	lua_setmetatable(L, 1); // set metatable
	// set userdata
    Video ** video = static_cast<Video **>(lua_newuserdata(L, sizeof(Video *)));
    if(!file_name.empty())
	{
		*video = new Video(file_name);
	}
    if(file_name.empty())
	{
		*video = new Video();
	}
	lua_setfield(L, 1, "udata");
	// return table
	if(lua_istable(L, -1))
	{
		return 1;
	}
	lua_pushnil(L);
	return 1;
}
