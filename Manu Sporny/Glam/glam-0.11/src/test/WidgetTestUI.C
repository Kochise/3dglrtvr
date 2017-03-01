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
#include <WidgetTestUI.h>

WidgetTestUI::WidgetTestUI()
{
   mRenderOptions = GmTypes::RO_2D;
}

WidgetTestUI::~WidgetTestUI()
{
}

void WidgetTestUI::setupTestPanel()
{
   mTestPanel = new GmPanel();
   mTestPanel->setRelativeGeometry(0.0, 0.0, 1.0, 1.0);
   mTestPanel->setBaseColor(0.85, 0.85, 0.85, 0.5);

   // Setup the menus
   // Put the menu bar together
   mTestMenuBar = new GmMenuBar();
   mTestMenuBar->setRelativeGeometry(0.0, 0.9, 1.0, 1.0);
   mTestMenuBar->setWidgetId(MENUBAR);
   mTestMenuBar->addSelectionListener(this);

   // sub sub menu setup
   mTestSubSubMenu = new GmMenu();
   mTestSubSubMenu->setText("Sub Sub");
   mTestSubSubMenu->addTextItem("stuff", MENU_SUBSUB_STUFF);
   
   // sub menu setup
   mTestSubMenu = new GmMenu();
   mTestSubMenu->setText("Save...");
   mTestSubMenu->addSubMenu(mTestSubSubMenu, MENU_SUBSUB);
   mTestSubMenu->addTextItem("Save", MENU_SUB_SAVE);
   mTestSubMenu->addTextItem("Save as...", MENU_SUB_SAVEAS);

   // menus setup
   mTestFileMenu = new GmMenu();
   mTestFileMenu->setText("File");
   mTestFileMenu->setFocusOrder(1);
   mTestFileMenu->addTextItem("New", MENU_FILE_NEW);
   mTestFileMenu->addTextItem("Open...", MENU_FILE_OPEN);
   mTestFileMenu->addSubMenu(mTestSubMenu, MENU_SUB);
   mTestFileMenu->addTextItem("Close", MENU_FILE_CLOSE);
   mTestFileMenu->addTextItem("Print...",  MENU_FILE_PRINT);
   mTestFileMenu->addTextItem("Quit", MENU_FILE_QUIT);

   mTestEditMenu = new GmMenu();
   mTestEditMenu->setText("Edit");
   mTestEditMenu->setFocusOrder(2);
   mTestEditMenu->addTextItem("Cut", MENU_EDIT_CUT);
   mTestEditMenu->addTextItem("Copy", MENU_EDIT_COPY);
   mTestEditMenu->addTextItem("Paste", MENU_EDIT_PASTE);
   mTestEditMenu->addTextItem("Find", MENU_EDIT_FIND);

   // add the menus to the menubar
   mTestMenuBar->addMenu(mTestFileMenu, MENU_FILE);
   mTestMenuBar->addMenu(mTestEditMenu, MENU_EDIT);

   // Setup the push button
   mTestButton = new GmPushButton();
   mTestButton->setRelativeGeometry(0.1, 0.4, 0.9, 0.6);
   mTestButton->setText("Hello World!");
   mTestButton->setWidgetId(HELLO_WORLD_BUTTON);
   mTestButton->addButtonListener(this);
   
   // Setup the label
   mTestLabel = new GmLabel();
   mTestLabel->setRelativeGeometry(0.2, 0.85, 0.9, 0.9);
   mTestLabel->setText("Test Label");
   mTestLabel->setTextColor(0.0, 1.0, 1.0, 1.0);

   // Setup the check box
   mTestCheckBox = new GmCheckBox();   
   mTestCheckBox->setRelativeGeometry(0.55, 0.25, 0.60, 0.30);
   mTestCheckBox->setBaseColor(0.1, 0.1, 0.1, 0.9);
   mTestCheckBox->setBorderWidth(0.1);
   mTestCheckBox->setText("Checkbox!");
   mTestCheckBox->setTextColor(0.0, 1.0, 0.0, 1.0);
   mTestCheckBox->setWidgetId(CHECKBOX_TEST);
   mTestCheckBox->addSelectionListener(this);

   // Setup the pixmap button
   // Setup the icons
   GmFileCache* fc = &GmFileCache::getInstance();

   // Setup the pixmap icon
   mTestPixmapIcon = (GmPixmapIcon*)fc->fetchIcon(
      GLAM_DATA_DIR "/iconNGNavalShipBlue.png");

   mTestPixmapIcon->setRelativeGeometry(0.0, 0.8, 0.1, 0.9);

   // Setup the svg icon
//    mTestSvgIcon = (GmSvgIcon*)fc->fetchIcon(
//       GLAM_DATA_DIR "/testSvgIcon.svg");
//    mTestSvgIcon->setRelativeGeometry(0.0, 0.5, 0.1, 0.6);

   // Setup the graphic button
   mTestGraphicButton = new GmPushButton();
   
   GmIcon* icon;

   icon = fc->fetchIcon(GLAM_DATA_DIR "/iconNGGroundBlue.png");
   mTestGraphicButton->setPressedIcon(icon);

   icon = fc->fetchIcon(GLAM_DATA_DIR "/iconNGAirBlue.png");
   mTestGraphicButton->setUnpressedIcon(icon);
   
   mTestGraphicButton->setRelativeGeometry(0.0, 0.6, 0.2, 0.8);
   mTestGraphicButton->setText("test");
   mTestGraphicButton->setTextColor(1.0, 1.0, 0.0, 1.0);
   mTestGraphicButton->setTextJustification(GmTypes::JTH_CENTER,
                                            GmTypes::JTV_BOTTOM);
   mTestGraphicButton->setToolTipText("Tooltip for AirCraft/Tank button");

   // Setup the radio buttons
   mTestRadioButton1 = new GmRadioButton();
   mTestRadioButton1->setRelativeGeometry(0.0, 0.8, 1.0, 1.0);
   mTestRadioButton1->setBaseColor(0.0, 0.0, 0.0, 0.9);
   mTestRadioButton1->setBorderWidth(0.1);
   mTestRadioButton1->setText("Radio Option 1");
   mTestRadioButton1->setTextColor(0.0, 0.0, 0.3, 1.0);

   mTestRadioButton2 = new GmRadioButton();     
   mTestRadioButton2->setRelativeGeometry(0.0, 0.4, 1.0, 0.6);
   mTestRadioButton2->setBaseColor(0.0, 0.0, 0.0, 0.9);
   mTestRadioButton2->setBorderWidth(0.1);
   mTestRadioButton2->setText("Radio Option 2");
   mTestRadioButton2->setTextColor(0.0, 0.0, 0.6, 1.0);

   mTestRadioButton3 = new GmRadioButton();
   mTestRadioButton3->setRelativeGeometry(0.0, 0.0, 1.0, 0.2);
   mTestRadioButton3->setBaseColor(0.0, 0.0, 0.0, 0.9);
   mTestRadioButton3->setBorderWidth(0.1);
   mTestRadioButton3->setText("Radio Option 3");
   mTestRadioButton3->setTextColor(0.0, 0.0, 0.9, 1.0);

   // Setup the radio group
   mTestRadioButtonGroup = new GmRadioButtonGroup();
   mTestRadioButtonGroup->setRelativeGeometry(0.45, 0.65, 0.9, 0.8);
   mTestRadioButtonGroup->setBaseColor(0.0, 0.0, 0.0, 0.0);
   mTestRadioButtonGroup->setWidgetId(RB_GROUP);
   mTestRadioButtonGroup->addSelectionListener(this);
   
   // Add the radio buttons to the group
   mTestRadioButtonGroup->addComponent(mTestRadioButton1, RB_ONE);
   mTestRadioButtonGroup->addComponent(mTestRadioButton2, RB_TWO);
   mTestRadioButtonGroup->addComponent(mTestRadioButton3, RB_THREE);

   // Setup the hyperlink
   mTestHyperlink = new GmHyperlink();
   mTestHyperlink->setRelativeGeometry(0.1, 0.25, 0.5, 0.35);
   mTestHyperlink->setText("Hyperlink");
   mTestHyperlink->setTextJustification(GmTypes::JTH_LEFT,
                                        GmTypes::JTV_BOTTOM);
   mTestHyperlink->setTextSize(8.0);
   mTestHyperlink->setWidgetId(HYPERLINK_TEST);
   mTestHyperlink->addHyperlinkListener(this);

   // Setup a test bar meter
   mTestBarMeter = new GmBarMeter();
   mTestBarMeter->setRelativeGeometry(0.1, 0.33, 0.9, 0.4);
   mTestBarMeter->setRange(0, 500);

   // Setup a test scrollbar
   mTestScrollbar = new GmScrollbar();
   mTestScrollbar->setOrientation(GmTypes::OT_VERTICAL);
   mTestScrollbar->setRange(0.0, 100.0);
   mTestScrollbar->setPageStep(10.0);
   mTestScrollbar->setRelativeGeometry(0.95, 0.2, 1.0, 0.9);
   //    mTestScrollbar->setCurrentValue(0.5);

   // Setup a test icon bar
   mTestIconBar = new GmIconBar();
   mTestIconBar->addButton("B1");
   mTestIconBar->addButton("B2");
   mTestIconBar->addButton("B3");
   mTestIconBar->addButton("B4");
   mTestIconBar->addButton("B5");
   mTestIconBar->addButton("B6");
   mTestIconBar->addButton("B7");
   mTestIconBar->addButton("B8");
   mTestIconBar->addButton("B9");
   mTestIconBar->addButton("C1");
   mTestIconBar->addButton("C2");
   mTestIconBar->addButton("C3");
   mTestIconBar->addButton("C4");
   mTestIconBar->addButton("C5");
   mTestIconBar->addButton("C6");
   mTestIconBar->addButton("C7");
   mTestIconBar->addButton("C8");
   mTestIconBar->setRelativeGeometry(0.0, 0.0, 1.0, 0.2);
   
   // add components to the test panel
   mTestPanel->addComponent(mTestMenuBar);
   mTestPanel->addComponent(mTestGraphicButton);
   mTestPanel->addComponent(mTestRadioButtonGroup);
   mTestPanel->addComponent(mTestIconBar);
   mTestPanel->addComponent(mTestCheckBox);
   mTestPanel->addComponent(mTestBarMeter);
   mTestPanel->addComponent(mTestPixmapIcon);
   mTestPanel->addComponent(mTestHyperlink);
   mTestPanel->addComponent(mTestScrollbar);
   mTestPanel->addComponent(mTestButton);
   mTestPanel->addComponent(mTestLabel);
   //mTestPanel->addComponent(mTestSvgIcon);
}

void WidgetTestUI::setupTestPanel2()
{
   // setup test panel 2
   mTestPanel2 = new GmPanel();
   mTestPanel2->setRelativeGeometry(0.0, 0.0, 1.0, 1.0);
   mTestPanel2->setBaseColor(0.85, 0.85, 0.85, 0.5);

   // Setup the text area
   mTestTextArea = new GmTextArea();
   mTestTextArea->setRelativeGeometry(0.0, 0.5, 1.0, 1.0);
   mTestTextArea->setTextColor(1.0, 0.0, 0.0, 1.0);
   //mTestTextArea->setText("This is a glam text area that can justify
   //itself horizontally and vertically.");

   string s1 = "   Hello there, this is a really long text ";
   string s2 = "area message that is being used for testing ";
   string s3 = "purposes. Here is a ";
   string s4 = "supercalifragilisticexpialidociouswords";

   mTestTextArea->setText(s1 + s2 + s3 + s4);
   mTestTextArea->setTextJustification(GmTypes::JTH_FULL,
                                       GmTypes::JTV_TOP);

   // Setup the combo box
   mComboBox = new GmComboBox();
   mComboBox->setText("???");
   mComboBox->addTextItem("Apples", COMBOBOX_APPLES);
   mComboBox->addTextItem("Oranges", COMBOBOX_ORANGES);
   mComboBox->addTextItem("Kiwis", COMBOBOX_KIWIS);
   mComboBox->addSelectionListener(this);

   // Setup an expandable list item
   GmFlowLayout* flowLayout = new GmFlowLayout();
   GmLabel* elLabel = new GmLabel();
   GmPanel* elPanel = new GmPanel();
   GmLabel* elText1 = new GmLabel();
   //GmLabel* elText2 = new GmLabel();
   
   elLabel->setText("- What do you want for Lunch?");
   elLabel->setTextJustification(GmTypes::JTH_LEFT,
                                 GmTypes::JTV_MIDDLE);
   elText1->setText("I want");
   //elText2->setText("for lunch.");
   flowLayout->setCellInsets(0.1, 0.1, 0.1, 0.1);
   elPanel->setLayout(flowLayout);
   elPanel->addComponent(elText1);
   elPanel->addComponent(mComboBox);
   //elPanel->addComponent(elText2);
   
   elLabel->setRelativeGeometry(0.0, 0.0, 1.0, 1.0);
   mExpandableListItem = new GmExpandableListItem(elLabel, elPanel);
   mExpandableListItem->setRelativeGeometry(0.05, 0.4, 0.95, 0.6);

   // Setup the test text input field
   mTextField = new GmTextField();
   mTextField->setText("type here");
   mTextField->setRelativeGeometry(0.1, 0.31, 0.4, 0.39);

   // Setup the drag text
   mDragText = new DragText();
   mDragText->setText("Drag Me!");
   mDragText->setRelativeGeometry(0.5, 0.31, 0.8, 0.39);

   // Setup a test list box
   mTestListBox = new GmScrollablePane();
   GmListLayout* ll = new GmListLayout();
   ll->setCellInsets(0.0, 0.0, 0.1, 0.0);
   mTestListBox->setLayout(ll);
   mTestListBox->setRelativeGeometry(0.1, 0.1, 0.9, 0.3);
   mTestListBox->setBaseColor(0.5, 0.5, 0.5, 1.0);

   GmTextWidget* listBoxText4 = new GmTextWidget();
   GmTextWidget* listBoxText5 = new GmTextWidget();
   GmTextWidget* listBoxText6 = new GmTextWidget();

   GmTextWidget* listBoxText7 = new GmTextWidget();
   GmTextWidget* listBoxText8 = new GmTextWidget();
   GmTextWidget* listBoxText9 = new GmTextWidget();

   GmTextWidget* listBoxText10 = new GmTextWidget();
   GmTextWidget* listBoxText11 = new GmTextWidget();
   GmTextWidget* listBoxText12 = new GmTextWidget();

   GmTextWidget* listBoxText13 = new GmTextWidget();
   GmTextWidget* listBoxText14 = new GmTextWidget();
   GmTextWidget* listBoxText15 = new GmTextWidget();

   listBoxText4->setText("I hate chicken");
   listBoxText5->setText("I hate liver");
   listBoxText6->setText("Meow Mix, Meow Mix, makes me quiver");

   listBoxText7->setText("This");
   listBoxText8->setText("is");
   listBoxText9->setText("giant LETTER");

   listBoxText10->setText("Yes");
   listBoxText11->setText("it");
   listBoxText12->setText("is");

   listBoxText13->setText("Buggalicious");
   listBoxText14->setText("Bugderbar");
   listBoxText15->setText("BJ Baracus");
   
   mTestListBox->addComponent(listBoxText4);
   mTestListBox->addComponent(listBoxText5);
   mTestListBox->addComponent(listBoxText6);
   mTestListBox->addComponent(listBoxText7);
   mTestListBox->addComponent(listBoxText8);
   mTestListBox->addComponent(listBoxText9);
   mTestListBox->addComponent(listBoxText10);
   mTestListBox->addComponent(listBoxText11);
   mTestListBox->addComponent(listBoxText12);
   mTestListBox->addComponent(listBoxText13);
   mTestListBox->addComponent(listBoxText14);
   mTestListBox->addComponent(listBoxText15);
   
   // add components to test panel 2
   mTestPanel2->addComponent(mTestTextArea);
   mTestPanel2->addComponent(mTestListBox);
   mTestPanel2->addComponent(mTextField);
   mTestPanel2->addComponent(mDragText);
   mTestPanel2->addComponent(mExpandableListItem);
}

void WidgetTestUI::setupTestPanel3()
{
   //this is the Table and ScrollablePane test panel
   mTestPanel3 = new GmPanel();
   mTestPanel3->setRelativeGeometry(0.0, 0.0, 1.0, 1.0);
   mTestPanel3->setBaseColor(0.85, 0.85, 0.85, 0.5);

   //set up the pane for the table and text
   mTablePane = new GmScrollablePane();
   mTablePane->setRelativeGeometry(0.05, 0.05, 0.95, 0.95);
   mTablePane->setBaseColor(0.6, 0.6, 0.6, 1.0);

   //initialize the table
   mTestTable = new GmTable();
   mTestTable->setWidgetId(TABLE);
   mTestTable->setTableBorderSize(2.0);
   mTestTable->setTableColor(GlmColor4(0.5, 0.5, 0.5, 1.0));
   mTestTable->setTableBorderColor(GlmColor4(0.3, 0.3, 0.3, 1.0));

   //set up all the labels to go into the table
   mTableLabel1 = new GmLabel();
   mTableLabel1->setText("Drag my cell's");
   mTableLabel1->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_BOTTOM);
   
   mTableLabel2 = new GmLabel();
   mTableLabel2->setText("lower right");
   mTableLabel2->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_BOTTOM);
   
   mTableLabel3 = new GmLabel();
   mTableLabel3->setText("border corners");
   mTableLabel3->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_BOTTOM);
   
   mTableLabel4 = new GmLabel();
   mTableLabel4->setText("to resize them!");
   mTableLabel4->setTextJustification(GmTypes::JTH_LEFT, GmTypes::JTV_BOTTOM);

   //set up the button for the table
   mTableButton = new GmPushButton();
   //mTableButton->setRelativeGeometry(0.05, 0.05, 0.95, 0.95);
   mTableButton->setText("Hello World!");
   mTableButton->setWidgetId(HELLO_WORLD_BUTTON);
   mTableButton->addButtonListener(this);

   //these following calls create the table
   //note: the table begins with no cells
   
   //1, 1 adds a cell at 1st row 1st column
   mTestTable->addCell(mTableLabel1, 1, 1);
   //1, 2, adds a cell at 1st row 2nd column
   mTestTable->addCell(mTableLabel2, 1, 2);
   //2, 2, adds all cells needed to have a 2nd row, 2nd column
   mTestTable->addCell(mTableLabel3, 2, 2);
   //4, 4, adds all cells needed to have a 4th row, 4th column
   mTestTable->addCell(mTableLabel4, 4, 4);

   //puts the button in the table at cell 2(row), 3(col)
   mTestTable->addCellContent(mTableButton, 2, 3);

   //manually set the sizes of some of the rows or columns
   mTestTable->setRowHeight(10.0, 2);
   mTestTable->setRowHeight(5.0, 3);
   mTestTable->setColWidth(40.0, 3);

   //set some fancy cell colors!
   mTestTable->setRowColor(GlmColor4(0.0, 0.7, 0.0, 1.0), 2);
   mTestTable->setColColor(GlmColor4(0.0, 0.0, 0.7, 1.0), 3);
   mTestTable->setCellColor(GlmColor4(0.0, 0.0, 0.0, 1.0), 4, 4);

   //you can turn off certain cells from rendering if you want!
   mTestTable->setRowVisibility(false, 3);
   mTestTable->setCellVisibility(true, 3, 2);
   mTestTable->setColVisibility(false, 4);
   mTestTable->setCellVisibility(true, 4, 4);

   //you can also disable their ability to be resized!
   //mTestTable->setRowResizability(false, 2);
   //mTestTable->setColResizability(false, 3);

   //add some textwidgets to the scrollable pane too
   GmTextWidget* listBoxText1 = new GmTextWidget();
   GmTextWidget* listBoxText2 = new GmTextWidget();
   GmTextWidget* listBoxText3 = new GmTextWidget();

   listBoxText1->setText("I love chicken");
   listBoxText2->setText("I love liver");
   listBoxText3->setText("Meow Mix, Meow Mix, please deliver");
   
   //add all components into the pane
   mTablePane->addComponent(listBoxText1);
   mTablePane->addComponent(listBoxText2);
   mTablePane->addComponent(listBoxText3);
   mTablePane->addComponent(mTestTable);
   
   mTestPanel3->addComponent(mTablePane);
}

void WidgetTestUI::setRenderOptions(GmTypes::RenderOptionType options)
{
   mRenderOptions = options;
}

void WidgetTestUI::renderUI()
{
   mTestPanel->render(mRenderOptions);
   mTestPanel2->render(mRenderOptions);
   mTestPanel3->render(mRenderOptions);
} 

void WidgetTestUI::applyTestPanelLayout()
{
   mTestPanel->layoutInGLArea(0.0, 0.0, 90.0, 100.0, -0.5);
   mTestPanel2->layoutInGLArea(-90.0, 0.0, 90.0, 100.0, -0.5);
   mTestPanel3->layoutInGLArea(0.0, -100.0, 90.0, 100.0, -0.5);
}

void WidgetTestUI::initUI()
{
   // Setup the widgets
   setupTestPanel();
   setupTestPanel2();
   setupTestPanel3();

   // panels have been set up
   // apply the test panel layouts
   applyTestPanelLayout();
}

void WidgetTestUI::updateBarMeter(float value)
{
   mTestBarMeter->setCurrentValue(value);
}

void WidgetTestUI::updateLabel(char* text)
{
   mTestLabel->setText(text);
}

void WidgetTestUI::print_usage()
{
   cout << "Glam Widget Test v0.1" << endl;
   cout << "usage: uiWidgetTest" << endl;
   cout << endl;
}

/* Implementation for Listeners */

void WidgetTestUI::selectionOccurred(GmSelectionAction* action)
{
   switch(action->getSelection())
   {
      case MENU_FILE:
         cout << "File menu selected" << endl;
         break;
      case MENU_FILE_NEW:
         cout << "New file" << endl;
         break;
      case MENU_FILE_OPEN:
         cout << "Open file" << endl;
         break;
      case MENU_FILE_CLOSE:
         cout << "Close file" << endl;
         break;
      case MENU_FILE_PRINT:
         cout << "Print file" << endl;
         break;
      case MENU_FILE_QUIT:
         cout << "Quitting..." << endl;
         exit(0);
         break;
      case MENU_EDIT:
         cout << "Edit menu selected" << endl;
         break;
      case MENU_EDIT_CUT:
         cout << "Cutting text..." << endl;
         break;
      case MENU_EDIT_COPY:
         cout << "Copying text..." << endl;
         break;
      case MENU_EDIT_PASTE:
         cout << "Pasting text..." << endl;
         break;
      case MENU_EDIT_FIND:
         cout << "Finding text..." << endl;
         break;
      case MENU_SUB:
         cout << "Sub Menu selected" << endl;
         break;
      case MENU_SUBSUB:
         cout << "Sub sub menu selected" << endl;
         break;
      case MENU_SUBSUB_STUFF:
         cout << "You selected stuff!" << endl;
         break;
      case MENU_SUB_SAVE:
         cout << "Saving file.." << endl;
         break;
      case MENU_SUB_SAVEAS:
         cout << "Saving file as..." << endl;
         break;
      case RB_ONE:
         cout << "Radio button 1" << endl;
         break;
      case RB_TWO:
         cout << "Radio button 2" << endl;
         break;
      case RB_THREE:
         cout << "Radio button 3" << endl;
         break;
      case CHECKBOX_TEST:
         cout << "Check!" << endl;
         break;
      case COMBOBOX_APPLES:
         cout << "Combo Box: Apples selected" << endl;
         break;
      case COMBOBOX_ORANGES:
         cout << "Combo Box: Oranges selected" << endl;
         break;
      case COMBOBOX_KIWIS:
         cout << "Combo Box: Kiwis selected" << endl;
         break;
      default:
         break;
   }
}

void WidgetTestUI::buttonPressed(GmButtonAction* action)
{
   switch(action->getSrcWidgetId())
   {
      case HELLO_WORLD_BUTTON:
         cout << "Hello World!" << endl;
         mTestScrollbar->setRange(0.0,
                                  mTestScrollbar->getMaximumValue() + 50.0);
         mTestScrollbar->setPageStep(mTestScrollbar->getPageStep() + 5);
         mTestScrollbar->setLineStep(mTestScrollbar->getPageStep() / 10.0);
         cout << "Test Scrollbar range and page step changed." << endl;
         break;
      default:
         cout << "WidgetTest: mousePress - unhandled button action!" << endl;
         break;
   }
}

void WidgetTestUI::hyperlinkPressed(GmHyperlinkAction* action)
{
   switch(action->getSrcWidgetId())
   {
      case HYPERLINK_TEST:
         cout << "Hyperlink!" << endl;
         break;
      default:
         break;
   }
}
