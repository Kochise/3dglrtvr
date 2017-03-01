/*
 * Copyright (c) 2002-2004, xRhino, Inc.
 *
 * This library is free software; you can redistribute and/or modify
 * it under the terms of the dual license distributed with this source
 * package (a combined Trolltech QPL/GPL-compatible license)
 *
 * Under this dual license scheme, a separate license is available for use of 
 * this code with a proprietary (closed) system. For more information on the 
 * license, please see the LICENSE file distributed with this source package.
 */
#include <values.h>
#include <fstream>
#include <glam/GmOptions.h>

#define GMOPTION_BUFFER_SIZE 4096

// The singleton instance
GmOptions* gGmOptionsInstance = NULL;

GmOptions::GmOptions()
{
   mColorOptions[GMO_TEXT_COLOR] = new GlmColor4(1.0, 1.0, 1.0, 1.0);
   mStringOptions[GMO_TEXT_FONT] = "\"/usr/share/fonts/truetype/ttf-bitstream-vera/Vera.ttf\"";
   mStringOptions[GMO_TEXT_FONTTYPE] = "Texture";
   mScalarOptions[GMO_TEXT_SIZE] = 10.0;
   mScalarOptions[GMO_TEXT_RESOLUTION] = 960.0;
}

GmOptions::~GmOptions()
{
}

GmOptions* GmOptions::getInstance()
{
   if(gGmOptionsInstance == NULL)
      gGmOptionsInstance = new GmOptions();
   
   return gGmOptionsInstance;
}

GmOptions* GmOptions::getInstance(char* optionsFile)
{
   GmOptions* options = GmOptions::getInstance();
   
   options->loadConfigurationFile(optionsFile);
   
   return gGmOptionsInstance;
}

bool GmOptions::loadConfigurationFile(char* configFile)
{
   ifstream cf;

   cf.open(configFile);

   // Check to see if the file exists
   if(!cf)
   {
      cout << "GmOptions: Could not open configuration file '"
           << configFile << "'" << endl;

      return false;
   }
   else
   {
      // Process each line in the configuration file
      while(!cf.eof())
      {
         char optionLine[256];

         cf.getline(optionLine, 256);
         processOptionLine(optionLine);
      }
   }

   return true;
}

const char* GmOptions::getStringOption(char* option)
{
   const char* retval = NULL;
   
   //cout << "Getting " << option << endl;
   
   if(mStringOptions.find(option) != mStringOptions.end())
   {
      retval = mStringOptions[option].c_str();
   }
   else
   {
      cout << "GMO: '" << option << "' not found!" << endl;
   }
   
   return retval;
}

const float GmOptions::getScalarOption(char* option)
{
   float retval = MAXFLOAT;
   
   //cout << "Getting " << option << endl;

   if(mScalarOptions.find(option) != mScalarOptions.end())
   {
      retval = mScalarOptions[option];
   }
   
   return retval;
}

const GlmColor4* GmOptions::getColorOption(char* option)
{
   map<string, GlmColor4*>::iterator p;
   GlmColor4* retval = NULL;

   //cout << "Getting " << option << endl;
   
   p = mColorOptions.find(option);

   if(p != mColorOptions.end())
   {
      retval = (*p).second;
   }
   else
   {
      cout << "GMO: '" << option << "' not found!" << endl;
   }
   
   return retval;
}

void GmOptions::processOptionLine(char* optionLine)
{
   if((optionLine[0] != '#') && (strlen(optionLine) > 1))
   {
      // Find out the type of datum we are dealing with
      if(index(optionLine, '"') != NULL)
         setStringOption(optionLine);
      else if((index(optionLine, '(') != NULL) &&
              (index(optionLine, ')') != NULL))
         setColorOption(optionLine);
      else
         setScalarOption(optionLine);
   }
}

void GmOptions::setStringOption(char* stringOption)
{
   char* key;
   char* value;
   char* strptr = NULL;

   key = strtok_r(stringOption, " ", &strptr);

   // parse the value string
   if(key != NULL)
   {
      value = strtok_r(NULL, "\"", &strptr);

      if(value != NULL)
      {
         mStringOptions[key] = value;
         //cout << "'"  << key << "' = '" << mStringOptions[key] << "'" << endl;
      }
   }
}

void GmOptions::setScalarOption(char* floatOption)
{
   char* key;
   char* strptr = NULL;

   key = strtok_r(floatOption, " ", &strptr);

   // parse the color information
   if(key != NULL)
   {
      char* value;

      value = strtok_r(NULL, " ", &strptr);

      // convert the string value into a floating point value
      if(value != NULL)
      {
         float fvalue;

         fvalue = atof(value);

         mScalarOptions[key] = fvalue;
         //cout << "'"  << key << "' = " << mScalarOptions[key] << endl;
      }
   }
}

void GmOptions::setColorOption(char* colorOption)
{
   char* key;
   char* newKey;
   char* strptr = NULL;

   key = strtok_r(colorOption, " ", &strptr);

   // parse the color information
   if(key != NULL)
   {
      char* red;
      char* green;
      char* blue;
      char* alpha;

      red = strtok_r(NULL, " ,()", &strptr);
      green = strtok_r(NULL, " ,()", &strptr);
      blue = strtok_r(NULL, " ,()", &strptr);
      alpha = strtok_r(NULL, " ,()", &strptr);

      // convert the string values into floating point values
      if(red != NULL && green != NULL && blue != NULL && alpha != NULL)
      {
         float r, g, b, a;
         GlmColor4* color = new GlmColor4();

         r = atof(red);
         g = atof(green);
         b = atof(blue);
         a = atof(alpha);

         color->setRGBA(r, g, b, a);
         mColorOptions[key] = color;
         //cout << "'"  << key << "' = " << *mColorOptions[key] << endl;
      }
      else
         delete newKey;
   }
}
