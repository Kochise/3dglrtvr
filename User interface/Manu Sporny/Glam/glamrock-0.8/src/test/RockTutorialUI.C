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
#include <GL/glut.h>

#include <glam/glam.h>
#include <glamrock/glamrock.h>
#include <RockTutorialUI.h>

using namespace std;

RockTutorialUI::RockTutorialUI()
{
   renderOptions = GmTypes::RO_2D;

   mGridDialog = NULL;
   mAboutDialog = NULL;
}

RockTutorialUI::~RockTutorialUI()
{
}

void RockTutorialUI::setupMainMenu()
{
   mFileNewFileMenu = new GrMenu();
   mFileNewFileMenu->setText("File...");
   mFileNewFileMenu->addTextItem("Document", MENU_FILE_NEW_FILE_DOCUMENT);

   mFileNewMenu = new GrMenu();
   mFileNewMenu->setText("New...");
   mFileNewMenu->addSubMenu(mFileNewFileMenu, MENU_FILE_NEW_FILE);

   mFileMenu = new GrMenu();
   mFileMenu->setText("File");
   mFileMenu->addSubMenu(mFileNewMenu, MENU_FILE_NEW);

   mFileMenu->addTextItem("Open...", MENU_FILE_OPEN);
   mFileMenu->addTextItem("Close", MENU_FILE_CLOSE);
   mFileMenu->addTextItem("Print...",  MENU_FILE_PRINT);
   mFileMenu->addTextItem("Quit", MENU_FILE_QUIT);
   
   mEditMenu = new GrMenu();
   mEditMenu->setText("Edit");
   mEditMenu->addTextItem("Cut", MENU_EDIT_CUT);
   mEditMenu->addTextItem("Copy", MENU_EDIT_COPY);
   mEditMenu->addTextItem("Paste", MENU_EDIT_PASTE);
   mEditMenu->addTextItem("Find",  MENU_EDIT_FIND);
   
   mViewMenu = new GrMenu();
   mViewMenu->setText("View");
   mViewMenu->addTextItem("Zoom In", MENU_VIEW_ZOOM_IN);
   mViewMenu->addTextItem("Zoom Out", MENU_VIEW_ZOOM_OUT);
   mViewMenu->addTextItem("Center", MENU_VIEW_CENTER);

   mLayoutMenu = new GrMenu();
   mLayoutMenu->setText("Layout");
   mLayoutMenu->addTextItem("Grid Layout", MENU_LAYOUT_GRID);
   mLayoutMenu->addTextItem("Flow Layout", MENU_LAYOUT_FLOW);
   
   mHelpMenu = new GrMenu();
   mHelpMenu->setText("Help");
   mHelpMenu->addTextItem("About...", MENU_HELP_ABOUT);
   
   mMainMenuBar = new GrMenuBar();
   mMainMenuBar->setRelativeGeometry(0.0, 0.8, 1.0, 0.9);
   
   mMainMenuBar->addMenu(mFileMenu, MENU_FILE);
   mMainMenuBar->addMenu(mEditMenu, MENU_EDIT);
   mMainMenuBar->addMenu(mViewMenu, MENU_VIEW);
   mMainMenuBar->addMenu(mLayoutMenu, MENU_LAYOUT);
   mMainMenuBar->addMenu(mHelpMenu, MENU_HELP);
   
   // set the menubar's widget id and listen to it
   mMainMenuBar->setWidgetId(MENUBAR);
   mMainMenuBar->addSelectionListener(this);
}

void RockTutorialUI::setupMainDialog()
{
   mMainDialog = new GrDialog();
   mMainDialog->setTitleText("Hello, glamrock!");
   mMainDialog->setRelativeGeometry(0.0, 0.0, 1.0, 1.0);
   
   mMainTextArea = new GmTextArea();
   mMainTextArea->setRelativeGeometry(0.0, 0.7, 1.0, 0.75);
   mMainTextArea->setText("Hello again, glamrock!");
   mMainTextArea->setTextColor(0.35, 0.37, 0.31, 1.0);

   mMainTextArea->setTextJustification(GmTypes::JTH_CENTER,
                                       GmTypes::JTV_MIDDLE);

   mComboBox = new GrComboBox();
   mComboBox->setRelativeGeometry(0.3, 0.5,
                                  0.7, 0.6);
   mComboBox->addTextItem("Mike Johnson", MJ_TEXT_ITEM);
   mComboBox->addTextItem("Tim Fuller", TF_TEXT_ITEM);
   mComboBox->addTextItem("Dave Longley", DL_TEXT_ITEM);
   mComboBox->setText("???");

   mRadioButton = new GmRadioButton();
   mRadioButton->setRelativeGeometry(0.3, 0.35, 0.7, 0.4);
   mRadioButton->setBaseColor(0.0, 0.0, 0.0, 0.9);
   mRadioButton->setBorderWidth(0.1);
   mRadioButton->setText("Radio Test");
   mRadioButton->setTextColor(0.0, 0.0, 0.3, 1.0);

   mHorzPanel = new GrExpanderPanel();
   mHorzPanel->setRelativeGeometry(0.6, 0.5, 0.8, 0.6);
   mHorzPanel->setExpandedGeometry(2.0, 0.0, GrTypes::EX_RIGHT);
   mHorzPanel->setButtonText("Horz");
   mHorzPanel->setTitleText("Horizontal");
   mHorzPanel->setTitlebarColor(1.0, 0.0, 0.0, 1.0);
   mHorzPanel->setTitleTextColor(1.0, 1.0, 1.0, 1.0);
   mHorzPanel->setTitleTextSize(3.0);

   mVertPanel = new GrExpanderPanel();
   mVertPanel->setRelativeGeometry(0.6, 0.3, 0.8, 0.4);
   mVertPanel->setExpandedGeometry(2.0, 0.0, GrTypes::EX_UP);
   mVertPanel->setButtonText("Vert");
   mVertPanel->setTitleText("Vertical");
   mVertPanel->setTitlebarColor(0.0, 0.0, 1.0, 1.0);
   mVertPanel->setTitleTextColor(1.0, 1.0, 1.0, 1.0);
   mVertPanel->setTitleTextSize(3.0);

   GmLabel* testLabel = new GmLabel();
   testLabel->setText("Test Horizontal");

   mHorzPanel->addComponent(testLabel);

   testLabel = new GmLabel();
   testLabel->setText("Test Vertical");

   mVertPanel->addComponent(testLabel);

   mMainDialog->addComponent(mHorzPanel);
   mMainDialog->addComponent(mVertPanel);

   GmFlowLayout* flowLayout = new GmFlowLayout();
   mELBody = new GmContentPane();
   mELTitle = new GmLabel();
   mELLabel1 = new GmLabel();
   mELLabel2 = new GmLabel();
   mELTitle->setText("Presidente?");
   mELLabel1->setText("I choose");
   mELLabel2->setText("for presidente.");
   
   flowLayout->setCellInsets(0.1, 0.1, 0.1, 0.1);
   mELBody->setLayout(flowLayout);
   mELBody->addComponent(mELLabel1);
   mELBody->addComponent(mComboBox);
   mELBody->addComponent(mELLabel2);
   
   mELItem = new GrExpandableListItem(mELTitle, mELBody);
   mELItem->setRelativeGeometry(0.0, 0.55, 1.0, 0.7);

   mMainDialog->addComponent(mMainMenuBar);
   mMainDialog->addComponent(mMainTextArea);
   mMainDialog->addComponent(mELItem);
   mMainDialog->addComponent(mRadioButton);

   Gm2dLayout* layout = new Gm2dLayout();
   layout->setInsets(0.1, 0.1, 0.0, 0.0);

   mMainDialog->setLayout(layout);
   applyMainDialogLayout();
}

void RockTutorialUI::createAboutDialog()
{
   mAboutDialog = new GrDialog();
   mAboutDialog->setRelativeGeometry(0.3, 0.3,
                                     0.7, 0.7);
   
   mAboutDialog->setTitleText("rockTutorial v0.1");

   mAboutOkButton = new GrPushButton();
   mAboutOkButton->setRelativeGeometry(0.3, 0.25, 0.7, 0.5);
   mAboutOkButton->setText("OK");
   mAboutOkButton->setTextColor(0.0, 0.0, 0.0, 1.0);
   //mAboutOkButton->setTextSize(5.0);

   mCheckBox = new GmCheckBox();
   mCheckBox->setRelativeGeometry(0.75, 0.25, 0.90, 0.5);
   mCheckBox->setBaseColor(0.1, 0.1, 0.1, 0.9);

   // set button's id and listen to it
   mAboutOkButton->setWidgetId(ABOUT_DIALOG_OK_BUTTON);
   mAboutOkButton->addButtonListener(this);

   mAboutDialog->addComponent(mAboutOkButton);
   mAboutDialog->addComponent(mCheckBox);

   Gm2dLayout* layout = new Gm2dLayout();
   
   mAboutDialog->setLayout(layout);
   applyAboutDialogLayout();
}

void RockTutorialUI::removeAboutDialog()
{
   // clean up dialog box
   delete mAboutDialog;
   mAboutDialog = NULL;
}

void RockTutorialUI::createGridDialog()
{
   mGridDialog = new GrDialog();
   mGridDialog->setRelativeGeometry(0.15, 0.15,
                                    0.85, 0.85);
   
   mGridDialog->setTitleText("Grid Layout");

   mGridButton1 = new GrPushButton();
   mGridButton1->setText("X");
   mGridButton1->setTextSize(5.0);
   mGridButton1->setWidgetId(GRID_DIALOG_BUTTON1);
   mGridButton1->addButtonListener(this);
   
   mGridButton2 = new GrPushButton();
   mGridButton2->setText("R+");
   mGridButton2->setTextSize(5.0);
   mGridButton2->setWidgetId(GRID_DIALOG_BUTTON2);
   mGridButton2->addButtonListener(this);

   mGridButton3 = new GrPushButton();
   mGridButton3->setText("R-");
   mGridButton3->setTextSize(5.0);
   mGridButton3->setWidgetId(GRID_DIALOG_BUTTON3);
   mGridButton3->addButtonListener(this);

   mGridButton4 = new GrPushButton();
   mGridButton4->setText("C+");
   mGridButton4->setTextSize(5.0);
   mGridButton4->setWidgetId(GRID_DIALOG_BUTTON4);
   mGridButton4->addButtonListener(this);

   mGridButton5 = new GrPushButton();
   mGridButton5->setText("C-");
   mGridButton5->setTextSize(5.0);
   mGridButton5->setWidgetId(GRID_DIALOG_BUTTON5);
   mGridButton5->addButtonListener(this);

   mGridButton6 = new GrPushButton();
   mGridButton6->setText("L+");
   mGridButton6->setTextSize(5.0);
   mGridButton6->setWidgetId(GRID_DIALOG_BUTTON6);
   mGridButton6->addButtonListener(this);

   mGridButton7 = new GrPushButton();
   mGridButton7->setText("R+");
   mGridButton7->setTextSize(5.0);
   mGridButton7->setWidgetId(GRID_DIALOG_BUTTON7);
   mGridButton7->addButtonListener(this);

   mGridButton8 = new GrPushButton();
   mGridButton8->setText("T+");
   mGridButton8->setTextSize(5.0);
   mGridButton8->setWidgetId(GRID_DIALOG_BUTTON8);
   mGridButton8->addButtonListener(this);

   mGridButton9 = new GrPushButton();
   mGridButton9->setText("B+");
   mGridButton9->setTextSize(5.0);
   mGridButton9->setWidgetId(GRID_DIALOG_BUTTON9);
   mGridButton9->addButtonListener(this);

   mGridDialog->insertComponent(mGridButton1, 0);
   mGridDialog->insertComponent(mGridButton2, 1);
   mGridDialog->insertComponent(mGridButton3, 2);
   mGridDialog->insertComponent(mGridButton4, 3);
   mGridDialog->insertComponent(mGridButton5, 4);
   mGridDialog->insertComponent(mGridButton6, 5);
   mGridDialog->insertComponent(mGridButton7, 6);
   mGridDialog->insertComponent(mGridButton8, 7);
   mGridDialog->insertComponent(mGridButton9, 8);

   GmGridLayout* layout = new GmGridLayout(3, 3);
   layout->setInsets(0.1, 0.1, 0.1, 0.1);
   
   mGridDialog->setLayout(layout);
   applyGridDialogLayout();
}

void RockTutorialUI::removeGridDialog()
{
   // clean up dialog box
   delete mGridDialog;
   mGridDialog = NULL;
}

void RockTutorialUI::createFlowDialog()
{
   mFlowDialog = new GrDialog();
   mFlowDialog->setRelativeGeometry(0.15, 0.15,
                                    0.85, 0.85);
   
   mFlowDialog->setTitleText("Flow Layout");

   mFlowCloseButton = new GrPushButton();
   mFlowCloseButton->setText("X");
   mFlowCloseButton->setTextSize(5.0);
   mFlowCloseButton->setWidgetId(FLOW_DIALOG_CLOSE_BUTTON);
   mFlowCloseButton->addButtonListener(this);
   mFlowCloseButton->setRelativeGeometry(0.15, 0.15, 0.85, 0.85);

   mLabel1 = new GmLabel();
   mLabel1->setText("Short");

   mLabel2 = new GmLabel();
   mLabel2->setText("This one is longer");

   mLabel3 = new GmLabel();
   mLabel3->setText("Wow, this label is very, very, very long");

   mLabel4 = new GmLabel();
   mLabel4->setText("This label is medium");

   mLabel5 = new GmLabel();
   mLabel5->setText("Is that a goat?");
   
   mFlowDialog->addComponent(mLabel1);
   mFlowDialog->addComponent(mLabel2);
   mFlowDialog->addComponent(mLabel3);
   mFlowDialog->addComponent(mLabel4);
   mFlowDialog->addComponent(mLabel5);
   mFlowDialog->addComponent(mFlowCloseButton);

   GmListLayout* layout = new GmListLayout();
   layout->setInsets(0.0, 0.0, 0.0, 0.0);
   
   mFlowDialog->setLayout(layout);
   applyFlowDialogLayout();
}

void RockTutorialUI::removeFlowDialog()
{
   delete mFlowDialog;
   mFlowDialog = NULL;
}

void RockTutorialUI::setRenderOptions(GmTypes::RenderOptionType options)
{
   renderOptions = options;
}

void RockTutorialUI::renderWidget(GmDrawable *widget)
{
   widget->render(renderOptions);
}

void RockTutorialUI::renderUI()
{
   renderWidget(mMainDialog);

   if(mGridDialog != NULL)
      renderWidget(mGridDialog);

   if(mFlowDialog != NULL)
      renderWidget(mFlowDialog);

   if(mAboutDialog != NULL)
      renderWidget(mAboutDialog);
} 

void RockTutorialUI::initUI()
{
   // Setup the widgets
   setupMainMenu();
   setupMainDialog();
}

void RockTutorialUI::applyMainDialogLayout()
{
   mMainDialog->layoutInGLArea(-70.0, -50.0,
                               130.0, 90.0, -0.5);
}

void RockTutorialUI::applyGridDialogLayout()
{
   if(mGridDialog != NULL)
      mGridDialog->layoutInGLArea(-70.0, -50.0,
                                  130.0, 90.0, -0.45);
}

void RockTutorialUI::applyFlowDialogLayout()
{
   if(mFlowDialog != NULL)
      mFlowDialog->layoutInGLArea(-70.0, -50.0,
                                  130.0, 90.0, -0.45);
}

void RockTutorialUI::applyAboutDialogLayout()
{
   if(mAboutDialog != NULL)
      mAboutDialog->layoutInGLArea(-70.0, -50.0,
                                   130.0, 90.0, -0.4);
}

void RockTutorialUI::print_usage()
{
   cout << "rockTutorial v0.1" << endl;
   cout << "usage: rockTutorial" << endl;
   cout << endl;
}

void RockTutorialUI::updateMainTitleText(string text)
{
   mMainDialog->setTitleText((char*)text.c_str());
}

/* Implementation for Listeners */

void RockTutorialUI::selectionOccurred(GmSelectionAction* action)
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
      case MENU_VIEW_ZOOM_IN:
         cout << "Zooming in" << endl;
         break;
      case MENU_VIEW_ZOOM_OUT:
         cout << "Zooming out" << endl;
         break;
      case MENU_VIEW_CENTER:
         cout << "Centering view" << endl;
         break;
      case MENU_LAYOUT_GRID:
         createGridDialog();
         break;
      case MENU_LAYOUT_FLOW:
         createFlowDialog();
         break;
      case MENU_HELP_ABOUT:
         createAboutDialog();
         break;
      default:
         break;
   }
}

void RockTutorialUI::buttonPressed(GmButtonAction* action)
{
   GmGridLayout* layout;
   
   switch(action->getSrcWidgetId())
   {
      case GRID_DIALOG_BUTTON1:
         removeGridDialog();
         break;
      case GRID_DIALOG_BUTTON2:
         layout = (GmGridLayout*)mGridDialog->getLayout();
         layout->setNumRows(layout->getNumRows() + 1);
         applyGridDialogLayout();
         break;
      case GRID_DIALOG_BUTTON3:
         layout = (GmGridLayout*)mGridDialog->getLayout();
         layout->setNumRows(layout->getNumRows() - 1);
         applyGridDialogLayout();
         break;
      case GRID_DIALOG_BUTTON4:
         layout = (GmGridLayout*)mGridDialog->getLayout();
         layout->setNumColumns(layout->getNumColumns() + 1);
         applyGridDialogLayout();
         break;
      case GRID_DIALOG_BUTTON5:
         layout = (GmGridLayout*)mGridDialog->getLayout();
         layout->setNumColumns(layout->getNumColumns() - 1);
         applyGridDialogLayout();
         break;
      case GRID_DIALOG_BUTTON6:
         layout = (GmGridLayout*)mGridDialog->getLayout();
         layout->setCellLeftInset(layout->getCellLeftInset() + 0.1);
         applyGridDialogLayout();
         break;
      case GRID_DIALOG_BUTTON7:
         layout = (GmGridLayout*)mGridDialog->getLayout();
         layout->setCellRightInset(layout->getCellRightInset() + 0.1);
         applyGridDialogLayout();
         break;
      case GRID_DIALOG_BUTTON8:
         layout = (GmGridLayout*)mGridDialog->getLayout();
         layout->setCellTopInset(layout->getCellTopInset() + 0.1);
         applyGridDialogLayout();
         break;
      case GRID_DIALOG_BUTTON9:
         layout = (GmGridLayout*)mGridDialog->getLayout();
         layout->setCellBottomInset(layout->getCellBottomInset() + 0.1);
         applyGridDialogLayout();
         break;
      case FLOW_DIALOG_CLOSE_BUTTON:
         removeFlowDialog();
         break;
      case ABOUT_DIALOG_OK_BUTTON:
         removeAboutDialog();
         break;
      default:
         cout << "RockTutorial: mousePress - unhandled button action!" << endl;
         break;
   }
}
