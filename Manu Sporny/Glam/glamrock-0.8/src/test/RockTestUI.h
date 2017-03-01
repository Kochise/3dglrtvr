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

#ifndef ROCKTESTUI_H
#define ROCKTESTUI_H

#include <glamrock/glamrock.h>

enum ComponentID
{
   BUTTON_DIALOG_ONE_OK,
   BUTTON_DIALOG_TWO_DISMISS,
   BUTTON_EXPANDERPANEL_SELECT,
   BUTTON_EXPANDERTOOLBAR_TANK,
   BUTTON_EXPANDERTOOLBAR_PLANE,
   RADIO_BUTTON_ONE,
   RADIO_BUTTON_TWO,
   RADIO_BUTTON_THREE,
   CHECKBOX,
   MENU_FILE,
   MENU_EDIT,
   MENU_TEST,
   MENU_VIEW,
   MENU_HELP,
   MENU_TEST_FRAME,
   MENU_FILE_OPEN,
   MENU_FILE_NEW,
   MENU_FILE_QUIT,
   MENU_FILE_PRINT,
   MENU_FILE_CLOSE,
   MENU_EDIT_CUT,
   MENU_EDIT_COPY,
   MENU_EDIT_PASTE,
   MENU_EDIT_FIND,
   MENU_VIEW_ZOOM_IN,
   MENU_VIEW_ZOOM_OUT,
   MENU_VIEW_CENTER,
   MENU_HELP_ABOUT,
   MENU_HELP_MANUAL,
   MENU_HELP_TUTORIALS,
   MENUBAR,
   EXPANDERPANEL,
   EXPANDERTOOLBAR
};

/**
 * The user interface class. This class defines UI components and handles
 * all events in this test application for GLAMROCK.  This class
 * implements two even handlers, a mouse event handler and a widget
 * event handler.
 */
class RockTestUI : public GmSelectionListener, public GmButtonListener
{
 private:

   /* Get the rendering options */
   GmTypes::RenderOptionType renderOptions;

   /* Button used for testing the glam library */
   GrDialog* testDialog1;
   GrDialog* testDialog2;
   GrPanel* testPanel1;
   GrPanel* testPanel2;
   GmScrollablePane* testScrollablePane;
   GmTextArea* testTextArea;
   GrPushButton* testButton1;
   GrPushButton* testButton2;
   GrPushButton* testButton3;
   GrPushButton* tankButton;
   GrPushButton* planeButton;
   GrRadioButton* testR1Button;
   GrRadioButton* testR2Button;
   GrRadioButton* testR3Button;
   GmRadioButtonGroup* testRadioButtonGroup;
   GrCheckBox* testCheckBox;
   GrBarMeter* testBarMeter;
   GrExpanderPanel* testExpanderPanel;
   GrExpanderToolbar* testExpanderToolbar;
   GrMenu* testFileMenu;
   GrMenu* testEditMenu;
   GrMenu* testTestMenu;
   GrMenu* testViewMenu;
   GrMenu* testHelpMenu;
   GrMenuBar* testMenuBar;
   GrToolTip* testExpanderPanelToolTip;
//   GrScrollbar* testScrollbar;
   //   GmScrollableListBox* testScrollableListBox;
   GrFrame* testFrame;
   
   void setupDialog1();
   void setupDialog2();
   void setupExpanderToolbar();
   void setupMenuBar();
   void setupFrame();
   
   void createFrame();

 public:

   /**
    * Constructor
    */
   RockTestUI();

   /**
    * Standard destructor
    */
   virtual ~RockTestUI();

   void setRenderOptions(GmTypes::RenderOptionType options);
   void renderWidget(GmDrawable *widget);

   void renderUI();

   void initUI();

   void applyDialog1Layout();
   void applyDialog2Layout();
   void applyMenuLayout();
   void applyExpanderToolbarLayout();
   void applyFrameLayout();

   void print_usage();

   /* Accessors */

   GrDialog* getDialog1();
   GrDialog* getDialog2();
   GrMenuBar* getMenuBar();
   GrBarMeter* getBarMeter();
   GrExpanderToolbar* getExpanderToolbar();

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

   
