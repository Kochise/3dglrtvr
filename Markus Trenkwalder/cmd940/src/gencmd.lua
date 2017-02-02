--[[
Copyright (c) 2007, Markus Trenkwalder

All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, 
  this list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation 
  and/or other materials provided with the distribution.

* Neither the name of the library's copyright owner nor the names of its 
  contributors may be used to endorse or promote products derived from this 
  software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
--]]

local cmdlist = dofile "cmdlist.lua"

--------------------------------------------------------------------------------
-- utility functions

local function transform(tbl, fct)
	assert(type(tbl) == "table", "parameter #1 must be a table")
	assert(type(fct) == "function", "parameter #2 must be a function")
	
	local r = {}
	for k, v in pairs(tbl) do
		r[k] = fct(k, v)
	end
	return r
end

local function file_to_string(filename)
	local f = assert(io.open(filename))
	local s = f:read("*a")
	f:close()
	return s
end

local function param_list_to_string(params, include_type, include_modifier, var_prefix, separator)
	local r = {}
	for k, v in ipairs(params) do
		local ptype = v[1]
		local pname = v[2]
		local s = ""
		if include_type then
			s = s .. ptype .. " "
		end
		if include_modifier then
			local m = v[3]
			if m ~= nil then
				assert(m == "&")
				s = s .. "const& "
			end
		end
		s = s .. var_prefix .. pname
		table.insert(r, s)
	end
	return table.concat(r, separator)
end

local function file_exists(filename)
	local f = io.open(filename)
	if f then
		f:close()
		return true
	else
		return false
	end
end

--------------------------------------------------------------------------------

-- create cmds directory that will hold the individual command files
pcall(
	function ()
		if string.find(os.getenv("OS"), "Windows") then
			os.execute("mkdir cmd940\\cmds")
		else
			os.execute("mkdir cmd940/cmds")
		end
	end
)

-- generate each individual command file
table.foreachi(cmdlist,
	function (key, value)
		local rettype = value[1]
		local name = value[2]
		local params = value[3]
		local filename = "cmd940/cmds/" .. name .. ".cpp"
		
		if file_exists(filename) then return end
		
		local plist = param_list_to_string(params, true, true, "", ", ")
		local f = assert(io.open(filename, "w"))
		local s = rettype .." " .. name .. "(" .. plist ..")\n{\n\n}\n"
		f:write(s)
		f:close()
	end
)

local interface_declarations = transform(cmdlist, 
	function (key, value)
		local rettype = value[1]
		local name = value[2]
		local params = value[3]
		local plist = param_list_to_string(params, true, true, "", ", ")
		return rettype .. " " .. name .. "(".. plist .. ");"
	end
)

local cmdincludes = transform(cmdlist,
	function (key, value)
		local name = value[2]
		return "#include \"cmds/" .. name .. ".cpp\""
	end
)

local stubs940 = transform(cmdlist,
	function (id, value)
		local rettype = value[1]
		local name = value[2]
		local params = value[3]

		local sparamlist = param_list_to_string(params, true, false, "", ";\n\t\t") .. ";"
		local calllist = param_list_to_string(params, false, false, "p__.", ", ")

		local s
		if rettype == "void" then
			if #params == 0 then
				s = file_to_string("templates/stub940_nr_np.txt")
			else
				s = file_to_string("templates/stub940_nr.txt")
			end
		else
			if #params == 0 then
				s = file_to_string("templates/stub940_np.txt")
			else
				s = file_to_string("templates/stub940.txt")
			end
		end
		
		s = s:gsub(":RETTYPE:", rettype)
		s = s:gsub(":NAME:", name)
		s = s:gsub(":SPARAMLIST:", sparamlist)
		s = s:gsub(":CALLLIST:", calllist)
		return s
	end
)

local stubs940list = transform(cmdlist,
	function (id, value)
		local name = value[2]
		return "\t\t_" .. name .. ","
	end
)

local stubs920 = transform(cmdlist,
	function (id, value)
		local rettype = value[1]
		local name = value[2]
		local params = value[3]

		local paramlist = param_list_to_string(params, true, true, "", ", ")
		local sparamlist = param_list_to_string(params, true, false, "", ";\n\t\t\t") .. ";"
		local calllist = param_list_to_string(params, false, false, "p__.", ", ")
		local assignments = table.concat(
			transform(params,
				function (key, value)
					local name = value[2]
					return "p__." .. name .. " = " .. name .. ";"
				end
			),
			";\n\t"			
		)
		
		local s
		if rettype == "void" then
			if #params == 0 then
				s = file_to_string("templates/stub920_nr_np.txt")
			else
				s = file_to_string("templates/stub920_nr.txt")
			end
		else
			if #params == 0 then
				s = file_to_string("templates/stub920_np.txt")
			else
				s = file_to_string("templates/stub920.txt")
			end
		end
		
		s = s:gsub(":ID:", id - 1)
		s = s:gsub(":PARAMLIST:", paramlist)
		s = s:gsub(":RETTYPE:", rettype)
		s = s:gsub(":NAME:", name)
		s = s:gsub(":SPARAMLIST:", sparamlist)
		s = s:gsub(":CALLLIST:", calllist)
		s = s:gsub(":ASSIGNMENTS:", assignments)
		return s
	end
)

do
	local f = assert(io.open("../include/interface.h", "w"))
	local s = file_to_string("templates/interface.h.txt")
	s = s:gsub(":INTERFACE_DECLARATIONS:", table.concat(
		transform(
			interface_declarations, 
				function (k, v) return "\t" .. v end 
			),
			"\n"
		)
	)
	f:write(s)
	f:close()
end

do
	local f = assert(io.open("cmd940/cmds940.cpp", "w"))
	local s = file_to_string("templates/cmds940.cpp.txt")
	s = s:gsub(":INTERFACE_DECLARATIONS:", table.concat(interface_declarations, "\n"))
	s = s:gsub(":CMDINCLUDES:", table.concat(cmdincludes, "\n"))
	s = s:gsub(":STUBS940:", table.concat(stubs940, "\n"))
	s = s:gsub(":STUBS940LIST:", table.concat(stubs940list, "\n"))
	f:write(s)
	f:close()
end

do
	local f = assert(io.open("cmd920/cmds920.cpp", "w"))
	local s = file_to_string("templates/cmds920.cpp.txt")
	s = s:gsub(":STUBS920:", table.concat(stubs920, "\n"))
	f:write(s)
	f:close()
end
