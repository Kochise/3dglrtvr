#ifndef __VECTORTEXT_HPP_
#define __VECTORTEXT_HPP_

#include "Position.hpp"

typedef class VECTORTEXT {
	public:
		VECTORTEXT();
		~VECTORTEXT();
		
		void Draw(POSITION Pos, float letter_size, char *text);
};

#endif // __VECTORTEXT_HPP_
