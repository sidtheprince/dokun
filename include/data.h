#ifndef _DATA
#define _DATA

#include "utility.h"
#include "string.h"
#include "logger.h"
#ifdef __cplusplus // if c++
extern "C" {      // run as c code
#endif
    #include <sqlite3.h>       // data (sqlite3)
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <iostream>
#include <cstdarg>
#include <fstream>
#include <sstream>
#include <utility> // std::pair
#include <lua.hpp>

#ifdef Data
#undef Data
#endif

class Data // for sql and xml data serialization
{
    public:
	    Data();     static int new_(lua_State *L);
		Data(const std::string& file_name);
		~Data();
		// SQL
		// opens and closes database
		bool open(const std::string& file_name);   static int open(lua_State *L); // open a data base
		void close();                              static int close(lua_State *L);// closes a database connection
		// executes sql commands
		void execute(const std::string& sql);      static int execute(lua_State *L);// execute an sql command
		// creates a table
		void table(const std::string& table_name, bool auto_inc = true); static int table(lua_State *L);// table players
		// creates a new column property
		void column(const std::string& table, const std::string& column_name, const std::string& column_def, std::string method = "ADD"); static int column(lua_State *L); // ADD, DROP, MODIFY
		// sets the column values
		void insert(const std::string& table, const std::string& variable, const std::string& value); static int insert(lua_State *L);
		// select tables to display
		void select(const std::string& table, std::string variable = "", std::string condition=""); static int select(lua_State *L);// selects from table
		// update
		void update(const std::string& table, const std::string& variable, const std::string& value, std::string cond=""); static int update(lua_State *L);
		// destroy column or table
        void drop(const std::string& table, std::string cond = ""); static int drop   (lua_State *L);
		// rename table
		void rename_table(const std::string& table, const std::string& new_name);
		static int rename_table(lua_State *L);

		// configure db int sqlite3_db_config(sqlite3*, int op, ...);
		// shut down sqlite    int sqlite3_shutdown(void);

		
		// free memory used by database
	    void free(); // int sqlite3_db_release_memory(sqlite3*);
        //sqlite3_free_table();		
		std::string get_database_name(); // const char *sqlite3_db_filename(sqlite3 *db, const char *zDbName);
	    // returns connection status of db
	    void get_status(); // int sqlite3_db_status(sqlite3*, int op, int *pCur, int *pHiwtr, int resetFlg);
	    // returns column name
		void get_column(); // const char *sqlite3_column_name(sqlite3_stmt*, int N);
		double get_column_double();
		int get_column_integer();
		std::string get_column_text();
		void get_row(); 
		// check if statement is complete
		// int sqlite3_complete(const char *sql);
		//////////////////////////////////
		// XML
		static void parse_xml(const std::string& file_name);
		
		bool xml_open(const std::string& file_name);
		void xml_close();
		bool xml_is_root();
		//void xml_();
		std::string xml_key(const std::string& node, const std::string& property); // returns the value of a node'a property
		
		// getters
		//////////////////////////////////
		// TEXT, BINARY
		bool text_to_binary(const std::string& input, const std::string& output);
		bool binary_to_text(const std::string& input, const std::string& output);
		//////////////////////////////////
		// JSON
		void json_init();
		void json_parse(const char * json);
		bool json_load(const std::string& file_name);
		bool json_write(const std::string& output_file);
		std::string json_stringify(); // prints out the json file content
		
		void json_start_object();
		void json_end_object();
		void json_array_start(); // can add a value in between attay_start and array_end
		void json_array_end(); // arr_start()   number(1) number(2) number(3) arr_end()  array with 3 numbers		
		void json_key(const std::string& key_name); // outputs a key
		void json_int(int value); // followed by a value
		void json_uint(unsigned int value);
		void json_double(double value);
		void json_bool(bool value);
		void json_string(const std::string& value);
		void json_string(const String& value);
		void json_null();

		void json_set(const std::string& key, const std::string& value);
		void json_set(const std::string& key, int value, bool is_bool = false); // or bool
		void json_set(const std::string& key, double value);
		void json_set(const std::string& key, const String& value);
		void json_set(const std::string& key, void * value);
		
		void json_get(const std::string& key);
		void json_get(const std::string& key, int * value);
		void json_get(const std::string& key, double * value);
		void json_get(const std::string& key, String& value);
		void json_get(const std::string& key, void * value);
		
		bool json_is_object(); // check if document is object
		bool json_has_member(const std::string& key);
		bool json_is_null(const std::string& key);
		bool json_is_boolean(const std::string& key);
		bool json_is_object(const std::string& key);
		bool json_is_array(const std::string& key);
		bool json_is_string(const std::string& key);
		bool json_is_number(const std::string& key);
		bool json_is_integer(const std::string& key);
		bool json_is_double(const std::string& key);
		//////////////////////////////////
	private:
		sqlite3 * database;
		static int callback(void *NotUsed, int argc, char **argv, char **azColName);
		///////////////
};
/*
usage:
	Data * data = new Data();
	if(!data->open("dokun.out"))
		std::cout << Logger("COuld not open sql database", 0) << std::endl;
	// table
	data->table("Player");
	data->column("Player", "name", "TEXT", "ADD");       // add variable 'name'  to table Player
	data->column("Player", "level", "INTEGER(3)", "ADD");       // add variable 'level' to table Player
	// column 1
	data->insert("Player", "name, level", "'Sid', 20" ); // initialize column 1 variables
	// column 2
	data->insert("Player", "name, level", "'Jack', 16"); // initialize column 2 variables
	// SET name WHERE id = 1
	data->update("Player", "name", "'Sid Dokugawa'", "id = 1"); // This will change name if id = 1
	data->update("Player", "name", "'Sid Dokugawa'");           // This will change all names to 'Sid Dokugawa'
	// delete column
	data->drop("Player", "id = 2"); // delete where id = 2
	
	data->select("Player", "name"); // print tables with that have var 'name' (crashes in lua)
	
	data->drop("Player"); // destroy table
	data->close();        // close database
*/
#endif
#endif