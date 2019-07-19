#include "../include/grid.h"

Grid::Grid() : rows(0), columns(0), color(64, 64, 64, 255),
// outline
outline(true), 
outline_width(2.0), 
outline_antialiased(false),
outline_color(255, 255, 255, 255),
// highlight
highlight(false),
highlight_color(0, 51, 102, 255)
{
	set_position(0, 0);
	set_size(50, 50); // size of each individual block in grid
	set_orientation(0);
}
Grid::Grid(int row, int column) :  color(64, 64, 64, 255),
// outline
outline(true), 
outline_width(2.0), 
outline_antialiased(false),
outline_color(255, 255, 255, 255),
// highlight
highlight(false),
highlight_color(0, 51, 102, 255)
{
	set_position(0, 0);
	set_size(50, 50); // size of each individual block in grid
	set_orientation(0);
	set_column(column);
	set_row(row);
}
int Grid::grid_new(lua_State *L)
{
	lua_settop(L, 0); // clear stack	
	lua_createtable(L, 0, 0);      // create table
	lua_getglobal(L, "Grid_mt");
	lua_setmetatable(L, 1);        // set metatable
	Grid ** grid = static_cast<Grid **>(lua_newuserdata(L, sizeof(Grid*))); // set userdata
	*grid = new Grid;
	lua_setfield(L, 1, "udata");
	return 1; 	// return the newly created table
}
Grid::~Grid()
{
    block.clear();
}
/////////////
/////////////
void Grid::draw() // COMPLETE! :D
{
	if(is_visible())
	{
		for(int i = 0; i < block.size(); i++) // block.size() = rows
		{
			for(int j = 0; j < block[i].size(); j++) // block[i] = items in block i
			{		
				block[i][j].set_position(get_position().x + j * get_width(), get_position().y + i * get_height());
                // Draw grid
 				Renderer::draw_box(block[i][j].get_position().x, block[i][j].get_position().y, get_width(), 
			        get_height(), get_angle(), get_scale().x, get_scale().y, block[i][j].get_color().x, block[i][j].get_color().y, block[i][j].get_color().z, block[i][j].get_color().w,
			        block[i][j].radius, false, false, 0, color, false, false, false, block[i][j].get_color(), outline, outline_width, outline_color, outline_antialiased, false, 0.0, color, block[i][j].gradient, block[i][j].gradient_color, false);
				// Draw label - don't draw label but set parent to this instead and it'll be drawn automatically
				// Draw image
				Image * image = block[i][j].get_image();
                if(image != nullptr)
			    {
					image->set_position(block[i][j].get_x(), block[i][j].get_y());	
                    image->set_scale(image->get_aspect_ratio_correction(get_width(), get_height()),
					    image->get_aspect_ratio_correction(get_width(), get_height()));	// equivalent to Image::scale_to_fit
				    image->draw();
				}				
				if(highlight)
				{
				    // on hover
                    if(Mouse::is_over(block[i][j].get_position().x+j, block[i][j].get_position().y+i, get_width(), get_height()))
					    block[i][j].set_color(highlight_color);
				    else block[i][j].set_color(get_color());					
				}
			}
		}	
	}
	on_draw(); // callback for all gui
} // nothing will be drawn if rows and columns are not set
int Grid::draw(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Grid * grid = *static_cast<Grid **>(lua_touserdata(L, -1));
		grid->draw();
	}
    return 0;
}
/////////////
void Grid::update()
{	
	block.clear(); // clear rows
	for(int i = 0; i < rows; i++)
	{
		// create empty rows
		block.push_back(std::vector<Widget>());
		// store row items based on # of columns
		for(int j = 0; j < columns; j++)
		{
			block[i].push_back(Widget());
		}
	}	
}
int Grid::update(lua_State *L)
{}
/////////////
/////////////
void Grid::set_row(int rows)
{
	this->rows = rows;
	update(); // update block each time row is changed
}              
int Grid::set_row(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Grid * grid = *static_cast<Grid **>(lua_touserdata(L, -1));
		grid->set_row(lua_tonumber(L, 2));
	}
    return 0;	
}
void Grid::set_column(int columns)
{
	this->columns = columns;
	update(); // update block each time column is changed
}              
int Grid::set_column(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Grid * grid = *static_cast<Grid **>(lua_touserdata(L, -1));
		grid->set_column(lua_tonumber(L, 2));
	}
    return 0;	
}
void Grid::set_block(int rows, int columns)
{
	set_row(rows);
	set_column(columns);
}
int Grid::set_block(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Grid * grid = *static_cast<Grid **>(lua_touserdata(L, -1));
		grid->set_block(lua_tonumber(L, 2), lua_tonumber(L, 3));
	}
    return 0;		
}
void Grid::set_color(int red, int green, int blue, int alpha)
{
	color = Vector4(red, green, blue, alpha);
}
void Grid::set_color(const Vector3& color)
{
	set_color(color.x, color.y, color.z);
}
void Grid::set_color(const Vector4& color)
{
	set_color(color.x, color.y, color.z, color.w);
}
int Grid::set_color(lua_State *L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	luaL_checktype(L, 2, LUA_TNUMBER);
	luaL_checktype(L, 3, LUA_TNUMBER);
	luaL_checktype(L, 4, LUA_TNUMBER);
	luaL_optnumber(L, 5, 255);
	std::cout << "alpha "<< lua_tonumber(L, 5) << "\n";
	lua_getfield(L, 1, "udata");
	if(lua_isuserdata(L, -1))
	{
		Grid * grid = *static_cast<Grid **>(lua_touserdata(L, -1));
		grid->set_color(lua_tonumber(L, 2), lua_tonumber(L, 3), lua_tonumber(L, 4), lua_tonumber(L, 5));
	}
    return 0;	
}
/////////////
/////////////
Widget * Grid::get_block(int row, int column)
{
	if(block.size() < row + 1) 
	{
		Logger("Attempt to access invalid location | grid.cpp (114)");
		return nullptr;
	}
	return &block[row][column];
}              
int Grid::get_block(lua_State *L)
{}
int Grid::get_row_count()const
{
	return rows;
}
int Grid::get_row_count(lua_State *L)
{}
int Grid::get_column_count()const
{
	return columns;
}
int Grid::get_column_count(lua_State *L)
{}
Vector4 Grid::get_color()const
{
	return color;
}              
int Grid::get_color(lua_State *L)
{}
/*
	std::cout << "rows " << block.size() << "\n"; 
	for(int k = 0; k < block.size(); k++)
	    std::cout << "row items in block [" << k << "] " << block[k].size() << "\n";
*/
