 /* ========================================================================
  * Copyright (C) 2000  Vienna University of Technology
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 2.1 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, write to the Free Software
  * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
  *
  * For further information please contact Dieter Schmalstieg under
  * <schmalstieg@ims.tuwien.ac.at> or write to Dieter Schmalstieg,
  * Vienna University of Technology, Favoritenstr. 9-11/188, A1040 Vienna,
  * Austria.
  * ======================================================================== 
  * PROJECT: Studierstube
  * ======================================================================== */ 
/** Implementation of class SoPucBuildLayoutAction
  *
  * @author Valerie Maquil
  *
  * $Header: /home/cvs/cvsroot/studierstube/src/extensions/PUC/SoPucBuildLayoutAction.cxx,v 1.3 2003/07/03 13:01:15 tomp Exp $
  * @file                                                                   */
 /* ======================================================================= */


#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/misc/SoChildList.h>

#include "SoTreeLeaf.h"
#include "SoTreeNode.h"


#include "SoBuildBotanicAction.h"

#include <iostream>

using namespace std;




SO_ACTION_SOURCE(SoBuildBotanicAction);

//
// initialize the class
// (must be called before any instance of the class is created)
//

void SoBuildBotanicAction::initClass()
{

    SO_ACTION_INIT_CLASS(SoBuildBotanicAction, SoAction);

  
    // register static methods to be called for a specific node

	SO_ACTION_ADD_METHOD(SoNode,			callDoAction);
	SO_ACTION_ADD_METHOD(SoNodeKitListPart,	callDoAction);
	SO_ACTION_ADD_METHOD(SoGroup,			callDoAction);

	SO_ACTION_ADD_METHOD(SoTreeLeaf,    treeLeafAction);
	SO_ACTION_ADD_METHOD(SoTreeNode,    treeNodeAction);

}



//
// constructor
//

SoBuildBotanicAction::SoBuildBotanicAction()
{
    SO_ACTION_CONSTRUCTOR(SoBuildBotanicAction);

	treeLayout = new SoSeparator();

}


//
// destructor
//

SoBuildBotanicAction::~SoBuildBotanicAction()
{	
}



//
// begin the traversal of the scenegraph
//


void SoBuildBotanicAction::beginTraversal(SoNode *node)
{	
	first = true;

	traverse(node);	
}







void SoBuildBotanicAction::callDoAction(SoAction *action, SoNode *node)
{
	//std::cout << "SoBuildBotanicAction::callDoAction" << std::endl;

	node->doAction(action);
}


//-------------------------------------------------------------------------------------------------------------------

void SoBuildBotanicAction::treeLeafAction(SoAction *action_, SoNode *node_)
{
	//std::cout << "SoBuildBotanicAction::treeLeafAction" << std::endl;
	SoBuildBotanicAction *action = (SoBuildBotanicAction *)action_;
	SoTreeLeaf *node = (SoTreeLeaf *)node_;

	int numChildren = action->curBranchList->getNumChildren();

	bool firstFruit = true;
	SoBotanicalBranch *fruitBranch;

	for (int i = 0; i<numChildren; i++)
	{
		SoBotanicalBranch *child = (SoBotanicalBranch *)action->curBranchList->getChild(i);
		if (child->isFruit==true) 
		{
			fruitBranch = (SoBotanicalBranch *)child;
			firstFruit = false;
		}
	}

	if (firstFruit == true)
	{
		fruitBranch = new SoBotanicalBranch();
		SoNodeKitListPart *fruitBranchList = SO_GET_PART(fruitBranch, "subbranches", SoNodeKitListPart);
		fruitBranch->isFruit = true;

		SoBotanicalFruit *fruit = new SoBotanicalFruit();
		SoNodeKitListPart *fruitData = SO_GET_PART(fruit, "data", SoNodeKitListPart);
		fruitData->addChild(node);

		fruitBranchList->addChild(fruit);
		action->curBranchList->addChild(fruitBranch);
	}
	else
	{		
		SoNodeKitListPart *fruitBranchList = SO_GET_PART(fruitBranch, "subbranches", SoNodeKitListPart);
		SoBotanicalFruit *fruit = (SoBotanicalFruit *)fruitBranchList->getChild(0);

		SoNodeKitListPart *fruitData = SO_GET_PART(fruit, "data", SoNodeKitListPart);
		fruitData->addChild(node);

	}



}


//-------------------------------------------------------------------------------------------------------------------

void SoBuildBotanicAction::treeNodeAction(SoAction *action_, SoNode *node_)
{
	//std::cout << "SoBuildBotanicAction::treeNodeAction" << std::endl;

	SoBuildBotanicAction *action = (SoBuildBotanicAction *)action_;
	SoTreeNode *node = (SoTreeNode *)node_;


	if (action->first == true) //if root
	{
		action->first = false;

		action->curBranch = new SoBotanicalBranch();
		action->treeLayout->addChild(action->curBranch);
		action->curBranchList = SO_GET_PART(action->curBranch, "subbranches", SoNodeKitListPart);
		
		node->doAction(action);

	}
	else
	{	
		SoBotanicalBranch *previousBranch = action->curBranch;
		SoBotanicalBranch *branch = new SoBotanicalBranch();
		action->curBranchList->addChild(branch);

		int curNumBranches = action->curBranchList->getNumChildren();

		action->curBranchList = SO_GET_PART(branch, "subbranches", SoNodeKitListPart);

		if (curNumBranches >= action->numBranches)
		{
			previousBranch = branch;
			branch = new SoBotanicalBranch();
			action->curBranchList->addChild(branch);
			action->curBranchList = SO_GET_PART(branch, "subbranches", SoNodeKitListPart);
		}
	
		action->curBranch = branch;

		node->doAction(action);

		action->curBranch = previousBranch;
		action->curBranchList = SO_GET_PART(previousBranch, "subbranches", SoNodeKitListPart);
	}

}
