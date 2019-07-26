#include "../include/editor.h"

Editor::Editor()
{
	open();
}
////////////
Editor::~Editor()
{}
////////////
bool Editor::status (false);
////////////
bool Editor::create()
{
   // create ui elements
	//Label * label[2];
	//label[0] = new Label();
	//label[1] = new Label();
	
	//Edit * edit[2];
	//edit[0] = new Edit();
	//edit[1] = new Edit();
	
	//Button * button = new Button();
    //Image * browse_icon = new Image(); // [...]
	
	// set and get input values
    //label[0]->set_string("Project Name:"); 
	// get project_name from edit
	std::string project ;//= edit[0]->get_text();
	//---
	//label[1]->set_string("Path:");
	// use default path
	//edit [1]->set_text(get_default_path()); 
	// 'path' can be changed by the user
	std::string path ;//= edit[1]->get_text();
	
	
	
	//button->set_text("Create"); 
	//if(button->is_pressed()) 
	//{ 
        //run this entire function 
    //}
	return create(project, path);	
}
////////////
bool Editor::create(const std::string& project, std::string path)
{
	cache(); // save old project path
	// get user input from text_edit (project name and path)
	// specify project name
	if(project.empty())
	{	
        std::cerr << "dokun: Project must have a name" << std::endl; // Tooltip("You must enter a name for the project!");		 
		return false;
	}
	// target path unspecified
    if(path.empty())
	{
		// check default path (to see if project folder and file exists)
        #ifdef _WIN32
		    DWORD dwAttrib = GetFileAttributes(std::string(get_default_path() + "\\" + project).c_str());
            if (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
			{
				std::ifstream file(std::string(get_default_path() + "\\" + project + "\\" + project + ".dokun").c_str());
				if(file.good()) // if it exists, then no need to create it  
	            {
	                if(MessageBox(nullptr, 
					static_cast<LPCTSTR>(
					std::string("Project '" + project + "' found in " + get_default_path() + "\\" + project + "\nWould you like to load?").c_str()
					), static_cast<LPCTSTR>("dokun"), static_cast<UINT>(MB_YESNO)) == IDYES) // MB_YESNOCANCEL, MB_OKCANCEL
					{
						load_file(get_default_path() + "\\" + project + "\\" + project + ".exe");
					}	
                    return false;					
	            }
			}
		#endif
        #ifdef __gnu_linux__
            DIR * dir = opendir(std::string(get_default_path() + "/" + project).c_str());
            if (dir) // folder in existance, already in default path (c:/documents)
            {
				std::ifstream file(std::string(get_default_path() + "/" + project + "/" + project + ".dokun").c_str());
				if(file.good()) // if it exists, then no need to create it  
	            {
					std::cerr << "dokun: Project '" << project << "' found in " << get_default_path() + "/" + project << std::endl;
	                std::cout << "Would you like to load in the editor?" << std::endl;
					return false;
	            }    		
                closedir(dir);
            }		
		#endif			
        // specify path		
		std::clog << "dokun: No path specified. Using default path 'Documents'" << std::endl;
		#ifdef _WIN32
		    path = project_data.default_path + "\\" + project; // Documents/{project_name}
		#endif
		#ifdef __gnu_linux__
		    path = project_data.default_path + "/" + project;
		#endif
	}
	// create path
	#ifdef _WIN32
	    if(CreateDirectory  (path.c_str(), nullptr) == 0)
		{
			return false;
		}
	#endif
	#ifdef __gnu_linux__
		if(mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) 
		{
			return(false);
		}
	#endif	
	// generate project file (.dokun)
	std::string dokun_ext;
#ifdef _WIN32	
	dokun_ext = (path + "\\" + project + ".dokun");
#endif
#ifdef __gnu_linux__	
    dokun_ext = path + "/" + project + ".dokun";
#endif
	std::ofstream dokun_file(dokun_ext.c_str());
    if (!dokun_file.is_open()) 
	{
		std::cerr << "dokun: Could not generate " << project + ".dokun" << std::endl;
		return false;
	}
	dokun_file << "project_name= \"" << project << "\"" << "\n";
	dokun_file << "path= \"" << path << "\"" << "\n";
	dokun_file << "system= " << 
	#ifdef _WIN32
	"Windows"
	#endif
	#ifdef __gnu_linux__
	"Linux"
	#endif
	#ifdef __ANDROID__
	"Android"
	#endif
	<< "\n";
	dokun_file.close();
	//std::clog << "dokun: " << dokun_file + " generated" << std::endl;	
	// generate lua script
	std::string lua_ext; 
#ifdef _WIN32	
	lua_ext = path + "\\" + "main.lua";
#endif	
#ifdef __gnu_linux__
    lua_ext = path + "/" + "main.lua";
#endif
	std::ofstream lua_file(lua_ext.c_str());
	if(!lua_file.is_open())
	{
		std::cerr << "dokun: Could not generate main.lua" << std::endl;
		return false;
	}
	lua_file << "require (\"dokun\")" << std::endl;
	lua_file << "" << std::endl;
	lua_file << "WINDOW_MODE  = 0" << std::endl;
	lua_file << "FULL_SCREEN  = 1" << std::endl;
	lua_file << "" << std::endl;	
	lua_file << "window = Window:new()" << std::endl;
	lua_file << "window:create(\"Hello World\", 800, 600, WINDOW_MODE)" << std::endl;
	lua_file << "window:show()" << std::endl;
	lua_file << "" << std::endl;
	lua_file << "while window:is_open() do " << std::endl;
	lua_file << "    window:clear(32, 32, 32)" << std::endl;
	lua_file << "    window:update()" << std::endl;
	lua_file << "end" << std::endl;
	lua_file << "dokun:close()" << std::endl;
	//lua_file << "" << std::endl;

	lua_file.close();
	// set project name
	project_data.name = project;
	// set project path
    project_data.path = path;	
	// store in cache
	recent_list.push_back(path);
	return true;
}
////////////
bool Editor::load_file(std::string file_name)
{
	// get the file name ..
	if(file_name.empty())
	{
	    File * fd = new File();
	    file_name = fd->dialog(0); // or DOKUN_DIALOG_OPEN
        delete fd;
	    fd = nullptr;
	}
	
	if(file_name.empty())
	{
		// no file selected ..
		std::cerr << "dokun: No file selected" << std::endl;
		return false;
	}
	// open file ..
	std::ifstream file(file_name.c_str());
	if(!file.is_open())
	{
		std::cerr << "dokun: Could not open " << file_name << std::endl;
		return false;
	}
	// check extension ..
	if(file_name.substr(file_name.find_last_of(".") + 1) != "dokun")
	{
		std::cout << "dokun: No project found " << std::endl;
		return false;
	}
	// do stuff with the file ..
	
    // store project info
	project_data.name = File::no_extension(File::file_from_directory(file_name));
	project_data.path = File::directory_from_file(file_name);
	// store project in cache
	recent_list.push_back(File::directory_from_file(file_name));
	file.close();
	return true;
}
////////////
////////////
bool Editor::load_directory()
{
	std::string path = "";
	// select folder
	#ifdef _WIN32
		TCHAR fpath[MAX_PATH];
        //const char * path_param = saved_path.c_str();
	
        BROWSEINFO bi = { 0 };
        bi.lpszTitle  = ("Browse project folder...");
        bi.ulFlags    = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
        bi.lpfn       = NULL;
        //bi.lParam     = (LPARAM) path_param;

        LPITEMIDLIST pidl = SHBrowseForFolder (&bi);

        if (pidl != 0)
        {
            //get the name of the folder and put it in path
            SHGetPathFromIDList (pidl, fpath);
            //free memory used
            IMalloc * imalloc = 0;
            if (SUCCEEDED(SHGetMalloc(&imalloc)))
            {
                imalloc->Free (pidl);
                imalloc->Release ();
            }
			
			path = fpath;
		}
	#endif
	#ifdef __gnu_linux__
	#endif
	// no path selected
	if(path.empty())
	{
		std::cout << "dokun: Could not load an existing project" << std::endl;
		return false;
	} 
    // search directory (for ".dokun" file)
	std::vector<std::string> file_list; // project file
    #ifdef _WIN32
	    char search_path[200];
      	sprintf(search_path, "%s/*.dokun*", path.c_str());

		WIN32_FIND_DATA fd; 
        HANDLE hFind = FindFirstFile(search_path, &fd); 
        if(hFind != INVALID_HANDLE_VALUE) 
	    {
            do 
			{ 
                if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
			    {
                    file_list.push_back(fd.cFileName);
                } 
            } while(FindNextFile(hFind, &fd)); 
            FindClose(hFind); 
        } 
	#endif
	#ifdef __gnu_linux__
	    DIR *dir = opendir (path.c_str());
        struct dirent * ent;
        if (dir != nullptr) 
	    {
            while ((ent = readdir (dir)) != nullptr)   /* print all the files and directories within directory */
            {
                std::cout << ent->d_name << std::endl;
			    file_list.push_back(ent->d_name);
            }
        closedir (dir);
        }
	#endif
	// invalid folder
	if(file_list.empty())
	{
		std::cerr << "dokun: No project found in " << path << std::endl;
		return false;
	}
	// multiple project in one folder
	if(file_list.size() > 1)
	{
		std::cerr << "dokun: Multiple projects found in " << path << std::endl;
		return false;
	}
	// load the project
	for(int i = 0; i < file_list.size(); i++)
	{
		load_file(path + "/" + file_list[i]); // load dokun files
		std::cout << "dokun: [100%] Loaded " << path + "\\" + file_list[i] << std::endl;
	}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             
	return true;
}
////////////
bool Editor::load_sprite()
{
	File * file = new File();
	std::string filename = file->dialog(0);
	if(filename.empty() == false)
	{
		Texture * texture = new Texture(String::no_spacing(filename));
		Sprite * sprite = new Sprite(* texture);
		sprite->set_position(500, 250);
		if(level) level->add(sprite); // add sprite to level
	}
    delete file;	
    return true;
}
////////////
bool Editor::load_sprite_custom()
{
	// FILE_DIALOG
	// create file dialog widget
	file_dialog = new Widget();
	file_dialog->set_size(500, 300);
	// position at center of window
	file_dialog->set_position((window->get_width() - file_dialog->get_width()) / 2,
	    (window->get_height() - file_dialog->get_height()) / 2);
	file_dialog->set_title_bar(true);
	file_dialog->set_outline  (true);
	file_dialog->set_draggable(true);
	// FILE_LIST
	// file list containing file names
	file_list = new List();
	// adjust item size to match dialog size
	file_list->set_width(file_dialog->get_width());
	file_list->set_parent(*file_dialog);
	// BUTTON
	// if open is pressed, open the file within the editor
	open_button = new Button("Open");
	open_button->set_parent(*file_dialog);
	// if cancel is pressed, make sure to set visibility to false so it is not drawn
	cancel_button = new Button("Cancel");
	cancel_button->set_parent(*file_dialog);
	// FILE_EDIT
	// get files in directory
	std::string target_dir = System::get_current_dir();
	std::vector<std::string> directory = File::getdir(target_dir);
	for(int i = 0; i < directory.size(); i++)
	{
		// store filenames in label
		Label * label = new Label();
		label->set_string(directory[i]);
		// create a list item
		Widget * widget = new Widget(); widget->set_color(160, 160, 160);
		//assign label to list item 
		widget->set_label(*label);
		// add item to list
		file_list->add(*widget);
		//////////////////////////
		//std::cout << directory[i]<< "\n";
			std::string extension = String::lower(directory[i].substr(directory[i].find_last_of(".") + 1));
			if(extension == "png")
			{
				Texture * texture = new Texture();
				if(texture->load(target_dir + "/" + directory[i])) // load success
				{
		            Sprite * sprite = new Sprite(* texture);
		            sprite->set_position((rand() % window->get_width())+1, (rand() % window->get_height())+1);
		            level->add(sprite); // add sprite to level
				}
			}		
	}
	// add to a level so it is drawn automatically
	if(level) level->add(file_dialog);//label->draw();	
    return true;
}
////////////
bool Editor::load_level()
{
	File * file = new File();
	std::string file_name = file->dialog(0);
	if(file_name.empty() == false)
	{
		std::string extension = String::lower(file_name.substr(file_name.find_last_of(".") + 1));
		if(extension == "lua" || extension == "level")
		{
		    if(level) level->load(state, file_name); // add sprite to level
		}
	}
    delete file;
    return true;		
}
////////////
void Editor::unload()
{
	project_data.name.clear();
}
////////////
void Editor::open()
{
    on_open();
}
////////////
void Editor::close()
{
	on_close();
}
////////////
void Editor::execute()
{ 
    while(window->is_open())
	{
		window->set_viewport(window->get_client_width(), window->get_client_height());
		window->clear(32, 32, 32);
		// Call Draw function
		draw();
		// Call Update function
		update();
	}
	if(!window->is_open()) // on close
		close();
}
////////////
void Editor::update()
{
	window->update();
}
////////////
void Editor::cache() // creates a cache file on closing application
{
	// write recent_proj
        std::ofstream file("cache.txt", std::ios::app);
		if(in_cache())
		{
			return;
		}
		if(!in_cache())
		{
			clear();
			file << get_project_path() << std::endl;
		    file.close();
		}		
	/*
	// read cache
	int recent_files = 0; // number of recent projects
	std::string line;
	std::ifstream rfile("cache.txt");
	if(rfile.is_open()) // if the file exists
	{
		while(std::getline(rfile, line)) // while not at end of file
		{
		    recent_files = recent_files + 1;
		}
	}
	rfile.close();
	*/
	/*
	if(recent_files > (10 * 2)) // recent file list is full
	{
		bool project_not_in_recent_list = false;
		for(int i = 0; i < recent_list.size(); i++)
		{
			if(recent_list[i] != get_project()) // current_project is not in cache
                project_not_in_recent_list = true;		    
		}
		// replace the first recent_project with the current_project
		if(project_not_in_recent_list) recent_list[0] = get_project();
	}*/
}
////////////
void Editor::clear()
{
	recent_list.clear();
}
////////////
void Editor::play() // run executable
{  
#ifdef __windows__
	Process process("\"" + get_project_path() + "\\" + get_project() + ".exe" + "\"", "");
#endif	
#ifdef __gnu_linux__
    std::string program = get_project_path() + "/" + get_project();
	Process process(program, program);
#endif		
} // run
////////////
void Editor::run() // run script (main.lua)
{
#ifdef __windows__
	Script script(get_state(), get_project_path() + "\\" + "main.lua");
#endif
#ifdef __gnu_linux__
	Script script(get_state(), get_project_path() + "/" + "main.lua");
#endif			
}
////////////
void Editor::stop() // terminates process
{
#ifdef __windows__
	TerminateProcess(GetCurrentProcess(), 0);
#endif
#ifdef __gnu_linux__
#endif
}
////////////
void Editor::build()
{ 
    if(!has_project())
	{
		Logger("Build failed : Project not loaded");
		return;
	}
	if(has_project())
	{
		if(clean() != 0)
		{
			Logger(String("[100%] Cleaned ") + get_project());
		}
	// build executable with srlua
	Process process;
#ifdef _WIN32
	    if(process.create("glue.exe", " srlua.exe "  + ("\"" + get_project_path() + "\\main.lua" + "\"") + " " + ("\"" + get_project_path() + "\\main.exe" + "\"")) == 0) {   
	        Logger("Build failed : Could not create process");
			return;}
		 Logger(String("[100%] Build success [target]: ") + get_project());
#endif
#ifdef __gnu_linux__
	    std::string program  = "./glue";
		std::string argument = " ./srlua " + get_project_path() + "/main.lua" + " " + get_project_path() + "/main";
		if(process.create("./glue", program + argument) == 0) {
		    Logger("Build failed : Could not create process");
			return;}
		// get file permission		
        if(chmod(std::string(get_project_path() + "/main").c_str(), S_IRWXU| // owner
		    S_IRWXG| // group
		    S_IRWXO| // others
			S_ISUID|S_ISGID) != 0) // set user + group ID on execution		
		{
			Logger("Build failed : Could not change file permission");
			return;
		}
		//  to ensure that the S_ISUID and S_ISGID bits are set
        struct stat buffer;
		int status = stat(std::string(get_project_path() + "/main").c_str(), &buffer);
		if(status == -1)
			Logger("Could not retrieve file status");
		Logger(String("[100%] Build success [target]: ") + get_project());
#endif
	// rename executable (to project name)		
#ifdef _WIN32
	    if(MoveFile(static_cast<LPCTSTR>(std::string(get_project_path() + "\\main.exe").c_str()), static_cast<LPCTSTR>(std::string(get_project_path() + "\\" + get_project() + ".exe").c_str())) == 0)
		    Logger("Could not rename project");
#endif
#ifdef __gnu_linux__
	    if(std::rename(std::string(get_project_path() + "/main").c_str(), std::string(get_project_path() + "/" + get_project()).c_str()) != 0)
	        Logger("Could not rename" + std::string(get_project_path() + "/main") + " to " + get_project());
#endif
	}
}
////////////
bool Editor::clean()
{ 
#ifdef _WIN32
	if(DeleteFile(static_cast<LPCTSTR>(std::string(get_project_path() + "\\" + get_project() + ".exe").c_str())) == 0) // error (file probably does not exist or is opened in another program)
    {
        Logger("Deletion of file has failed");		
		return false;
	}			
#endif
#ifdef __gnu_linux__
	if(std::remove(std::string(get_project_path() + "/" + get_project() + "").c_str()) == -1)
	{
		Logger("Deletion of file has failed");	
		return false;
	}
#endif
	return true;
}
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
bool Editor::get_status()
{
	return status;
}
////////////
std::string Editor::get_project()
{
	return project_data.name;
}
////////////
std::string Editor::get_project_path()
{
	return project_data.path;
}
std::string Editor::get_default_path()
{
	return project_data.default_path;
}
////////////
WINDOW * Editor::get_window()const
{
	return window;
}
////////////
int Editor::get_error()
{
	return error;
}
////////////
lua_State * Editor::get_state()const
{
	return state;
}
////////////
bool Editor::is_open()
{
	return (window->is_open() != 0);
}
////////////
bool Editor::has_project()
{
	return (project_data.name.empty() == 0);
}
////////////
bool Editor::in_cache() // project_path with extension
{
	std::ifstream file((get_project_path() + "/" + get_project() + ".dokun").c_str()); std::string line; std::vector<std::string> linev;
	if(file.is_open())
	{
		while(std::getline(file, line))
		{
            linev.push_back(line);
		}
	}
	for(int i = 0; i < linev.size(); i++)
	{
		for(int j = 0; j < recent_list.size(); j++)
	    {
			if(!linev[i].empty())
			{
	            if(linev[i] == recent_list[j])
		        {
			        return true;
		        }
			}
	    }
	}
	return false;
}
////////////
void Editor::on_open()
{
	if(status == 1) // editor initialized previously
	{
		Logger("Editor has already been started");
		return;
	}
	// default mode
	editor_data.mode = ("edit");
	// default path for every project
#ifdef __WIN32__
    TCHAR path[MAX_PATH];
    if(SUCCEEDED(SHGetFolderPath(nullptr, CSIDL_PERSONAL, nullptr, 0, path))) 
	{
		project_data.default_path = static_cast<std::string>(path); // C:/Users/{username}/Documents
	}
#endif
#ifdef __gnu_linux__
	struct passwd * pw = getpwuid(getuid());
    const char * homedir = pw->pw_dir;
	project_data.default_path = static_cast<std::string>(homedir) + "/Documents"; //  /home/{username}/Documents
#endif
	// initialize dokun
    engine = new Engine();
	engine->open();
	if(engine->get_status() == 0)	
	{
		Logger("Engine failed to start");
		status = 0;
	}
	// initialize lua
	state = luaL_newstate();
	if(state == nullptr)
	{
		Logger("Lua failed to start");
		status = 0;
	}
	luaL_openlibs(state);
	// init level
	level = new Level();
	// create ui
    setup();
	window->show();
	// get recent files
	/*std::ifstream file("cache.txt"); if(file.is_open()){ while(!file.eof())   }*/
	//cache(); // save recent files if not saved yet
	// proof that editor has been initialized
	status = true;
}
////////////
void Editor::on_close()
{
	// request to save project, if not yet saved
	cache(); // save recent files
	// close editor (window)
	if(window != nullptr)
	{
	    window->destroy();
	    window = nullptr;
	#ifdef DOKUN_DEBUG
        Logger("Window destroyed");
    #endif	
	}
	// close dokun
	engine->close();
	#ifdef DOKUN_DEBUG
	    Logger("Engine closed");
	#endif
	// close lua (state)
	lua_close(state);
	#ifdef DOKUN_DEBUG
	    Logger("Lua closed");
	#endif
	if(get_status() != 0)
	{
		status = false;
	}	
}
////////////
void Editor::setup(void) // create then initialize Window and UI Elements
{
    // initialize window
	window = new WINDOW();
#ifdef __WIN32__
	/*window->set_style (DOKUN_POPUP | DOKUN_BORDER | DOKUN_SYSMENU);
	window->set_position(200, 200);*/ // SEG FAULT
	//--------------------------------------------------------------
	/*SetWindowLong(window->get_handle(), GWL_STYLE, 0 );  // Without 1 point border = white rectangle 0-WS_POPUP
    SetWindowPos(window->get_handle(), 0, 150, 100, 250, 250, SWP_FRAMECHANGED);*/
#endif
	window->create("Editor", 1280, 720, 0);
	//window->show();	
#ifdef __gnu_linux__
    //window->set_style(DOKUN_DOCK);
#endif	

	// titlebar	- only when in dock mode (DOKUN_DOCK)
	titlebar = new Widget();
	titlebar->set_position(0, 0);
	titlebar->set_size(window->get_width(), 20);
	titlebar->set_color(0, 51, 102);
	// titlebar closebutton (closebutton is its own widget)
	titlebar_close_button = new Widget();
	titlebar_close_button->set_parent(*titlebar);
	titlebar_close_button->set_size(20, titlebar->get_height() - 4); // bottom_padding
	titlebar_close_button->set_position(titlebar->get_x() + (titlebar->get_width() - titlebar_close_button->get_width()) - 5, titlebar->get_y() + 2);
	// titlebar text
	/*titlebar_text = new Label();
	titlebar_text->set_parent(*titlebar);
	titlebar_text->set_string("Editor");
	titlebar_text->set_scale(0.6, 0.6);*/
	// titlbar hide - only when in normal window mode
#ifdef __gnu_linux__ 	
	if(window->get_style() != DOKUN_DOCK) titlebar->hide();
#endif	
	//===============================================
	// menubar
	menubar = new Menubar();
	if(titlebar->is_visible()) menubar->set_position(0, titlebar->get_y() + titlebar->get_height()); // menubar would be underneath custom titlebar
	//menubar->set_size(window->get_client_width(), 20);
    menubar->add("File"); menubar->get_menu(0)->get_label()->hide();// file
    menubar->add("Edit"); // edit
    menubar->add("View"); // view
	// submenu
	/*menubar->sub("File", "New"); // new
	menubar->sub("File", "Open"); // open
	menubar->sub("File", "Save"); // save
	menubar->sub("File", "Save As"); // save ass
	menubar->sub("File", "Print"); // 
	menubar->sub("File", "Export"); // 
	menubar->sub("File", "Recent"); // 
	menubar->sub("Edit", "Undo"); // 
	menubar->sub("Edit", "Redo"); // 
	menubar->sub("Edit", "Cut"); // 
	menubar->sub("Edit", "Copy"); // 
	menubar->sub("Edit", "Paste"); // 
	menubar->sub("Edit", "Select"); // 
	menubar->sub("Edit", "Select All"); // 
	menubar->sub("View", "Zoom In"); // 
	menubar->sub("View", "Zoom Out"); //
	//===============================================
	*/
/* 	
	// toolbar or sidebar
	// sidebar	
	sidebar = new Widget();
	sidebar->set_position(0, 0);
	sidebar->set_size(20, window->get_height()-menubar->get_height());
	sidebar->set_color(0, 51, 102, 255);
	mainbar = new Widget();
	mainbar->set_size(500, window->get_height()-menubar->get_height());
	mainbar->set_position(window->get_width()-sidebar->get_width(), 0); // show sidebar while hiding mainbar at window_width
	sidebar->set_parent(*mainbar);
	
	mainbar->set_position(window->get_width()-sidebar->get_width(), 
	      menubar->get_height());
	// tool button
	play_button  = new Widget();
	play_button->set_as_icon(true);
	Image  play_image("res/Play.png");
	play_button->set_image(play_image);
	play_button->set_parent(*mainbar);
	play_button->set_position(20, 20);
	*/
	//pause_button = new Widget();
	//pause_button->set_as_icon(true);
	//Image  pause_image("res/Pause.png");
	//pause_button->set_image(pause_image);
	//pause_button->set_parent(*mainbar);
	
	//stop_button  = new Widget();
	//stop_button->set_as_icon(true);
	//Image  stop_image("res/Stop.png");
	//stop_button->set_image(stop_image);
	//stop_button->set_parent(*mainbar);
	
	//run_button   = new Widget();
	//run_button->set_as_icon(true);
	//Image  run_image();
	//run_button->set_image(run_image);
	//run_button->set_parent(*mainbar);
	
	//build_button = new Widget();
	//build_button->set_as_icon(true);
	//Image  build_image();
	//build_button->set_image(build_image);
	//build_button->set_parent(*mainbar);
	/*

	// selection
	property_box = new Widget();
	property_box->set_size(300, 500);
	property_box->set_position(10, window->get_height() - (property_box->get_height() + 30));
	property_box->set_outline(true);
	property_box->set_outline_width(2.0);
	property_box->set_outline_color(255, 255, 255);
	// dialogue
    dialog = new Widget();
	dialog->set_size(300, 50);
	dialog->set_position(500, 500);
	dialog->set_title_bar(true);
	dialog->set_title_bar_size(20);
    dialog->set_title_bar_button_close(true);
	dialog->set_outline(true);
	dialog->set_draggable(true);

	project_edit = new Edit();
	project_edit->set_size(project_edit->get_width(), dialog->get_height()/2);
	project_edit->set_parent(*dialog);
	
	ok_button = new Button("OK");
	ok_button->set_size(20, 20);
	ok_button->set_parent(*dialog);
	ok_button->set_position(dialog->get_width() - ok_button->get_width(), 
	    dialog->get_height());
	cancel_button = new Button("Cancel");
	cancel_button->set_size(50, 20);
	cancel_button->set_parent(*dialog);	
	cancel_button->set_position(ok_button->get_x() - ok_button->get_width(), 
	    dialog->get_height());	
	
    dialog->hide();	
		
	selection_box = new Widget();
    selection_box->set_size(50, 50);	
	selection_box->set_color(128, 128, 128);
	selection_box->set_parent(*property_box);
	selection_box->set_relative_position(10, 10);
	selection_box->set_outline(true);
	selection_box->set_outline_width(2.0);
	*/
	// view
	/*
	Viewer.window = new Window();
	Viewer.window->set_style (DOKUN_POPUP | DOKUN_BORDER | DOKUN_SYSMENU);
	int width  = window->get_width()  / 2;
	int height = window->get_height() / 2;
	// x and y determine center
	int x = (window->get_width() - width) / 2;
	int y = (window->get_height() - height) / 2;
	Viewer.get_window()->create("View", x, y, width, height);
    SetParent(Viewer.get_window()->get_handle(), window->get_handle());
	Viewer.get_window()->show();
	*/	
/*
dialog = new Widget();
	dialog->set_size(300, 50);
	dialog->set_position(500, 500);
	dialog->set_title_bar(true);
	dialog->set_title_bar_size(20);
    dialog->set_title_bar_button_close(true);
	dialog->set_outline(true);
	dialog->set_text("This is a message");
	dialog->set_draggable(true);
	// toolbar
	toolbar = new Widget();
	toolbar->set_position(0, 0);
	toolbar->set_size(window->get_width(), 32);
	//toolbar->set_draggable(true);
    // tools	
	play_button  = new Image("res/Play.png");
	play_button->scale_to_fit(toolbar->get_size());//resize(play->get_width(), toolbar->get_height());
	pause_button = new Image("res/Pause.png");
	pause_button->scale_to_fit(toolbar->get_size());//resize(pause->get_width(), toolbar->get_height());
	stop_button  = new Image("res/Stop.png");
	stop_button->scale_to_fit(toolbar->get_size());//resize(stop->get_width(), toolbar->get_height());
	build_button = new Image("res/Lego-Designer-icon.png");//("res/brick-icon.png");
	build_button->scale_to_fit(toolbar->get_size());//resize(build->get_width(), toolbar->get_height());
	run_button = new Image("res/Document.png");
	run_button->scale_to_fit(toolbar->get_size());//
	cross = new Image("res/cross.png");
	cross->scale_to_fit(toolbar->get_size());
#ifdef DOKUN_OPENGL	
	cross->set_filter(GL_LINEAR, GL_LINEAR);
#endif	
	//Image * = new Image("res/.png");
	//Image * = new Image("res/.png");
	toolbar->set_forbidden_area(
	toolbar->get_x() + (toolbar->get_width()/2 - play_button->get_width() * 3) - 5,
	toolbar->get_y(),	
	(play_button->get_width() * 4),// width
	toolbar->get_height()); // height
	//------------------------------------	
	project_edit = new Edit();
	project_edit->set_position(500, 250);
	//project_edit->set_();
	//------------------------------------
		// collection
	collection_box = new Grid();
	collection_box->set_row(1);
	collection_box->set_column(10);
	collection_box->set_position((window->get_width()+collection_box->get_width())/2, window->get_height() - (collection_box->get_height() + 10)); // 10 is bottom padding
	// create tiles
	for(int i = 0; i < 10; i++)
	{
		Image * image = new Image("res/burger.png");
		collection_box->get_block(0, i)->set_image(*image);
	}
	//-------------------------------------
	// mouse box
	mouse_box = new Widget();
	mouse_box->set_size(32, 32);
	mouse_box->set_position(Mouse::get_position(*WINDOW::get_active()));
	mouse_box->set_outline(true);
	mouse_box->set_outline_color(255, 255, 255);
	//-------------------------------------
	tile_sheet = new Texture("res/path0.png", 0, 0, 128, 64);
	map = new Sprite(*tile_sheet);
	frame = 0;
	//-------------------------------------
	combo = new Combobox();
	List * list = new List();
	for(int i = 0; i < 10; i++)
		list->add(*new Widget());
	combo->set_list(*list);
	combo->set_position(520, 250);*/
	//-------------------------------------
}
////////////
void Editor::on_interact(void)
{
// input
		/*if(Mouse::is_over(play_button->get_rect()))
		{
			if(Mouse::is_pressed(1))
			play();
		}
		
		if(Mouse::is_over(run_button->get_rect()))
		{
			if(Mouse::is_pressed(1))
			run();
		}
	    if(Mouse::is_over(pause_button->get_rect()))
		{
			std::cout << "Pause button pressed\n";
		}
		if(Mouse::is_over(stop_button->get_rect()))
		{
			if(Mouse::is_pressed(1))
			stop();
		}			
	    if(Mouse::is_over(build_button->get_rect()))
		{
			if(Mouse::is_pressed(1))
            build();
		}	
		if(Mouse::is_over(cross->get_rect()))
		{
			if(Mouse::is_pressed(1))
			close();
		}*/
		/*
		for(int i = 0; i < 10; i++)
		{
			if(Mouse::is_over(collection_box->get_block(0, i)->get_rect()))
			{
				if(Mouse::is_pressed(1))
				{
				//Widget * block = collection_box->get_block(0, i);
				   Image * image = collection_box->get_block(0, i)->get_image();
				   if(image != nullptr)
				   {
					image->set_position(Mouse::get_position(*WINDOW::get_active()));
				   }
				}
			}
		}*/
			/*
	// draw ui elements 
	titlebar->draw();
	titlebar_close_button->draw();
	
	menubar->draw();

	collection_box->draw();
	mainbar->draw();
	property_box->draw();
	dialog->draw();
	// input
    if(Mouse::is_over(mainbar->get_rect())) // the mainbar and slidebar make up the mainbar
	{
		sidebar->set_relative_position(0, 0);
		mainbar->set_position(window->get_width() - mainbar->get_width(), menubar->get_height()); // slidebar is part of mainbar
	} else {
		sidebar->set_relative_position(0, 0);
	    mainbar->set_position(window->get_width() - sidebar->get_width(), menubar->get_height()); // show the sidebar part of the mainbar	
	}	
	
	if(menubar->get_submenu(0, 0)->is_pressed())
	{
		dialog->show();
	}
	if(dialog->is_visible())
	{
		if(Keyboard::is_pressed(0x0D))
		{
			create(project_edit->get_text());
		}
	}*/
	//////////////////////////
	/*
	if(Mouse::is_over(titlebar->get_position(), titlebar->get_size()))
	{
			dragged = true; // can drag
		if(Mouse::is_over(titlebar_close_button->get_position(), titlebar_close_button->get_size()))
            dragged = false;			
    }
	if(Mouse::is_pressed(1) && dragged)
	{
		window->set_position(
		    Mouse::get_position().x - titlebar->get_width () / 2,
			Mouse::get_position().y - titlebar->get_height() / 2);
	}
	if(Mouse::is_released(1)) // on release
	{
		dragged = false;
	}
	else window->set_position(window->get_position().x, window->get_position().y);
	*/
	//////////////////////////	
	/*if(titlebar_close_button->is_pressed())
	{
		close(); // close the editor
	}*/		
	// if(Mouse::is_over(->get_rect()) && Mouse::is_pressed(1))	
	/*if(Keyboard::is_pressed(DOKUN_KEY_ENTER))
	{
		//if (!project_edit->get_text().empty())
		{
			create(project_edit->get_text());
		}
	}	
	if(Keyboard::is_pressed(DOKUN_KEY_DOWN))
	{
		map->translate(0, 1);
	}
	if(Keyboard::is_pressed(DOKUN_KEY_RIGHT))
	{
		map->translate(1, 0);
	}	*/
}
////////////
void Editor::on_update(void)
{
	// Menubar
	menubar->set_size(window->get_client_width(), 20); // make sure menubar is same width as window client
	//toolbar->set_size(toolbar->get_size()); // update on window resize
	//play_button->set_position (toolbar->get_x(), toolbar->get_y());//(toolbar->get_x() + (toolbar->get_width()/2 - play_button->get_width() * 3) - 5, toolbar->get_y());
	//pause_button->set_position(play_button->get_x () + play_button->get_width (), toolbar->get_y());//(toolbar->get_x() + (toolbar->get_width()/2 - play_button->get_width() * 2) - 5, toolbar->get_y());
	//stop_button->set_position (pause_button->get_x() + pause_button->get_width(), toolbar->get_y());//(toolbar->get_x() + (toolbar->get_width()/2 - play_button->get_width()) - 5, toolbar->get_y());
	//build_button->set_position(stop_button->get_x () + stop_button->get_width (), toolbar->get_y());//(toolbar->get_x() + (toolbar->get_width()/2) - play_button->get_width() * 5 - 5, toolbar->get_y());
	//run->set_position(toolbar->get_x() + (toolbar->get_width()/2) - play->get_width() * 4 - 10, toolbar->get_y());
	//cross->set_position(toolbar->get_width() - cross->get_new_width(),
	    //toolbar->get_y());
	/*
    mouse_box->set_position(Mouse::get_position(*WINDOW::get_active()));	
	for(int i = 0; i < Factory::get_gui_factory()->get_size(); i++)
	{
		GUI * gui = static_cast<GUI *>(Factory::get_gui_factory()->get_object(i));
		if(gui != nullptr)
		{
			if(Mouse::is_over(gui->get_rect()))
			{
				//mouse_box->set_size(gui->get_size());
			}
		}
	}*/
}
////////////
void Editor::on_draw(void)
{
	on_update(); // ALWAYS update before drawing
	//------------------------------------------
	// Titlebar
	titlebar->draw();
	// Menubar
	menubar->draw ();
	// TOOLBAR --------------------------------
	/*
	toolbar->draw();
	play_button->draw ();
	pause_button->draw();
	stop_button->draw ();
	build_button->draw();
	//run_button->draw();
*/	
	//cross->draw();
	// PROJECT EDIT ------------------------------------------
    //project_edit->draw();
	//------------------------------------------
	//dialog->draw();
	//------------------------------------------
	//collection_box->draw();
	//------------------------------------------
	//mouse_box->draw();
	//frame++;
	//if(frame >= 5)
		//frame = 0;
	//map->draw(13);
	//std::cout << map->get_position() << std::endl;
	//------------------------------------------
	//combo->draw();
	//------------------------------------------
	//level->draw(); // draw anything added to the level
	//------------------------------------------
}
////////////
void Editor::draw(void)
{
    on_interact();
    on_draw();
}
////////////
////////////
  // create a c++ executable that runs a Lua script
int main(int argc, char * argv[])
{	
	Editor * editor = new Editor();editor->execute();
	return 0;
}

/*
auto.lua is the script generated from using the Drag-and-Drop editor
main.lua is the script for running the game

REQUIREMENTS:
 must have both srlua and glue.
 
To create a project
 editor->create("Sid's Game");
 
To build an executable (from code or editor)
  editor->build();

To run the executable 
  editor->play();
  
To run the script (main.lua)
  editor->run(); 

To stop the running process
  editor->stop(); 
*/
