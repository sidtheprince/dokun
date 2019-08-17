#include "../include/loader.h"

Loader::Loader()
{}
//////////
Loader::Loader(const std::string& file_name)
{
	load(file_name);
}
//////////
Loader::~Loader()
{
	clear();
}
//////////
Loader * Loader::dokun_loader (nullptr);
//////////		
std::vector<std::string> Loader::keyword    {}; // registered keys (when a key is registered, you can access its value)
//////////	
void Loader::load(const std::string& file_name)
{
	if(File::extension(file_name) == "dokun")
	{
		load_dokun(file_name);
	}
}
//////////
void Loader::clear()
{
	userdata.clear();
	string.clear();
	number.clear();
	boolean_.clear();
	vector.clear();
}
//////////
void Loader::remove(std::string key)
{
	if(is_pointer(key))
	{
	    userdata.erase(key);
	}
	if(is_string(key))
	{
	    string.erase(key);
	}
	if(is_number(key))
	{
	    number.erase(key);
	}
	if(is_boolean_(key))
	{
	    boolean_.erase(key);
	}
	if(is_vector(key))
	{
	    vector.erase(key);
	}
	if(is_function(key))
	{
	    function.erase(key);
	}
}
//////////
//////////
void Loader::set_key(std::string keyword)
{
	(this)->keyword.push_back(keyword);
	// re-load if loaded before setting
	if(!file.empty())
	{
	    load(file);
	}
}
//////////
//////////
// GETTERS
//////////
void Loader::get_data(std::string key, void * value)
{
	value = userdata[key];
}
//////////
void Loader::get_data(std::string key, std::string& value)
{
	value = string[key];
}
//////////
void Loader::get_data(std::string key, String * value)
{
	*value = string[key];
}
//////////
void Loader::get_data(std::string key, int *  value)
{
	*value = boolean_[key]; // can also be integer
}
//////////
void Loader::get_data(std::string key, double *  value)
{
	*value = number[key];
}
//////////
void Loader::get_data(std::string key, Vector2 *  value)
{
	*value = Vector2(vector[key].x, vector[key].y );
}
//////////
void Loader::get_data(std::string key, Vector3 *  value)
{
	*value = Vector3(vector[key].x, vector[key].y, vector[key].z );
}
//////////
void Loader::get_data(std::string key, Vector4 *  value)
{
	*value = vector[key];
}
//////////
void Loader::get_data(std::string key, std::function<void (void)> value)
{
	value = function[key];
}
//////////
int Loader::get_size(std::string type)
{
	if(String::match((type), "pointer"))
	{
		return userdata.size();
	}
	if(String::match((type), "string"))
	{
		return string.size();
	}
	if(String::match((type), "number"))
	{
		return number.size();
	}
	if(String::match((type), "boolean_"))
	{
		return boolean_.size();
	}
	if(String::match((type), "vector"))
	{
		return vector.size();
	}
	if(String::match((type), "function"))
	{
		return function.size();
	}
    return (-1);
}
//////////
int Loader::get_size()
{
	return content.size();
}
//////////
std::string Loader::get_line(int index)
{
	return content[index];
}
//////////
std::string Loader::get_content()
{
	//return content
	return ("");
}
//////////
//////////	
Loader * Loader::get_dokun_loader()
{
	if(!dokun_loader)
	{
		dokun_loader = new Loader();
	}
	return (dokun_loader);
}
//////////
//////////
// boolean_
//////////
bool Loader::is_pointer(std::string key)
{
	if(!is_key(key)) // not a valid key
	{
		return false;
	}
    if(userdata.count(key) == 0) // not a valid function
    {
        return false;
    }			
	return true;	
}
//////////
bool Loader::is_string(std::string key)
{
	if(!is_key(key)) // not a valid key
	{
		return false;
	}
    if(string.count(key) == 0) // not a valid function
    {
        return false;
    }			
	return true;	
}
//////////
bool Loader::is_number(std::string key)
{
	if(!is_key(key)) // not a valid key
	{
		return false;
	}
    if(number.count(key) == 0) // not a valid function
    {
        return false;
    }			
	return true;	
}
//////////
bool Loader::is_boolean_(std::string key)
{
	if(!is_key(key)) // not a valid key
	{
		return false;
	}
    if(boolean_.count(key) == 0) // not a valid function
    {
        return false;
    }			
	return true;	
}
//////////
bool Loader::is_vector(std::string key)
{
	if(!is_key(key)) // not a valid key
	{
		return false;
	}
    if(vector.count(key) == 0) // not a valid function
    {
        return false;
    }			
	return true;	
}
//////////
bool Loader::is_function(std::string key)
{
	if(!is_key(key)) // not a valid key
	{
		return false;
	}
    if(function.find(key) == function.end()) // not a valid function
    {
        return false;
    }			
	return true;
}
//////////
bool Loader::is_key(std::string key)
{
    if(userdata.count(key) > 0)
    {
        return true;
    }	
    if(string.count(key) > 0)
    {
        return true;
    }	
    if(number.count(key) > 0)
    {
        return true;
    }	
    if(boolean_.count(key) > 0)
    {
        return true;
    }	
    if(vector.count(key) > 0)
    {
        return true;
    }	
    if(function.count(key) > 0)
    {
        return true;
    }	
	return false;
}
//////////
bool Loader::is_empty()
{
	if(!userdata.empty())
	{
		return false;
	}
	if(!string.empty())
	{
		return false;
	}	
	if(!number.empty())
	{
		return false;
	}	
	if(!boolean_.empty())
	{
		return false;
	}	
	if(!vector.empty())
	{
		return false;
	}	
	if(!function.empty())
	{
		return false;
	}	
	return(true);
}
//////////
//////////
// SPECIFIC FORMAT LOADERS
//////////
bool Loader::load_dokun(const std::string& file_name)
{
	(this)->getline(file_name); // get file contents, line by line

	for(int i = 0; i < get_size(); i++)
	{
		if(String::contains(get_line(i), "project_name") ) // line contains 'project_name'
		{
			std::string proj = get_line(i); // store line
			size_t find_first_quote = proj.find_first_of("\""); // find first quote
			proj = proj.substr(find_first_quote, proj.length()); // get quoted string
            proj = String::remove_all(proj, "\""); // remove quotes
			
			
			(this)->string["project_name"] = proj; // store value in key 'project_name'
			if(!is_key("project_name"))
			{
				std::cout << "key 'project_name' not found" << std::endl;
			    return false;
			}
			//std::cout << proj << "\n";
		}
		if(String::contains(get_line(i), "path") ) // line contains 'path'
		{
			std::string path = get_line(i); // store line
			size_t find_first_quote = path.find_first_of("\""); // find first quote
			path = path.substr(find_first_quote, path.length()); // get quoted string
            path = String::remove_all(path, "\""); // remove quotes
			
			(this)->string["path"] = path; // store value in key 'path'
			if(!is_key("path"))
			{
				std::cout << "key 'path' not found" << std::endl;
				return false;
			}
			//std::cout << path << "\n";
		}
        // look for others keys
        for(int j = 0; j < keyword.size(); j++)
		{
			if(String::contains(get_line(i), keyword[j]) )
			{
				std::string value = get_line(i); // store line
				if(String::contains(value, "\"")) // check where there are quotes
				{
				    size_t find_first_quote = value.find_first_of("\""); // find first quote
				    value = value.substr(find_first_quote, value.length()); // get quoted string

				    String::remove_all(value, "\""); // remove quotes
					(this)->string[ keyword[j] ] = value; // store value in key 'keyword[j]'
				}
				if(!is_key(keyword[j]))
			    {
				    std::cout << "key " << keyword[j] << " is not valid" << std::endl;
			        return false;
			    }
				//std::cout << value << "\n";
			}
		}			
	}
	return true;	
}
//////////
/*
bool Loader::load_dokun(lua_State *L, std::string file_name)
{
	if(!dokun::exists(file_name)) // file not in path?
	{
		Debug::print(file_name + " not found");
		return false;
	}
	if(luaL_dofile(L, file_name.c_str()) != 0) // error in script?
    {
		Logger("dokun: Error in " + file_name);
        return false;
    }	
	// get path and project
	lua_getglobal(L, "project_name");
	std::cout << lua_tostring(L, -1) << std::endl;
	lua_getglobal(L, "path");
	std::cout << lua_tostring(L, -1) << std::endl;
	return true;
}
//////////
bool Loader::load_png(const std::string& file_name, void * data, int * width, int * height)
{
	*width  = 0;
	*height = 0;
 // header for testing if it is a png
	unsigned int number = 8;
    png_byte header[8];
    // open the file 
    std::FILE *fp ;
	#if defined(_MSC_VER) && (_MSC_VER >= 1020)
	    fopen_s(&fp, file_name.c_str(), "rb");
	#else
	    fp = fopen(file_name.c_str(), "rb");
	#endif
	if (!fp) 
    {
		return false; // exit after retrieving error code
    }
	else 
	{
		//file =(file_name);
	}
    // read the header
    fread(header, 1, number, fp);
    int is_png = !png_sig_cmp(header, 0, number);
    if (!is_png) 
	{
        fclose(fp);
		return false; 
    }
   // create png struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr,
        nullptr, nullptr);
    if (!png_ptr) 
	{
        fclose(fp);
		return false;
    }
   //create png info struct
   png_infop info_ptr = png_create_info_struct(png_ptr);
   if (!info_ptr) {
     png_destroy_read_struct(&png_ptr, (png_infopp) nullptr, (png_infopp) nullptr);
     fclose(fp);
	 return false;
   }
   //create png end info struct
   png_infop end_info = png_create_info_struct(png_ptr);
   if (!end_info) {
     png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) nullptr);
     fclose(fp);
	 return false;
   }   
   //png error stuff, not sure libpng man suggests this.
   if (setjmp(png_jmpbuf(png_ptr))) {
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
	 return false;
   }
   //init png reading
   png_init_io(png_ptr, fp);
   //let libpng know you already read the first 8 bytes
   png_set_sig_bytes(png_ptr, number);
   // change zlib compression buffer size when reading compressed data
   int buffer_size = 8192;
   png_set_compression_buffer_size(png_ptr, buffer_size);
   // read all the info up to the image data
   png_read_info(png_ptr, info_ptr);
   //variables to pass to get info
   int bit_depth, color_type, interlace_type, compression_type, filter_method;
   int channels;
   
   //update width and height based on png info
   *width =  png_get_image_width(png_ptr, info_ptr);
   *height = png_get_image_height(png_ptr, info_ptr);
   // get bit_depth and color_type based on png info
   bit_depth   = png_get_bit_depth(png_ptr, info_ptr);
   color_type = png_get_color_type(png_ptr, info_ptr);
   interlace_type=png_get_interlace_type(png_ptr,
                         info_ptr);
   compression_type=png_get_compression_type(png_ptr,
                         info_ptr);
   filter_method    = png_get_filter_type(png_ptr,
                         info_ptr);

   channels = png_get_channels(png_ptr, info_ptr);
    
// .. must be called before using png_read_update_info, as well as other setters
   int number_of_passes;
   number_of_passes = png_set_interlace_handling(png_ptr);
   // Update the png info struct.
   png_read_update_info(png_ptr, info_ptr);
   // Row size in bytes.
   int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
   
   
   // Allocate the image_data as a big block, to be given to opengl
   data = new png_byte[rowbytes * (*height)];
   if (!data) 
   {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
	 return false;
   }
 
   //row_pointers is for pointing to data for reading the png with libpng
   png_bytep *row_pointers = new png_bytep[*height];
   if (!row_pointers) 
   {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     data = nullptr;
     fclose(fp);
	 return false;
   }
   // set the individual row_pointers to point at the correct offsets of data
   for (unsigned int i = 0; i < *height; ++i) 
   {
     row_pointers[*height - 1 - i] = (png_byte*)data + i * rowbytes;
   }
  
   // the actual reading of the png file
   //read the png into data through row_pointers
   png_read_image(png_ptr, row_pointers);
 
   //clean up memory and close stuff
   png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
   fclose(fp);
   return true; // return false;   
}
//////////
void Loader::load_png(const std::string& file_name, void * data, int * width, int * height, dokun_Error * error)
{
	*width  = 0;
	*height = 0;	
  // header for testing if it is a png
	unsigned int number = 8;
    png_byte header[8];
    // open the file 
    std::FILE *fp ;
	#if defined(_MSC_VER) && (_MSC_VER >= 1020)
	    fopen_s(&fp, file_name.c_str(), "rb");
	#else
	    fp = fopen(file_name.c_str(), "rb");
	#endif
	if (!fp) 
    {
		*error = DOKUN_ERROR_FILE_NOT_FOUND;
		return; // exit after retrieving error code
    }
	else 
	{
		//file =(file_name);
	}
    // read the header
    fread(header, 1, number, fp);
    int is_png = !png_sig_cmp(header, 0, number);
    if (!is_png) 
	{
		*error = DOKUN_ERROR_FILE_INVALID;
        fclose(fp);
		return; 
    }
   // create png struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr,
        nullptr, nullptr);
    if (!png_ptr) 
	{
		*error = DOKUN_ERROR_FILE_BROKEN;
        fclose(fp);
		return;
    }
   //create png info struct
   png_infop info_ptr = png_create_info_struct(png_ptr);
   if (!info_ptr) {
	 *error = DOKUN_ERROR_FILE_BROKEN;
     png_destroy_read_struct(&png_ptr, (png_infopp) nullptr, (png_infopp) nullptr);
     fclose(fp);
	 return;
   }
   //create png end info struct
   png_infop end_info = png_create_info_struct(png_ptr);
   if (!end_info) {
	 *error = DOKUN_ERROR_FILE_BROKEN;
     png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) nullptr);
     fclose(fp);
	 return;
   }   
   //png error stuff, not sure libpng man suggests this.
   if (setjmp(png_jmpbuf(png_ptr))) {
	 *error = DOKUN_ERROR_FILE_BROKEN;
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
	 return;
   }
   //init png reading
   png_init_io(png_ptr, fp);
   //let libpng know you already read the first 8 bytes
   png_set_sig_bytes(png_ptr, number);
   // change zlib compression buffer size when reading compressed data
   int buffer_size = 8192;
   png_set_compression_buffer_size(png_ptr, buffer_size);
   // read all the info up to the image data
   png_read_info(png_ptr, info_ptr);
   //variables to pass to get info
   int bit_depth, color_type, interlace_type, compression_type, filter_method;
   int channels;
   
   //update width and height based on png info
   *width =  png_get_image_width(png_ptr, info_ptr);
   *height = png_get_image_height(png_ptr, info_ptr);
   // get bit_depth and color_type based on png info
   bit_depth   = png_get_bit_depth(png_ptr, info_ptr);
   color_type = png_get_color_type(png_ptr, info_ptr);
   interlace_type=png_get_interlace_type(png_ptr,
                         info_ptr);
   compression_type=png_get_compression_type(png_ptr,
                         info_ptr);
   filter_method    = png_get_filter_type(png_ptr,
                         info_ptr);

   channels = png_get_channels(png_ptr, info_ptr);
    
// .. must be called before using png_read_update_info, as well as other setters
   int number_of_passes;
   number_of_passes = png_set_interlace_handling(png_ptr);
   // Update the png info struct.
   png_read_update_info(png_ptr, info_ptr);
   // Row size in bytes.
   int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
   
   
   // Allocate the image_data as a big block, to be given to opengl
   data = new png_byte[rowbytes * (*height)];
   if (!data) 
   {
	 *error = DOKUN_ERROR_FILE_BROKEN;
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
	 return;
   }
 
   //row_pointers is for pointing to data for reading the png with libpng
   png_bytep *row_pointers = new png_bytep[*height];
   if (!row_pointers) 
   {
	 *error = DOKUN_ERROR_FILE_BROKEN;
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     data = nullptr;
     fclose(fp);
	 return;
   }
   // set the individual row_pointers to point at the correct offsets of data
   for (unsigned int i = 0; i < *height; ++i) 
   {
     row_pointers[*height - 1 - i] = (png_byte*)data + i * rowbytes;
   }
  
   // the actual reading of the png file
   //read the png into data through row_pointers
   png_read_image(png_ptr, row_pointers);
 
   //clean up memory and close stuff
   png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
   fclose(fp); 
   *error = DOKUN_ERROR_NO_ERROR;		
}
*/
//////////
bool Loader::load_obj(std::string file_name, std::vector<Vector3>& vertex/*, std::vector<int>& index*/)
{
	content.clear(); // clear previous cache
	(this)->getline(file_name); // store each line
	for(int i = 0; i < get_size(); i++) // scan through each line
	{
		if(String::contains(get_line(i), "#") )
		{
			continue;
		}
		if(String::contains(get_line(i), "v") ) // line contains 'v'
		{
			std::string v = get_line(i); // store line
			std::vector<std::string> vertex_split = String::split(v, " ");
			for(int i = 0; i < vertex_split.size(); i++)
			{
	            vertex_split[i] = String::remove_all(vertex_split[i], "v"); // remove v's
			}
			for( int i = 0; i < vertex_split.size(); i+=3 ) // for every 3 number
			{
				if (!vertex_split[i].empty()) // cannot be empty
				{
				    double x = String::extract(vertex_split[i]);
				    double y = String::extract(vertex_split[i + 1]);
				    double z = String::extract(vertex_split[i + 2]);
					
					//vertex.push_back(Vector3(x, y, z));
					//std::cout << Vector3(x, y, z) << std::endl;
				}
			}
		}		
	}
	return true;
}
//////////
//////////
//////////
//////////
void Loader::copy(const std::string& file_name)
{
	std::ifstream file(file_name.c_str());
    std::istream_iterator<std::string> start(file), end;
    std::vector<std::string> line(start, end);

	content = line;
	file.close();
}
//////////
void Loader::getline(const std::string& file_name) // takes longer than Loader::copy
{
	std::ifstream file(file_name.c_str());
	if(!file.is_open())
	{
		return;
	} std::string line;
	while(std::getline(file, line))
	{
		if(!line.empty())
		{
			content.push_back(line); // store each line
		}
	}
	file.close();
}
//////////
void Loader::stream(const std::string& file_name) // takes longer than Loader::getline
{
	std::ifstream file(file_name.c_str());
	if(!file.is_open())
	{
		return;
	}
	std::string line;
	std::stringstream stream;
	while(std::getline(file, line))
	{
		stream >> line;
		if(!line.empty())
		{
			content.push_back(line);
			//std::cout << line << "\n";
		}
	}
	file.close();
}
//////////
/*
    // best way to get key from file ..

	loader->getline("test.dokun"); // get file contents, line by line

	for(int i = 0; i < loader->get_size(); i++)
	{
		if(String::contains(loader->get_line(i), "project_name") ) // line contains 'project_name'
		{
			std::string proj = loader->get_line(i); // store line
			size_t find_first_quote = proj.find_first_of("\""); // find first quote
			proj = proj.substr(find_first_quote, proj.length()); // get quoted string
String::remove_all(proj, '\"'); // remove quotes
			
			std::cout << proj << "\n";
		}
		//std::cout << loader->get_line(i) << "\n";
	}








*/
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////
//////////