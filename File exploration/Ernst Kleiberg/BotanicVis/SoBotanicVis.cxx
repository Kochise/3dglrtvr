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
/** Implementation of SoBotanicVis
  *
  * @author Valerie Maquil
  *
  * $Header: 
  * @file                                                                   */
 /* ======================================================================= */

#include "SoBotanicVis.h"


#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSeparator.h>
#include "SoBotanicalBranch.h"
#include "SoBotanicalFruit.h"
#include "SoTreeLeaf.h"
#include "SoTreeBase.h"
#include "SoTreeNode.h"

#include <iostream>

using namespace std;


//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoBotanicVis);

//----------------------------------------------------------------------------

void SoBotanicVis::initClass()
{
    SO_KIT_INIT_CLASS(SoBotanicVis, SoBaseKit, "BaseKit");

	SoBotanicalBase::initClass();
	SoBotanicalBranch::initClass();
	SoBotanicalFruit::initClass();
	SoTreeBase::initClass();
	SoTreeLeaf::initClass();
	SoTreeNode::initClass();
	SoBuildBotanicAction::initClass();

}

//----------------------------------------------------------------------------

SoBotanicVis::SoBotanicVis()//
{
	 SO_KIT_CONSTRUCTOR(SoBotanicVis);
    
    //--- partName, partClassName, nullByDefault, parentName, rightName, isPublicPart

   //---                     partName, partClassName, nullByDefault, parentName, rightName, isPublicPart
    SO_KIT_ADD_CATALOG_ENTRY(geometry,		SoSeparator,  FALSE, this, \x0, TRUE);
	SO_KIT_ADD_CATALOG_ENTRY(treeElements, SoSeparator,  TRUE, this, \x0, TRUE);

			
	SO_NODE_ADD_FIELD(lengthOfBranches,  (10.0f));
	SO_NODE_ADD_FIELD(thickness,  (0.5f));
	SO_NODE_ADD_FIELD(trigger, ());
	SO_NODE_ADD_FIELD(alpha, (1.0f));
	SO_NODE_ADD_FIELD(beta, (1.62f));
	SO_NODE_ADD_FIELD(numOfBranches, (3));


	SO_KIT_INIT_INSTANCE();	

	treeSensor.setData(this);
	treeSensor.setFunction(treeSensorCB);

	triggerSensor.setData(this);
	triggerSensor.setFunction(triggerSensorCB);

	numOfBranchesChanged = false;

	setUpConnections(TRUE, TRUE);


}

//----------------------------------------------------------------------------

SoBotanicVis::~SoBotanicVis()
{

}

//----------------------------------------------------------------------------

void SoBotanicVis::triggerSensorCB( void *data, SoSensor *sen)
{
	std::cout << "SoBotanicVis::triggerSensorCB" << endl;

	SoBotanicVis* self = (SoBotanicVis*)data;

	if ((self->treeElements.getValue() != NULL) & self->numOfBranchesChanged)
	{
		self->buildTree();
		self->numOfBranchesChanged = false;
	}


	SoSeparator *sep = ((SoSeparator *)self->geometry.getValue());

	if (sep->getNumChildren() != 0)
	{
		std::cout << "SoBotanicVis::adaptGeometry" << endl;

		SoBotanicalBranch *branch = ((SoBotanicalBranch *)sep->getChild(0));
		branch->setThickness(self->thickness.getValue());
		branch->setAlpha(self->alpha.getValue());
		branch->beta.setValue(self->beta.getValue());
		branch->lengthOfBranch.setValue(self->lengthOfBranches.getValue());

		int sum = branch->adjustSubbranches();
		branch->strands.setValue(sum);

	}

	writeFile("botanicVis.iv",self,FALSE);

}


//----------------------------------------------------------------------------

void SoBotanicVis::treeSensorCB( void *data, SoSensor *sen)
{
	SoBotanicVis* self = (SoBotanicVis*)data;
	std::cout << "SoBotanicVis::treeSensorCB" << self->numOfBranches.getValue() << endl;

	self->treeSensor.detach();

	self->numOfBranchesChanged = true;

	self->treeSensor.attach(&(self->numOfBranches));

}


//----------------------------------------------------------------------------

void SoBotanicVis::buildTree()
{
	//
	std::cout << "SoBotanicVis::buildTree " << numOfBranches.getValue()+2 << endl;

	SoBuildBotanicAction treeAction;
	treeAction.setNumBranches(numOfBranches.getValue());
	treeAction.apply(treeElements.getValue());

	SoSeparator *sep = treeAction.getGeometry();

	//remove geometry
	if (geometry.getValue() != NULL)
	{
		std::cout << "SoBotanicVis::removeChildren " << endl;
		((SoSeparator *)geometry.getValue())->removeAllChildren();
	}

	// add new geometry
	int numChildren = sep->getNumChildren();

	for (int i = 0; i<numChildren; i++)
	{
		SoNode *child = sep->getChild(i);
		((SoSeparator *)geometry.getValue())->addChild( child );
	}


	//writeFile("botanicVis.iv",this,FALSE);
	
}
#pragma
//----------------------------------------------------------------------------

SbBool SoBotanicVis::setUpConnections( SbBool onOff, SbBool doItAlways )
{
 if (!doItAlways && connectionsSetUp == onOff)
    return onOff;
  if (onOff) {
    // We connect AFTER base class.
    SoBaseKit::setUpConnections(onOff, doItAlways);
    
    // do your own special setup here !
	triggerSensor.attach(&trigger);
	triggerSensor.schedule();

 	treeSensor.attach(&numOfBranches);
	treeSensor.schedule();

 }
  else {
    // We disconnect BEFORE base class.
    // Disconnect the field sensors.
	treeSensor.detach();
	triggerSensor.detach();
    SoBaseKit::setUpConnections(onOff, doItAlways);
  }
  return !(connectionsSetUp = onOff);
}

//----------------------------------------------------------------------------

