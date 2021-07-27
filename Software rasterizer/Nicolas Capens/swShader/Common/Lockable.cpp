#include "Lockable.hpp"

#include "Error.hpp"

namespace swShader
{
	Lockable::Lockable()
	{
		lockCount = 0;
	}

	Lockable::~Lockable()
	{
		if(lockCount)
		{
			throw Error("Attempted to delete locked object");
		}
	}

	void Lockable::aquire()
	{
		lockCount++;
	}

	void Lockable::release()
	{
		lockCount--;

		if(lockCount < 0) lockCount = 0;
	}
}