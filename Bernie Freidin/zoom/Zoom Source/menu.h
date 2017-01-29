#ifndef MENU_H
#define MENU_H

struct minfo_t
{
	int    preset_count; // -1 is boolean
	double preset_data[10];
	double data_min;
	double data_max;
	char   prompt[200];
	char   string[200];
	int    parent_id;
	int    entry_id;
	void  *ptr;
};

int MENU_Register(int preset_count, char preset_data_str[],
                  double data_min, double data_max, double data,
                  char string[], int parent_id, int entry_id, void *ptr);

#endif
