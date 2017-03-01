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
#ifndef GMWIDGETTEST_H
#define GMWIDGETTEST_H

#include <glam/glam.h>
#include <DragText.h>

enum WidgetID
{
   HELLO_WORLD_BUTTON,
   RADIO_GROUP_TEST,
   CHECKBOX_TEST,
   HYPERLINK_TEST,
   RB_GROUP,
   RB_ONE,
   RB_TWO,
   RB_THREE,
   MENUBAR,
   MENU_FILE,
   MENU_FILE_NEW,
   MENU_FILE_OPEN,
   MENU_FILE_CLOSE,
   MENU_FILE_PRINT,
   MENU_FILE_QUIT,
   MENU_EDIT,
   MENU_EDIT_CUT,
   MENU_EDIT_COPY,
   MENU_EDIT_PASTE,
   MENU_EDIT_FIND,
   MENU_SUB,
   MENU_SUBSUB,
   MENU_SUBSUB_STUFF,
   MENU_SUB_SAVE,
   MENU_SUB_SAVEAS,
   COMBOBOX_APPLES,
   COMBOBOX_ORANGES,
   COMBOBOX_KIWIS,
   MENU_SOLO,
   TABLE
};

/**
 * The user interface class. This class defines UI components and handles
 * all events in this test application for GLAM.
 */
class WidgetTestUI : public GmButtonListener,
                     public GmSelectionListener,
                     public GmHyperlinkListener
{
 private:

   /* Get the rendering options */
   GmTypes::RenderOptionType mRenderOptions;

   /* Widgets used in this tutorial of the glam library */
   GmPanel* mTestPanel;
   GmPushButton* mTestButton;
   GmLabel* mTestLabel;
   GmCheckBox* mTestCheckBox;
   GmPixmapIcon* mTestPixmapIcon;
   GmSvgIcon* mTestSvgIcon;
   GmPushButton* mTestGraphicButton;
   GmRadioButtonGroup* mTestRadioButtonGroup;
   GmRadioButton* mTestRadioButton1;
   GmRadioButton* mTestRadioButton2;
   GmRadioButton* mTestRadioButton3;
   GmMenuBar* mTestMenuBar;
   GmMenu* mTestFileMenu;
   GmMenu* mTestEditMenu;
   GmMenu* mTestSubMenu;
   GmMenu* mTestSubSubMenu;
   GmHyperlink* mTestHyperlink;
   GmBarMeter* mTestBarMeter;
   GmSlider* mTestSlider;
   GmScrollbar* mTestScrollbar;
   GmIconBar* mTestIconBar;

   GmPanel* mTestPanel2;
   GmTextArea* mTestTextArea;
   GmComboBox* mComboBox;
   GmLabel** mTestELILabel;
   GmLabel** mTestELIText;
   GmExpandableListItem** mTestExpandableListItem;
   GmTextField* mTextField;
   DragText* mDragText;
   GmScrollablePane* mTestListBox;
   GmPushButton** mTestButtons;
   GmExpandableListItem* mExpandableListItem;

   GmPanel* mTestPanel3;
   GmScrollablePane* mTablePane;
   GmTable* mTestTable;
   GmLabel* mTableLabel1;
   GmLabel* mTableLabel2;
   GmLabel* mTableLabel3;
   GmLabel* mTableLabel4;
   GmPushButton* mTableButton;
   
   void setupTestPanel();
   void setupTestPanel2();
   void setupTestPanel3();

 public:
   /**
    * Constructor
    */
   WidgetTestUI();

   /**
    * Standard destructor
    */
   virtual ~WidgetTestUI();

   void setRenderOptions(GmTypes::RenderOptionType options);

   void renderUI();

   void initUI();

   void applyTestPanelLayout();

   void updateBarMeter(float value);
   void updateLabel(char* text);

   void print_usage();
   
   /* Listener method declarations */ 
   
   /**
    * Called when a selection is made.
    *
    * @param action the action associated with the selection.
    */
   virtual void selectionOccurred(GmSelectionAction* action);

   /**
    * Called whenever an associated button is pressed.
    *
    * @param action the button action involved in the press.
    */
   virtual void buttonPressed(GmButtonAction* action);

   /**
    * Called whenever an associated hyperlink is pressed.
    *
    * @param action the hyperlink action involved in the press.
    */
   virtual void hyperlinkPressed(GmHyperlinkAction* action);
};

#endif
