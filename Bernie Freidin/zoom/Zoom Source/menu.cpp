#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gl/glut.h>
#include "menu.h"

static minfo_t MENU_LIST[20];

static void _menuCallback(int value)
{
	int menu_id = glutGetMenu();
	minfo_t *minfo = &MENU_LIST[menu_id];
	double data;
	char buf[200];
	
	if(minfo->preset_count < 0)
	{
		// ************
		// Boolean menu
		// ************
		
		if(value == 1)
		{
			sprintf(buf, "%s (on)", minfo->string);
		}
		else
		{
			sprintf(buf, "%s (off)", minfo->string);
		}
		*(int*)minfo->ptr = 2 - value;
	}
	else
	{
		// *********
		// Data menu
		// *********
		
		if(value-1 < minfo->preset_count)
		{
			data = minfo->preset_data[value-1];
		}
		else
		{
			fprintf(stdout, "Enter %s: ", minfo->prompt);
			fgets(buf, 199, stdin);
			data = atof(buf);
			if(data < minfo->data_min) data = minfo->data_min;
			if(data > minfo->data_max) data = minfo->data_max;
		}
		sprintf(buf, "%s (%.4f)", minfo->string, data);
		
		*(double*)minfo->ptr = data;
	}
	glutSetMenu(minfo->parent_id);
	glutChangeToSubMenu(minfo->entry_id, buf, menu_id);
	glutPostRedisplay();
}

int MENU_Register(int preset_count, char preset_data_str[],
                  double data_min, double data_max, double data,
                  char string[], int parent_id, int entry_id, void *ptr)
{
	int i, menu_id = glutCreateMenu(_menuCallback);
	minfo_t *minfo = &MENU_LIST[menu_id];
	
	minfo->preset_count = preset_count;
	minfo->data_min = data_min;
	minfo->data_max = data_max;
	minfo->ptr = ptr;
	strncpy(minfo->prompt, string, 199);
	strncpy(minfo->string, string, 199);
	
	for(i = 0; minfo->prompt[i] != '\0'; i++)
	{
		minfo->prompt[i] = tolower(minfo->prompt[i]);
	}
	minfo->parent_id = parent_id;
	minfo->entry_id = entry_id;
	
	char buf[500], *s = buf;
	strncpy(s, preset_data_str, 499);
	
	if(preset_count < 0)
	{
		// ************
		// Boolean menu
		// ************
		
		glutAddMenuEntry("On", 1);
		glutAddMenuEntry("Off", 2);
		
		if(data > 0.0)
		{
			sprintf(buf, "%s (on)", minfo->string);
		}
		else
		{
			sprintf(buf, "%s (off)", minfo->string);
		}
		*(int*)minfo->ptr = (int)data;
	}
	else
	{
		// *********
		// Data menu
		// *********
		
		for(i = 0; i < preset_count; i++)
		{
			char *s1 = strtok(s, " ");
			minfo->preset_data[i] = atof(s1);
			glutAddMenuEntry(s1, i+1);
			s = NULL;
		}
		glutAddMenuEntry("Set via console...", i+1);
		
		sprintf(buf, "%s (%.4f)", minfo->string, data);
		*(double*)minfo->ptr = data;
	}
	glutSetMenu(parent_id);
	glutAddSubMenu(buf, menu_id);
	
	return menu_id;
}

