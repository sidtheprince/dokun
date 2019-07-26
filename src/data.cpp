#include "../include/data.h"

Data::Data()
{}
////////////
Data::Data(const std::string& file_name)
{
	if(!open(file_name))
		Logger("Could not open database: " + String::to_string(sqlite3_errmsg(database)) + file_name);
}
////////////
Data::~Data()
{}
////////////
bool Data::open(const std::string& file_name) // create a data base; works!
{
	if(sqlite3_open(file_name.c_str(), &database))
    {
		close();
		return false;
	}
	return true;
}
////////////
int Data::open(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_optstring(L, 2, "");
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Data * data = *static_cast<Data **>(lua_touserdata(L, -1));
		data->open(lua_tostring(L, 2));
	}
	lua_pushboolean(L, false);
	return 1;
}
////////////
void Data::execute(const std::string& sql) // execute an sql statement; works !
{
	char * errmsg = 0;
	int error = sqlite3_exec(database, sql.c_str(), Data::callback, 0, &errmsg);
	if (error != SQLITE_OK)
	{
		Logger("SQL error: " + String::to_string(errmsg));
		sqlite3_free(errmsg);
	}
}
////////////
int Data::execute(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Data * data = *static_cast<Data **>(lua_touserdata(L, -1));
		data->execute(lua_tostring(L, 2));
	}
	return 0;
}
////////////
void Data::table(const std::string& table, bool auto_inc) // creates a new table; works !
{
	std::string sql = "CREATE TABLE table_name(id  INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT);";
	// set table name
    sql = String::swap_first_of(sql, "table_name", table);
	if(auto_inc == false)
		sql = String::remove_first_of(sql, "AUTOINCREMENT");
	// execute sql
	execute(sql.c_str());
}
////////////
int Data::table(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	luaL_optnumber(L, 3, 0);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Data * data = *static_cast<Data **>(lua_touserdata(L, -1));
		data->table(lua_tostring(L, 2)/*, (unsigned int)lua_tonumber(L, 3)*/);
	}
	return 0;
} // data->table("Player");
////////////
void Data::column(const std::string& table, const std::string& column_name, const std::string& column_def, std::string method) // creates a column; works!
{
	std::string sql = "ALTER TABLE table_name method column_name column_def;";
	// add a column to table 
	sql = String::swap_first_of(sql, "table_name", table);
	sql = String::swap_first_of(sql, "method", method); // ADD, DROP, MODIFY
	sql = String::swap_first_of(sql, "column_name",   column_name );
	sql = String::swap_first_of(sql, "column_def",   column_def );
	std::cout << sql << "\n";
	// execute 
	execute(sql.c_str());	
} 
/* 
data->column("Player", "name", "TEXT", "ADD"); 
data->column("Player", "level", "INTEGER(3)", "ADD");
*/
/////////////
int Data::column(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING); // table
	luaL_checktype(L, 3, LUA_TSTRING); // column
	luaL_checktype(L, 4, LUA_TSTRING); // type
	luaL_checktype(L, 5, LUA_TSTRING); // method
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Data * data = *static_cast<Data **>(lua_touserdata(L, -1));
		data->column(lua_tostring(L, 2), lua_tostring(L, 3), lua_tostring(L, 4), lua_tostring(L, 5));
	}
	return 0;
}
////////////
void Data::insert(const std::string& table, const std::string& variable, const std::string& value) // sets the value of a column (column must exist first); works!
{  
    std::string sql = "INSERT INTO table_name (variable)" 
	" VALUES (value);";
    // set table variable
  	sql = String::swap_first_of(sql, "table_name", table   );
	sql = String::swap_first_of(sql, "variable"  , variable);
	sql = String::swap_first_of(sql, "value"     , /* "'" + */ value /* + "'" */);
	// execute
	execute(sql.c_str());
} // data->insert("Player", "name, level", "'Sid', 1"); -> sets the column 'name' to 'Sid'
////////////
int Data::insert(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	luaL_checktype(L, 3, LUA_TSTRING);
	luaL_checkany(L, 4);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Data * data = *static_cast<Data **>(lua_touserdata(L, -1));
		data->insert(lua_tostring(L, 2), lua_tostring(L, 3), lua_tostring(L, 4));
	}
	return 0;
}
////////////
void Data::select(const std::string& table, std::string variable, std::string condition) // selects variable from a table
{
	std::string sql = "SELECT * "
	"FROM table_name "
	"WHERE condition;";
	// select from table
	sql = String::swap_first_of(sql, "table_name", table);
	if (!variable.empty()) 
	    String::swap_first_of(sql, "*",  variable);
	if(condition.empty())
		sql = String::remove_first_of(sql, "WHERE condition");
	if(!condition.empty())
	    sql = String::swap_first_of(sql, "condition",  condition);
	// execute
	execute(sql.c_str());
}
int Data::select(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	luaL_checktype(L, 3, LUA_TSTRING);
	luaL_optstring(L, 4, "");	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Data * data = *static_cast<Data **>(lua_touserdata(L, -1));
		data->select(lua_tostring(L, 2), lua_tostring(L, 3), lua_tostring(L, 4));
	}
	return 0;
}
////////////
void Data::update(const std::string& table, const std::string& variable, const std::string& value, std::string condition) // updates a variable; works!
{
	std::string sql = "UPDATE table_name "
	"SET variable = value " 
	"WHERE condition;     ";
	// update variable 
	sql = String::swap_first_of(sql, "table_name", table   );
	sql = String::swap_first_of(sql, "variable"  , variable);
	sql = String::swap_first_of(sql, "value"     , value   );
	if(condition.empty())
		sql = String::remove_first_of(sql, "WHERE condition");
	if(!condition.empty())
	    sql = String::swap_first_of(sql, "condition",  condition);
	// execute
	execute( sql.c_str() );
}
int Data::update(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	luaL_checktype(L, 3, LUA_TSTRING);
	luaL_checktype(L, 4, LUA_TSTRING);
    luaL_optstring (L, 5, "");	
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Data * data = *static_cast<Data **>(lua_touserdata(L, -1));
		data->update(lua_tostring(L, 2), lua_tostring(L, 3), lua_tostring(L, 4), lua_tostring(L, 5));
	}
	return 0;
}
////////////
void Data::drop(const std::string& table, std::string condition) // works!
{
	std::string sql;
	if(condition.empty())
	{
		// delete table
	    sql = "DROP TABLE table_name;";
	    sql = String::swap_first_of(sql, "table_name", table);
	}
	if(!condition.empty())
	{
		// delete column
		sql = "DELETE FROM table_name "
		"WHERE condition";
		sql = String::swap_first_of(sql, "table_name", table);
		sql = String::swap_first_of(sql, "condition", condition);
	}
	// execute
	execute(sql.c_str());	
}
////////////
int Data::drop(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	luaL_optstring(L, 3, "");
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Data * data = *static_cast<Data **>(lua_touserdata(L, -1));
		data->drop(lua_tostring(L, 2), lua_tostring(L, 3));
	}	
	return 0;
}
////////////
////////////
////////////
////////////
////////////
void Data::rename_table(const std::string& table, const std::string& new_name)
{
	std::string sql = "ALTER TABLE table_name " 
	"RENAME TO new_name;";
    // rename table
	sql = String::swap_first_of(sql, "table_name", table   );
	sql = String::swap_first_of(sql, "new_name"  , new_name);
	std::cout << sql << "\n";
	// execute
	execute(sql.c_str());
}
int Data::rename_table(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TSTRING);
	luaL_checktype(L, 3, LUA_TSTRING);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Data * data = *(Data **)lua_touserdata(L, -1);
		data->rename_table(lua_tostring(L, 2), lua_tostring(L, 3));
	}	
    return 0;
}
////////////
////////////

double Data::get_column_double()
{
    return 0;
}
int Data::get_column_integer()
{
    return 0;
}
std::string Data::get_column_text()
{
    return "";
}
////////////
////////////
void Data::close()
{
	sqlite3_close(database);
}
////////////
int Data::close(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Data * data = *static_cast<Data **>(lua_touserdata(L, -1));
		data->close();
	}	
	return 0;
}
////////////
////////////
////////////
////////////
////////////
////////////
////////////
/*
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Data * data = *(Data **)lua_touserdata(L, -1);
		data->();
	}
*/	
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////

////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
////////////
int Data::callback(void *un_used, int argc, char **argv, char **azcolname)
{
    int i;
    for(i = 0; i < argc; i++) {
	    std::cout << azcolname[i] << " = " << (argv[i] ? argv[i] : "nullptr") << std::endl;  // printf("%s = %s\n", azcolname[i], argv[i] ? argv[i] : "nullptr");
    }
    std::cout << std::endl;
    return 0;	
}
////////////
/*
int Data::(lua_State *L)
{}
*/
////////////
int Data::new_(lua_State *L)
{
	std::string file_name("");
	if(lua_type(L, -1) == LUA_TSTRING)
	{
		file_name = lua_tostring(L, -1);
	}
	lua_settop(L, 0);
	// table
	lua_createtable(L, 0, 0);
	// mt
	lua_getglobal(L, "Data_mt");
	lua_setmetatable(L, 1);
	// udata
	Data ** data = static_cast<Data **>(lua_newuserdata(L, sizeof(Data *)));
	if( !file_name.empty())
	{
		*data = new Data(file_name);
	}
	else
	{
	    *data = new Data();
	}
	lua_setfield(L, 1, "udata");
	// return table
	if(lua_istable(L, -1))
		return 1;
	lua_pushnil(L);
	return 1 ;
}
////////////
////////////
////////////
////////////
////////////
bool Data::text_to_binary(const std::string& input, const std::string& output) // (input)file_to_convert_to_binary, (output)binary_file
{
	std::ifstream rfile(input.c_str());
	if(!rfile.is_open())
	{
	    return false;
	}	
	std::stringstream stream;
	stream << rfile.rdbuf(); // dump file contents	
	
	std::string content = stream.str();	// store file content in string
    rfile.close();
	// ----
	    std::ofstream file(output.c_str()); // open file for writing
        if(!file.is_open())
        {
	        return false;
        }	
        for(int i = 0; i < content.size(); i++)
		{
			file << dokun::char_to_binary(content.at(i)) << "\n";
		}
	file.close();	
	return true;
}
////////////
bool Data::binary_to_text(const std::string& input, const std::string& output)
{
	std::ifstream rfile(input.c_str());
	if(!rfile.is_open())
	{
	    return false;
	}	
	std::string line;
	std::vector<std::string> binary;
    while(std::getline(rfile, line))
	{
		binary.push_back(line);
	}
    rfile.close();
	// ----
	    std::ofstream file(output.c_str()); // open file for writing
        if(!file.is_open())
        {
	        return false;
        }	
        for(int i = 0; i < binary.size(); i++)
		{
			file << dokun::binary_to_char(binary[i]);
			//std::cout << dokun::binary_to_char(binary[i]);
		}
	file.close();	
	return true;
}
////////////
////////////
////////////
////////////
