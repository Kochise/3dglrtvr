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
#ifndef ROCKTUTORIALUI_H
#define ROCKTUTORIALUI_H

#include <glamrock/glamrock.h>

enum ComponentID
{
   GRID_DIALOG_BUTTON1,
   GRID_DIALOG_BUTTON2,
   GRID_DIALOG_BUTTON3,
   GRID_DIALOG_BUTTON4,
   GRID_DIALOG_BUTTON5,
   GRID_DIALOG_BUTTON6,
   GRID_DIALOG_BUTTON7,
   GRID_DIALOG_BUTTON8,
   GRID_DIALOG_BUTTON9,
   FLOW_DIALOG_CLOSE_BUTTON,
   ABOUT_DIALOG_OK_BUTTON,
   CHECKBOX_BUTTON,
   MENUBAR,
   MENU_FILE,
   MENU_FILE_NEW,
   MENU_FILE_NEW_FILE,
   MENU_FILE_NEW_FILE_DOCUMENT,
   MENU_FILE_OPEN,
   MENU_FILE_CLOSE,
   MENU_FILE_PRINT,
   MENU_FILE_QUIT,
   MENU_EDIT,
   MENU_EDIT_CUT,
   MENU_EDIT_COPY,
   MENU_EDIT_PASTE,
   MENU_EDIT_FIND,
   MENU_VIEW,
   MENU_VIEW_ZOOM_IN,
   MENU_VIEW_ZOOM_OUT,
   MENU_VIEW_CENTER,
   MENU_LAYOUT,
   MENU_LAYOUT_GRID,
   MENU_LAYOUT_FLOW,
   MENU_HELP,
   MENU_HELP_ABOUT,
   MJ_TEXT_ITEM,
   TF_TEXT_ITEM,
   DL_TEXT_ITEM
};

/**
 * The user interface class. This class defines UI components and handles
 * all events in this tutorial application for GLAM.  This class
 * implements two even handlers, a mouse event handler and a widget
 * event handler.
 */
class RockTutorialUI : public GmSelectionListener, public GmButtonListener
{
 private:

   /* Get the rendering options */
   GmTypes::RenderOptionType renderOptions;

   /* Widgets used in this tutorial of the glam library */
   GrDialog* mMainDialog;
   GmTextArea* mMainTextArea;

   GrMenuBar* mMainMenuBar;
   GrMenu* mFileMenu;
   GrMenu* mFileNewMenu;
   GrMenu* mFileNewFileMenu;
   GrMenu* mEditMenu;
   GrMenu* mViewMenu;
   GrMenu* mLayoutMenu;
   GrMenu* mHelpMenu;

   GrDialog* mGridDialog;
   GrPushButton* mGridButton1;
   GrPushButton* mGridButton2;
   GrPushButton* mGridButton3;
   GrPushButton* mGridButton4;
   GrPushButton* mGridButton5;
   GrPushButton* mGridButton6;
   GrPushButton* mGridButton7;
   GrPushButton* mGridButton8;
   GrPushButton* mGridButton9;

   GrDialog* mFlowDialog;
   GrPushButton* mFlowCloseButton;
   GmLabel* mLabel1;
   GmLabel* mLabel2;
   GmLabel* mLabel3;
   GmLabel* mLabel4;
   GmLabel* mLabel5;
   
   GrDialog* mAboutDialog;
   GrPushButton* mAboutOkButton;
   GmCheckBox* mCheckBox;
   GmExpandableListItem* mELItem;
   GmLabel* mELTitle;
   GmContentPane* mELBody;
   GmLabel* mELLabel1;
   GmComboBox* mComboBox;
   GmLabel* mELLabel2;
   GmRadioButton* mRadioButton;
   GrExpanderPanel* mHorzPanel;
   GrExpanderPanel* mVertPanel;
   
   void setupMainMenu();
   void setupMainDialog();

   void createGridDialog();
   void removeGridDialog();

   void createFlowDialog();
   void removeFlowDialog();
   
   void createAboutDialog();
   void removeAboutDialog();

 public:

   /**
    * Constructor
    */
   RockTutorialUI();

   /**
    * Standard destructor
    */
   virtual ~RockTutorialUI();

   void setRenderOptions(GmTypes::RenderOptionType options);
   void renderWidget(GmDrawable *widget);

   void renderUI();

   void initUI();

   void applyMainDialogLayout();

   void applyGridDialogLayout();

   void applyFlowDialogLayout();
   
   void applyAboutDialogLayout();
   
   void print_usage();

   void updateMainTitleText(string text);
   
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
};

#endif
