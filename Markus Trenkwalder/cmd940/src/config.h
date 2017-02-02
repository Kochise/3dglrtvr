#ifndef CONFIG_H_74233FA7_9EB3_47f2_8C90_EC0846F68ACF
#define CONFIG_H_74233FA7_9EB3_47f2_8C90_EC0846F68ACF

#include <cstddef>

namespace cmd940i {
	// Sizes of each ring buffer. Make them multiples of 4 to prevent alignment 
	// problems.
	static const size_t RING_BUFFER_SIZE_920_TO_940 = 768 * 1024;
	static const size_t RING_BUFFER_SIZE_940_TO_920 = 256 * 1024;

	// Start address in upper memory. First comes the 920_TO_940 buffer, 
	// directly after it comes the 940_TO_920 buffer. 
	// If this is changed the region setup in the startup code has to be 
	// adjusted as well.
	static const size_t RING_BUFFER_LOCATION = 0x100000;

	// If defined this provides a dummy operator delete to prevent linking 
	// problems when using classes with a virtual destructor (which every class 
	// using virtual functions should have). If you provide your own don't 
	// define this.
	//#define PROVIDE_DUMMY_DELETE

	// If you have static class instances in your functions you probably want to
	// define this. I don't realy know if it actually works.
	//#define STATIC_CLASS_GUARDS
}

#endif
