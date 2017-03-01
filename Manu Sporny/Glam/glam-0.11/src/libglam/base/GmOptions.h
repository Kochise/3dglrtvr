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
#ifndef GMOPTIONS_H
#define GMOPTIONS_H

#include <glmath/GlmColor4.h>
#include <glam/GmConfig.h>
#include <glam/GmTypes.h>

#define GMO_DIALOG_BASECOLOR "Dialog.BaseColor"
#define GMO_DIALOG_TITLEBARCOLOR "Dialog.TitleBarColor"
#define GMO_DIALOG_TITLETEXTCOLOR "Dialog.TitleTextColor"
#define GMO_PANEL_BASECOLOR "Panel.BaseColor"
#define GMO_TEXT_FONT "Text.Font"
#define GMO_TEXT_FONTTYPE "Text.FontType"
#define GMO_TEXT_COLOR "Text.Color"
#define GMO_TEXT_RESOLUTION "Text.Resolution"
#define GMO_TEXT_SIZE "Text.Size"
#define GMO_HYPERLINK_COLOR "Hyperlink.Color"
#define GMO_TOOLTIP_POPUPTIMEOUT "ToolTip.PopupTimeout"
#define GMO_TOOLTIP_BASECOLOR "ToolTip.BaseColor"
#define GMO_TOOLTIP_TEXTCOLOR "ToolTip.TextColor"
#define GMO_MENUBAR_BASECOLOR "MenuBar.BaseColor"
#define GMO_MENU_BASECOLOR "Menu.BaseColor"
#define GMO_MENU_TEXTCOLOR "Menu.TextColor"
#define GMO_PUSHBUTTON_BASECOLOR "PushButton.BaseColor"
#define GMO_PUSHBUTTON_TEXTCOLOR "PushButton.TextColor"
#define GMO_PUSHBUTTON_BORDERWIDTH "PushButton.BorderWidth"
#define GMO_SCROLLBAR_BASECOLOR "Scrollbar.BaseColor"
#define GMO_SCROLLBAR_BORDERWIDTH "Scrollbar.BorderWidth"
#define GMO_RADIOBUTTON_BASECOLOR "RadioButton.BaseColor"
#define GMO_CHECKBOX_BASECOLOR "CheckBox.BaseColor"
#define GMO_CHECKBOX_CHECKCOLOR "CheckBox.CheckColor"
#define GMO_METER_BORDERWIDTH "Meter.BorderWidth"
#define GMO_BARMETER_BASECOLOR "BarMeter.BaseColor"
#define GMO_BARMETER_BARCOLOR "BarMeter.BarColor"

/**
 * The GmOptions class controls all GLAM options available to the
 * user. Some configurable options include: display resolution,
 * default size, etc
 */
class GmOptions
{
 protected:
   /**
    * The database of scalar configuration keys and values
    */
   map<string, float> mScalarOptions;

   /**
    * The database of color configuration keys and values
    */
   map<string, GlmColor4*> mColorOptions;

   /**
    * The database of string configuration keys and values
    */
   map<string, string> mStringOptions;
   
   /**
    * Protected constructor for singleton class
    */
   GmOptions();

   /**
    * Virtual protected destructor for singleton class
    */
   virtual ~GmOptions();

 public:
   /**
    * Gets the instance of the GLAM options object
    */
   static GmOptions* GmOptions::getInstance();
   
   /**
    * Gets the instance of the GLAM options object and loads the given
    * configuration file.
    *
    * @param configFile the configuration file to load
    */
   static GmOptions* GmOptions::getInstance(char* configFile);

   /**
    * Loads a set of configurations from a configuration file. The
    * configuration file format can be in the form of:<p>
    * <i>TOKEN VALUE</i><p>
    *
    * <i>VALUE</i> can be either a floating point scalar or RGBA
    * quadlet. For example:<p>
    *
    * Text.Resolution 200.0<br>
    * Text.Color 0.35 0.37 0.31 1.0<p>
    *
    * @param configFile the configuration file to load the setup
    *                   information from.
    * @return true if config file loaded, false if not
    */
   virtual bool loadConfigurationFile(char* configFile);

   /**
    * Gets a string option value from the options database.
    *
    * @param key the string key value to retrieve
    * @return the text string that is associated with the given key
    */
   virtual const char* getStringOption(char* key);

   /**
    * Gets a scalar point value from the options database.
    *
    * @param key the scalar key value to retrieve
    * @return the scalar value associated with the given key
    */
   virtual const float getScalarOption(char* key);

   /**
    * Gets a color value from the keys database.
    *
    * @param key the color value to get from the options database
    * @return the color associated with the given key
    */
   virtual const GlmColor4* getColorOption(char* key);
   
 protected:
   /**
    * This method processes a complete option line by
    * performing strict syntax checking on it as well as identifying
    * what sort of configuration option it is.
    */
   virtual void processOptionLine(char* optionLine);

   /**
    * Sets a string option in the options database. The string option
    * can look something like:<p>
    *
    * <tt>Text.Font "/usr/share/font/truetype/Arial.ttf"</tt><p>
    *
    * @para stringOption the string option, which should include a
    *                    key, value pair.
    */
   virtual void setStringOption(char* stringOption);

   /**
    * Sets a single  scalar option in the options database. The
    * string should look something like the following:<p>
    *
    * <tt>Text.Size 14.0</tt><p>or<p>
    *
    * @param scalarOption the option string, which should include a key
    *                    value(s) pair.
    */
   virtual void setScalarOption(char* scalarOption);

   /**
    * Sets a color option in the options database. The
    * string should look something like the following:<p>
    *
    * <tt>Text.Color 0.24 0.74 0.70 1.0</tt><p>
    *
    * @param colorOption the option string, which should include a key
    *                    value(s) pair.
    */
   virtual void setColorOption(char* colorOption);
};

#endif
