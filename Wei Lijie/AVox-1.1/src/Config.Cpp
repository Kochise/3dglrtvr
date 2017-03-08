#include "Config.H"

ConfigManager::ConfigManager()
{
}

ConfigManager::~ConfigManager()
{
}


bool ConfigManager::ReadCfg(const char *FileName, const void *Ptr, unsigned long recordSize)
{
	ifstream File(FileName, ios::binary | ios::nocreate);

	if (File.fail())
	{
		Log.Write("Red","Error: Cannot open file %s",FileName);
		return false;
	}

	// Check file size
	File.seekg(0, ios::end);
	if (File.tellg()!=recordSize) Log.Write("Red","Warning: Wrong size of configuration file: %i",File.tellg());
	File.seekg(0, ios::beg);
	
	File.read((char*) Ptr, recordSize);

	File.close();
	
	return true;
}


bool ConfigManager::WriteCfg(const char *FileName, const void *Ptr, unsigned long recordSize)
{
	ofstream File(FileName, ios::binary | ios::out);

	if (File.fail())
	{
		Log.Write("Red","Error: Cannot create file %s",FileName);
		return false;
	}
	
	File.write((char*)Ptr, recordSize);

	File.close();

	return true;
}
