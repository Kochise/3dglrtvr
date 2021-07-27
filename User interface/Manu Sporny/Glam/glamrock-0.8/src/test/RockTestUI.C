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
#include <glam/glam.h>
#include <glamrock/glamrock.h>
#include <RockTestUI.h>

using namespace std;

RockTestUI::RockTestUI()
{
   renderOptions = GmTypes::RO_2D;
   testFrame = NULL;
}

RockTestUI::~RockTestUI()
{
}

void RockTestUI::setupDialog1()
{
   testDialog1 = new GrDialog();
   testDialog1->setRelativeGeometry(0.0, 0.0, 1.0, 1.0);
   testDialog1->setTitleText("Sample Dialog");
   testDialog1->setTitleTextSize(4.0);
   testPanel1 = new GrPanel();
   testPanel1->setRelativeGeometry(0.0, 0.45, 0.75, 1.0);

   testButton1 = new GrPushButton();
   testButton1->setText("OK");
   testButton1->setTextSize(4.0);
   testButton1->setRelativeGeometry(0.4, 0.0, 0.6, 0.1);
   testButton1->setWidgetId(BUTTON_DIALOG_ONE_OK);
   testButton1->addButtonListener(this);

   // Setup the radio buttons
   testR1Button = new GrRadioButton();
   testR1Button->setRelativeGeometry(0.0, 0.0, 1.0, 0.2);
   testR1Button->setBaseColor(0.65, 0.67, 0.81, 1.0);
   testR1Button->setBorderWidth(1.0);
   testR1Button->setText("Apples");
   testR1Button->setTextSize(4.0);
   testR1Button->setTextColor(0.25, 0.27, 0.41, 1.0);
   testR1Button->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);

   testR2Button = new GrRadioButton();
   testR2Button->setRelativeGeometry(0.0, 0.4, 1.0, 0.6);
   testR2Button->setBaseColor(0.65, 0.67, 0.81, 1.0);
   testR2Button->setBorderWidth(1.0);
   testR2Button->setText("Oranges");
   testR2Button->setTextSize(4.0);
   testR2Button->setTextColor(0.25, 0.27, 0.41, 1.0);
   testR2Button->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);

   testR3Button = new GrRadioButton();
   testR3Button->setRelativeGeometry(0.0, 0.8, 1.0, 1.0);
   testR3Button->setBaseColor(0.65, 0.67, 0.81, 1.0);
   testR3Button->setBorderWidth(1.0);
   testR3Button->setText("Pineapples");
   testR3Button->setTextSize(4.0);
   testR3Button->setTextColor(0.25, 0.27, 0.41, 1.0);
   testR3Button->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_MIDDLE);

   testRadioButtonGroup = new GmRadioButtonGroup();
   testRadioButtonGroup->setRelativeGeometry(0.05, 0.3, 1.0, 0.8);
   testRadioButtonGroup->addComponent(testR1Button, RADIO_BUTTON_ONE);
   testRadioButtonGroup->addComponent(testR2Button, RADIO_BUTTON_TWO);
   testRadioButtonGroup->addComponent(testR3Button, RADIO_BUTTON_THREE);
   testRadioButtonGroup->addSelectionListener(this);

   testButton3 = new GrPushButton();
   testButton3->setBaseColor(0.83, 0.95, 0.83, 1.0);
   testButton3->setText("T hate Chicken!");
   testButton3->setTextColor(0.25, 0.40, 0.25, 1.0);
   testButton3->setTextSize(4.0);
   testButton3->setRelativeGeometry(0.05, 0.0, 0.95, 0.3);
   testButton3->setWidgetId(BUTTON_EXPANDERPANEL_SELECT);
   testButton3->addButtonListener(this);

   testExpanderPanel = new GrExpanderPanel();
   testExpanderPanel->setRelativeGeometry(0.8, 0.6, 1.0, 0.8);
   testExpanderPanel->setExpandedGeometry(10.0, 0.6, GrTypes::EX_UP);
   testExpanderPanel->setButtonText("Push");
   testExpanderPanel->setTitleText("Expander Panel");
   testExpanderPanel->setTitlebarColor(0.92, 0.92, 1.0, 1.0);
   testExpanderPanel->setTitleTextColor(0.0, 0.0, 0.0, 1.0);

   testExpanderPanel->setTitleTextSize(4.0);
   testExpanderPanel->setWidgetId(EXPANDERPANEL);

   testExpanderPanel->addComponent(testButton3);
   //testExpanderPanel->addComponent(testRadioButtonGroup);

   testExpanderPanel->setToolTipText("Press this button to expand/collapse");

   testCheckBox = new GrCheckBox();
   testCheckBox->setRelativeGeometry(0.0, 0.3, 1.0, 0.4);
   //testCheckBox->setBaseColor(0.92, 0.92, 1.0, 1.0);
   testCheckBox->setTextColor(0.25, 0.27, 0.41, 1.0);
   testCheckBox->setText("Enable blender for food processing");
   testCheckBox->setTextSize(4.0);
   //testCheckBox->setCheckColor(0.65, 0.95, 0.65, 1.0);
   testCheckBox->setWidgetId(CHECKBOX);
   testCheckBox->addSelectionListener(this);

   testBarMeter = new GrBarMeter();
   testBarMeter->setRelativeGeometry(0.0, 0.15, 1.0, 0.25);
   testBarMeter->setRange(0,500);

   testDialog1->addComponent(testPanel1);
   testDialog1->addComponent(testExpanderPanel);
   testDialog1->addComponent(testBarMeter);
   testDialog1->addComponent(testButton1);
   testDialog1->addComponent(testCheckBox);

   Gm2dLayout *layout = new Gm2dLayout();
   layout->setInsets(.1, .1, .1, .1);
   testDialog1->setLayout(layout);
   applyDialog1Layout();
}

void RockTestUI::applyDialog1Layout()
{
   testDialog1->layoutInGLArea(-50.0, -40.0, 100.0, 80.0, -0.5);
}

void RockTestUI::setupDialog2()
{
   testDialog2 = new GrDialog();
   testDialog2->setBaseColor(0.74, 0.32, 0.32, 1.0);
   testDialog2->setRelativeGeometry(0.0, 0.0, 1.0, 1.0);
   testDialog2->setTitlebarColor(1.0, 0.93, 0.93, 1.0);
   testDialog2->setTitleText("ASTi Communication Error");
   testDialog2->setTitleTextSize(4.0);
   testDialog2->setTitleTextColor(0.4, 0.2, 0.2, 1.0);
   testDialog2->setTitleTextJustification(GmTypes::JTH_CENTER,
                                          GmTypes::JTV_TOP);

//   testScrollableListBox = new GmScrollableListBox();
//   testScrollableListBox->setRelativeGeometry(0.0, 0.45, 1.0, 0.95);
//   testScrollableListBox->setBaseColor(1.0, 0.93, 0.93, 1.0);
   //testPanel2 = new GrPanel();
   //testPanel2->setRelativeGeometry(0.0, 0.3, 1.0, 1.0);
   //testPanel2->setBaseColor(1.0, 0.93, 0.93, 1.0);

   testTextArea = new GmTextArea();
   testTextArea->setTextColor(0.35, 0.37, 0.31, 1.0);
   testTextArea->setText("Communication has been temporarily severed with the USF-237 simulator ASTi radio systems. Usually this is caused by a network error, or hardware error on the simulator system. For more information on the problem, please launch the ASTi radio network troubleshooter. If you are unable to access the ASTi radio network troubleshooter at this time, please contact customer support.");
   testTextArea->setTextJustification(GmTypes::JTH_FULL, GmTypes::JTV_TOP);
   testTextArea->setTextSize(4.0);
   testTextArea->setBaseColor(0.1, 0.93, 0.93, 1.0);

   testScrollablePane = new GrScrollablePane();
   testScrollablePane->setRelativeGeometry(0.0, 0.3, 1.0, 1.0);
   testScrollablePane->addComponent(testTextArea);

   testButton2 = new GrPushButton();
   testButton2->setBaseColor(0.8, 0.73, 0.73, 1.0);
   testButton2->setText("Dismiss");
   testButton2->setTextColor(0.4, 0.2, 0.2, 1.0);
   testButton2->setTextSize(4.0);
   testButton2->setRelativeGeometry(0.3, 0.0, 0.7, 0.2);
   testButton2->setWidgetId(BUTTON_DIALOG_TWO_DISMISS);
   testButton2->addButtonListener(this);

   //testScrollbar->setRelativeGeometry(0.95, 0.5, 1.0, 1.0);
   //testScrollbar->setBaseColor(0.4, 0.2, 0.2, 1.0);
   //testScrollbar->setCurrentValue(0.5);

  // testScrollableListBox->addComponent(testTextArea);
   //testPanel2->addComponent(testScrollablePane);
//   testDialog2->addComponent(testScrollableListBox);   
   testDialog2->addComponent(testScrollablePane);
   testDialog2->addComponent(testButton2);
   //testDialog2->addComponent(testScrollbar);

   Gm2dLayout *layout = new Gm2dLayout();
   layout->setInsets(.1, .1, .1, .1);
   testDialog2->setLayout(layout);
   applyDialog2Layout();
}

void RockTestUI::applyDialog2Layout()
{
   testDialog2->layoutInGLArea(90.0, -30.0, 100.0, 70.0, -0.7);
}

void RockTestUI::setupMenuBar()
{
   testFileMenu = new GrMenu();
   testFileMenu->addTextItem("New", MENU_FILE_NEW);
   testFileMenu->addTextItem("Open...", MENU_FILE_OPEN);
   testFileMenu->addTextItem("Close", MENU_FILE_CLOSE);
   testFileMenu->addTextItem("Print...",  MENU_FILE_PRINT);
   testFileMenu->addTextItem("Quit", MENU_FILE_QUIT);
   testFileMenu->setText("File");
   testFileMenu->setTextSize(4.0);
   
   testEditMenu = new GrMenu();
   testEditMenu->setText("Edit");
   testEditMenu->addTextItem("Cut", MENU_EDIT_CUT);
   testEditMenu->addTextItem("Copy", MENU_EDIT_COPY);
   testEditMenu->addTextItem("Paste", MENU_EDIT_PASTE);
   testEditMenu->addTextItem("Find",  MENU_EDIT_FIND);
   testEditMenu->setTextSize(4.0);

   testTestMenu = new GrMenu();
   testTestMenu->setText("Test");
   testTestMenu->addTextItem("Frame", MENU_TEST_FRAME);
   testTestMenu->setTextSize(4.0);

   testViewMenu = new GrMenu();
   testViewMenu->setText("View");
   testViewMenu->addTextItem("Zoom In", MENU_VIEW_ZOOM_IN);
   testViewMenu->addTextItem("Zoom Out", MENU_VIEW_ZOOM_OUT);
   testViewMenu->addTextItem("Center", MENU_VIEW_CENTER);
   testViewMenu->setTextSize(4.0);

   testHelpMenu = new GrMenu();
   testHelpMenu->setText("Help");
   testHelpMenu->addTextItem("About...", MENU_HELP_ABOUT);
   testHelpMenu->addTextItem("User Manual", MENU_HELP_MANUAL);
   testHelpMenu->addTextItem("Tutorials", MENU_HELP_TUTORIALS);
   testHelpMenu->setTextSize(4.0);

   testMenuBar = new GrMenuBar();
   testMenuBar->addMenu(testFileMenu, MENU_FILE);
   testMenuBar->addMenu(testEditMenu, MENU_EDIT);
   testMenuBar->addMenu(testTestMenu, MENU_TEST);
   testMenuBar->addMenu(testViewMenu, MENU_VIEW);
   testMenuBar->addMenu(testHelpMenu, MENU_HELP);
   testMenuBar->setRelativeGeometry(0.0, 0.0, 1.0, 1.0);
   testMenuBar->setWidgetId(MENUBAR);
   testMenuBar->addSelectionListener(this);

   Gm2dLayout *layout = new Gm2dLayout();
   testMenuBar->setLayout(layout);
   testMenuBar->render(renderOptions);
   applyMenuLayout();
}

void RockTestUI::applyMenuLayout()
{
   testMenuBar->layoutInGLArea(-100.0, 80.0, 70.0, 7.5, -0.4);
}

void RockTestUI::setupExpanderToolbar()
{
   planeButton = new GrPushButton();
   planeButton->setText("Plane");
   planeButton->setBaseColor(0.8, 0.73, 0.73, 1.0);
   planeButton->setTextColor(0.4, 0.2, 0.2, 1.0);
   planeButton->setTextSize(4.0);
   planeButton->setBorderWidth(0.1);

   GmFileCache* fc = &GmFileCache::getInstance();

   planeButton->setPressedIcon(
      new GmSvgIcon(GLAMROCKDATADIR "/unitPlane.svg"));
   planeButton->setUnpressedIcon(
      new GmSvgIcon(GLAMROCKDATADIR "/unitPlane.svg"));

   planeButton->setTextJustification(GmTypes::JTH_CENTER,
                                     GmTypes::JTV_BOTTOM);

   planeButton->setRelativeGeometry(0.1, 0.1, 0.9, 0.9);
   planeButton->setWidgetId(BUTTON_EXPANDERTOOLBAR_PLANE);
   planeButton->addButtonListener(this);
   
   tankButton = new GrPushButton();
   tankButton->setText("Tank");
   tankButton->setBaseColor(0.8, 0.73, 0.73, 1.0);
   tankButton->setTextColor(0.4, 0.2, 0.2, 1.0);
   tankButton->setTextSize(4.0);
   tankButton->setBorderWidth(0.1);

   fc = &GmFileCache::getInstance();

   tankButton->setPressedIcon(
      new GmSvgIcon(GLAMROCKDATADIR "/unitTank.svg"));
   tankButton->setUnpressedIcon(
      new GmSvgIcon(GLAMROCKDATADIR "/unitTank.svg"));

   tankButton->setTextJustification(GmTypes::JTH_CENTER,
                                    GmTypes::JTV_BOTTOM);

   tankButton->setRelativeGeometry(0.1, 0.1, 0.9, 0.9);
   tankButton->setWidgetId(BUTTON_EXPANDERTOOLBAR_TANK);
   tankButton->addButtonListener(this);
   
   testExpanderToolbar = new GrExpanderToolbar();
   testExpanderToolbar->setWidgetId(EXPANDERTOOLBAR);
   testExpanderToolbar->setRelativeGeometry(0.0, 0.0, 1.0, 1.0);
   testExpanderToolbar->addComponent(tankButton);
   testExpanderToolbar->addComponent(planeButton);
   
   applyExpanderToolbarLayout();
}

void RockTestUI::setRenderOptions(GmTypes::RenderOptionType options)
{
   renderOptions = options;
}

void RockTestUI::renderWidget(GmDrawable *widget)
{
   widget->render(renderOptions);
}

void RockTestUI::renderUI()
{
   renderWidget(testDialog1);
   renderWidget(testDialog2);
   renderWidget(testMenuBar);
   renderWidget(testExpanderToolbar);
   if(testFrame!=NULL)
      renderWidget(testFrame);
}

void RockTestUI::initUI()
{
   //setup the widgets
   setupDialog1();
   setupDialog2();
   setupMenuBar();
   setupExpanderToolbar();
   setupFrame();
   applyDialog1Layout();
   applyDialog2Layout();
   applyMenuLayout();
   applyExpanderToolbarLayout();
   applyFrameLayout();
}

void RockTestUI::applyExpanderToolbarLayout()
{
   testExpanderToolbar->expand();
   testExpanderToolbar->layoutInGLArea(-100.0, 50.0, 100.0, 20.0, -0.5);
   testExpanderToolbar->setWidgetId(EXPANDERTOOLBAR);
   testExpanderToolbar->contract();
}

void RockTestUI::print_usage()
{
   cout << "rockTest v0.1" << endl;
   cout << "usage: rockTest" << endl;
   cout << endl;
}

GrDialog* RockTestUI::getDialog1()
{
   return testDialog1;
}

GrBarMeter* RockTestUI::getBarMeter()
{
   return testBarMeter;
}

GrDialog* RockTestUI::getDialog2()
{
   return testDialog2;
}

GrMenuBar* RockTestUI::getMenuBar()
{
   return testMenuBar;
}

GrExpanderToolbar* RockTestUI::getExpanderToolbar()
{
   return testExpanderToolbar;
}

void RockTestUI::selectionOccurred(GmSelectionAction* action)
{
   switch(action->getSelection())
   {
      case MENU_FILE_NEW:
         cout << "Creating new file" << endl;
         break;
      case MENU_FILE_OPEN:
         cout << "Opening file" << endl;
         break;
      case MENU_FILE_CLOSE:
         cout << "Closing file" << endl;
         break;
      case MENU_FILE_PRINT:
         cout << "Printing file" << endl;
         break;
      case MENU_FILE_QUIT:
         cout << "Quitting program" << endl;
         break;
      case MENU_EDIT_CUT:
         cout << "Cutting" << endl;
         break;
      case MENU_EDIT_COPY:
         cout << "Copying" << endl;
         break;
      case MENU_EDIT_PASTE:
         cout << "Pasting" << endl;
         break;
      case MENU_EDIT_FIND:
         cout << "Finding" << endl;
         break;
      case MENU_TEST_FRAME:
         createFrame();
         break;
      case MENU_VIEW_ZOOM_IN:
         cout << "Zooming in" << endl;
         break;
      case MENU_VIEW_ZOOM_OUT:
         cout << "Zooming out" << endl;
         break;
      case MENU_VIEW_CENTER:
         cout << "Centering view" << endl;
         break;
      case RADIO_BUTTON_ONE:
         cout << "RadioButton:  Apples" << endl;
         break;
      case RADIO_BUTTON_TWO:
         cout << "RadioButton:  Oranges" << endl;
         break;
      case RADIO_BUTTON_THREE:
         cout << "RadioButton:  Pineapples" << endl;
         break;
      case CHECKBOX:
         cout << "Checkbox:    Radical!!!" << endl;
         break;
      default:
         break;
   }
}

void RockTestUI::buttonPressed(GmButtonAction* action)
{
   switch(action->getSrcWidgetId())
   {
      case BUTTON_DIALOG_ONE_OK:
         cout << "Dialog 1:  OK!" << endl;
         break;
      case BUTTON_DIALOG_TWO_DISMISS:
         cout << "Dialog 2:  Dismiss" << endl;
         break;
      case BUTTON_EXPANDERPANEL_SELECT:
         cout << "ExpanderPanel:  Select" << endl;
         break;
      case BUTTON_EXPANDERTOOLBAR_TANK:
         cout << "ExpanderToolbar:  Tank"  << endl;
         break;
      case BUTTON_EXPANDERTOOLBAR_PLANE:
         cout << "ExpanderToolbar:  Plane" << endl;
         break;
      default:
         cout << "RockTest: buttonPress - unhandled button action!" << endl;
         break;
   }
}

void RockTestUI::setupFrame()
{
   testFrame = new GrFrame();
   testFrame->setRelativeGeometry(0.0, 0.0, 1.0, 1.0);
   testFrame->setTitleText("Test Frame");
   testFrame->setVisible(false);
}

void RockTestUI::applyFrameLayout()
{
   testFrame->layoutInGLArea(-20.0, 45.0, 50, 30, -0.1);
}

void RockTestUI::createFrame()
{
   testFrame->setVisible(true);
}

