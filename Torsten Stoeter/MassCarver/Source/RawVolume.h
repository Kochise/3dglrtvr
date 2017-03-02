#ifndef RAW_VOLUME_H
#define RAW_VOLUME_H

class RawVolume
{
	private:
		char *data;
		int size;

	public:
		RawVolume(const char *filename);
		~RawVolume();

		char *getDataPointer();
		char getVoxel(int i);
};

#endif

