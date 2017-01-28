#ifndef Lockable_hpp
#define Lockable_hpp

namespace swShader
{
	class Lockable
	{
	public:
		Lockable();

		~Lockable();

		void aquire();
		void release();

	private:
		int lockCount;
	};
}

#endif   // Lockable_hpp