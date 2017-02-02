Renderer
Version 1.2.0

Does vertex transformation using a user supplied VertexShader.
The output coordinates x, y, z and w are all to be specified in 
fixed point 16.16 format.

The clipping and rasterization pipeline only uses integer arithmetic.
The rasterizer is block based and currently uses 8x8 sized blocks.
It does perspective correct interpolation for the corners of the block
and linear within each block.

The Renderer can be configured with a vertex shader and a block shader.
A block shader is just like a pixel/fragment shader but it is supposed to work
on a whole block instead of just a single pixel.

Following are examples for a vertex and block shader:

class BaseVertexShader {
public:
	// how many different attribute sources does this shader use
	static const int attribute_count = 1;

	static void shade(const Renderer::VertexInput in, Renderer::VertexOutput &out)
	{
		// VertexInput is an array out pointers pointing to the different 
		// attribute arrays specified by vertex_attrib_pointer. You may want to 
		// cast these to your vertex structure.

		// Process the vertex, apply tranformation and lighting etc. and then
		// ALWAYS write x, y, z and w of the VertexOutput out.
		// Writing to the varyings is optional.
		// The outputs x, y, z and w are all to be specified in 16.16 fixed
		// point format since the clipping and triangle setup pipeline assumes 
		// this format. For the varyings you can use whatever format you like
		// since these are then passed directly to the rasterizer which 
		// interpolates them
	}
};

class BaseFragmentShader {
public:
	static const int  varying_count = 0;
	static const bool interpolate_z = false;
	
	// this function is used when rendering points or lines as there only single
	// fragments are generated. This function always needs to be defined even
	// when not using line rendering so that everything compiles
	static void single_fragment(int x, int y, const Rasterizer::FragmentData &fd)
	{}
};


