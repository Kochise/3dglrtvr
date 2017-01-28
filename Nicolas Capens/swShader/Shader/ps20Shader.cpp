#include "PS20Shader.hpp"

namespace swShader
{
	PS20Shader::PS20Shader(const char *shaderFile) : file(shaderFile), ps20_parser(&file)
	{
		try
		{
			main();
			file.close();
			encode();
		}
		catch(ScanException &error)
		{
			throw Error(error.what());
		}
		catch(ParseException &error)
		{
			throw Error(error.what());
		}
	}

	PS20Shader::~PS20Shader()
	{
	}
}