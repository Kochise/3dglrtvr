#ifndef __Config
#define __Config

#include "Log.H"

class ConfigManager
{
private:
	
protected:

public:
	
	bool ReadCfg(const char *, const void*, unsigned long);
	bool WriteCfg(const char *, const void*, unsigned long);

	ConfigManager();
	~ConfigManager();
};

#endif