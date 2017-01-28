#ifndef X86Shader_hpp
#define X86Shader_hpp

namespace swShader
{
	class X86Shader
	{
	public:
		virtual void (*shader())() = 0;
	};
}

#endif   // X86Shader_hpp