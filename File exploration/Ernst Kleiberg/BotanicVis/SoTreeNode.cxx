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

 /* ======================================================================= */
/** Implementation of SoTreeNode
  *
  * @author Valerie Maquil
  *
  * $Header:
  * @file                                                                   */
 /* ======================================================================= */

#include "SoTreeNode.h"
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>



//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoTreeNode);

//----------------------------------------------------------------------------

void SoTreeNode::initClass()
{
	SO_KIT_INIT_CLASS(SoTreeNode, SoTreeBase, "SoTreeBase");
}

//----------------------------------------------------------------------------

SoTreeNode::SoTreeNode()
{
	 SO_KIT_CONSTRUCTOR(SoTreeNode);
    
    //--- partName, partClassName, nullByDefault, parentName, rightName, isPublicPart

	SO_KIT_ADD_CATALOG_LIST_ENTRY(subnodes,		SoGroup, FALSE, this, \x0, SoTreeBase, TRUE);
	
		
	SO_KIT_INIT_INSTANCE();	


}

//----------------------------------------------------------------------------

SoTreeNode::~SoTreeNode()
{
}

//----------------------------------------------------------------------------