#ifndef File_hpp
#define File_hpp

#include "Types.hpp"

#include <stdio.h>

namespace swShader
{
	class String;

	class File
	{
	public:
		File();
		File(const char *fileName);

		~File();

		void create();
		void rename();

		void open(const char *fileName);
		void close();

		bool changeSize(int size);
		int length() const;

		int position() const;

		bool seekSet(int offset = 0);
		bool seekCur(int offset);
		bool seekEnd(int offset = 0);

		bool endOfFile() const;

		char parseChar() const;
		short parseShort() const;
		int parseInt() const;
		float parseFloat() const;
		double parseDouble() const;

		unsigned int readString(char *stringBuffer) const;
		int read(void *buffer, int size, int count) const;
		unsigned char readByte() const;
		char readChar() const;
		unsigned short readWord() const;
		unsigned int readDWord() const;
		float readFloat() const;
		double readDouble() const;

		bool scan(const char *sequence);

		String drive() const;
		String path() const;
		String directory() const;
		String fileName() const;
		String extension() const;
		bool hasExtension(const char *ext) const;

		String getFullPath() const;

		void *getBuffer() const;
		char *getString() const;

		static bool exists(const char *fileName);

	private:
		FILE *fileHandle;

		char fullPath[512];   // FIXME: Use dynamical allocation
	};
}

#endif   // File_hpp