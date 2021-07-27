/* ========================================================================
 * Copyright (C) 2000,2001,2002  Vienna University of Technology
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
 * PROJECT: animal
 * ======================================================================== */
/**
 * header file for SoBuildPucStyleAction
 *
 * @author  Valerie Maquil
 *
 * $Header:
 * @file                                                                   */
/* ======================================================================= */

#ifndef __SOBuildBotanicAction_H__
#define __SOBuildBotanicAction_H__


#include <Inventor/actions/SoSubAction.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoTransform.h>


#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif


#include "BotanicVis.h"
#include "SoBotanicalBranch.h"
#include "SoBotanicalFruit.h"

#include <string>
#include <stdlib.h>


/**
 * reorganises a hierarchical structure consisting of SoTreeNode's and SoTreeLeaf's
 * into a botanical hierachical structure consisting of SoBotanicalBranch'es and 
 * SoBotanicalFruit's.
 *
 * This action is thus only responsible for determing the structure of the botanical 
 * tree and doesn't calculate any transformations of its elements.


  @ingroup bot
*/
class BOTANIC_VIS_API SoBuildBotanicAction : public SoAction {
	SO_ACTION_HEADER(SoBuildBotanicAction);

public:

	/**
	 * inventor class initialisation
	 */
	static void initClass();

	/** 
	 * constructor
	 */
	SoBuildBotanicAction();
    /**
	 * destructor
	 */
	virtual ~SoBuildBotanicAction();

	/**
	 * returns the structure of the botanical tree
	 */
	SoSeparator *getGeometry() const {return treeLayout; }

	/** 
	 * sets the number of branching connections to num
	 */
	void setNumBranches(int num) {numBranches = num;}

protected:

	/**
	 * is called before traversal of the scenegraph
	 */
	virtual void beginTraversal(SoNode *node);

private:

	/** function that is called for nodes that perform a default action behaviour
	 * e.g.separators
     */
	static void callDoAction(SoAction *, SoNode *);
	/**
	 * function that is called for nodes of type SoTreeNode
	 */
	static void treeNodeAction(SoAction *, SoNode *);
	/**
	 * function that is called for nodes of type SoTreeLeaf
	 */
	static void treeLeafAction(SoAction *, SoNode *);

	/**
	 * contains the new structure of the botanical tree
	 */
	SoSeparator *treeLayout;

	/**
	 * number of branching connections leaving one branch
	 */
	int numBranches;
	/**
	 * pointer to the branch where the next child should be added
	 */
	SoBotanicalBranch *curBranch;
	/**
	 * pointer to the node list of the branch where the next child should 
	 * be added
	 */
	SoNodeKitListPart *curBranchList;
	/**
	 * if TRUE the current node that is traversed is the root node
	 */
	bool first;

};


#ifdef WIN32
#include <SoWinEnterScope.h>
#endif

#endif