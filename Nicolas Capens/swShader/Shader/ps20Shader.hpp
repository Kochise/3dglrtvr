#ifndef PS20Shader_hpp
#define PS20Shader_hpp

#include "ps20_parser.hpp"

namespace swShader
{
	class PS20Shader : public ps20_parser
	{
	public:
		PS20Shader(const char *shaderFile);

		~PS20Shader();

	private:
		std::ifstream file;
	};
}

#endif   // PS20Shader_hpp