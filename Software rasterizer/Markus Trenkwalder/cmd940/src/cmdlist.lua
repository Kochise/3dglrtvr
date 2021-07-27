return {
	--[[
	
	In this file you define the functions for which the stubs shall be 
	generated.
	
	Format: {"rtype", "fctname", {{"ptype", "pname"},{"ptype", "pname", "&"}}}
	
	Rtype is the function return type. It can be any C/C++ type, also structs 
	if you want. The function name is given by fctname. After the function name
	follows a list of parameters for which ptype is the type of the parameter.
	It can be any C/C++ type, also structs. If you want to have a pointer you 
	put the * inside ptype. Pname is the name of the parameter. There is an 
	optional third argument for a parameter which is "&". This turns the 
	parameter into a const reference type. This is useful to reduce internal 
	copying and thus gain some speed.
	
	--]]
	
	-- Example:
	--[[
	{"void", "test1", {}},
	{"void", "test2", {{"int", "x"}, {"int", "y" }}},
	{"int" , "test3", {}},
	{"int" , "test4", {{"int", "x"}}},
	
	{"void", "function", {{"struct_t", "s", "&"}}}
	--]]
}