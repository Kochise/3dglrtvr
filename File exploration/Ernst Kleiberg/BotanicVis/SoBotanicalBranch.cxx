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
/** Implementation of SoBotanicalBranch
  *
  * @author Valerie Maquil
  *
  * $Header:
  * @file                                                                   */
 /* ======================================================================= */

#include "SoBotanicalBranch.h"
#include "SoBotanicalFruit.h"
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCylinder.h>
#include <Inventor/nodes/SoRotationXYZ.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoMatrixTransform.h>
#include <stbapi/misc/SoAbortGroup.h>


//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoBotanicalBranch);

//----------------------------------------------------------------------------

void SoBotanicalBranch::initClass()
{
	SO_KIT_INIT_CLASS(SoBotanicalBranch, SoBotanicalBase, "SoBotanicalBase");
}

//----------------------------------------------------------------------------

SoBotanicalBranch::SoBotanicalBranch()
{
	 SO_KIT_CONSTRUCTOR(SoBotanicalBranch);
    
    //--- partName, partClassName, nullByDefault, parentName, rightName, isPublicPart
    SO_KIT_ADD_CATALOG_ENTRY(hiddenGeometry, SoAbortGroup,   TRUE, this,           \x0, TRUE );

	SO_KIT_ADD_CATALOG_LIST_ENTRY(subbranches,		SoGroup, FALSE, hiddenGeometry, \x0, SoBotanicalBase, TRUE);
	

	SO_KIT_ADD_CATALOG_ENTRY(rotBranch, SoRotationXYZ,   FALSE, geometry,           \x0, TRUE );
	SO_KIT_ADD_CATALOG_ENTRY(ext, SoSeparator,   FALSE, geometry,           \x0, TRUE );
	SO_KIT_ADD_CATALOG_ENTRY(shear, SoMatrixTransform,   FALSE, ext,           \x0, TRUE );
	SO_KIT_ADD_CATALOG_ENTRY(transCone, SoTranslation,   FALSE, ext,           \x0, TRUE );
	SO_KIT_ADD_CATALOG_ENTRY(extBranch, SoCone,   FALSE, ext,           \x0, TRUE );
	SO_KIT_ADD_CATALOG_ENTRY(rotAlpha, SoRotationXYZ,   FALSE, geometry,           \x0, TRUE );
	SO_KIT_ADD_CATALOG_ENTRY(transAfter, SoTranslation,   FALSE, geometry,           \x0, TRUE );

	SO_KIT_ADD_CATALOG_ENTRY(branch, SoCylinder,   FALSE, geometry,           \x0, TRUE );

	SO_KIT_ADD_CATALOG_ENTRY(rotBeta, SoRotationXYZ,   FALSE, geometry,           \x0, TRUE );
	SO_KIT_ADD_CATALOG_ENTRY(transBefore, SoTranslation,   FALSE, geometry,           \x0, TRUE );

	SO_KIT_ADD_CATALOG_ENTRY(subgeometry, SoGroup, FALSE, geometry, \x0, TRUE);

    SO_NODE_ADD_FIELD(direction, (0));
	SO_NODE_ADD_FIELD(alpha, (0));
    SO_NODE_ADD_FIELD(beta, (1.62f));
	SO_NODE_ADD_FIELD(previousStrand, (0));
		
	SO_KIT_INIT_INSTANCE();	

	SoAbortGroup *abortGrp = (SoAbortGroup *)readNodeFromMemory("SoAbortGroup { abortActions (RENDER | BBOX) }");
		assert( abortGrp != NULL );
		assert( setPart("hiddenGeometry", abortGrp ) );


	alphaSensor.setFunction(angleSensorCB);
	alphaSensor.setData(this);

	directionSensor.setFunction(angleSensorCB);
	directionSensor.setData(this);

	previousStrandSensor.setFunction(previousStrandSensorCB);
	previousStrandSensor.setData(this);


    setUpConnections(TRUE, TRUE);


	isFruit = false;
}

//----------------------------------------------------------------------------

SoBotanicalBranch::~SoBotanicalBranch()
{
}

//----------------------------------------------------------------------------

	
SbBool SoBotanicalBranch::setUpConnections( SbBool onOff, SbBool doItAlways )
{
 if (!doItAlways && connectionsSetUp == onOff)
    return onOff;
  if (onOff) {
    // We connect AFTER base class.
    SoBotanicalBase::setUpConnections(onOff, doItAlways);
	
	SoRotationXYZ* rBeta = SO_GET_PART(this, "rotBeta", SoRotationXYZ);
	rBeta->axis.setValue("Y");
	(rBeta->angle).connectFrom(&beta);

	previousStrandSensor.attach(&previousStrand);
	previousStrandSensor.schedule();
   	directionSensor.attach(&direction);
	directionSensor.schedule();
	alphaSensor.attach(&alpha);
	alphaSensor.schedule();



  }
  else {
    // We disconnect BEFORE base class.
    // Disconnect the field sensors.
	alphaSensor.detach();
	directionSensor.detach();
	previousStrandSensor.detach();
    SoBotanicalBase::setUpConnections(onOff, doItAlways);
  }
  return !(connectionsSetUp = onOff);
}

//----------------------------------------------------------------------------

void SoBotanicalBranch::resizeHeightCB()
{
	//std::cout << "SoBotanicalBranch::resizeHeight " << lengthOfBranch.getValue()/2.0 << std::endl;
	SoCylinder* cyl = SO_GET_PART(this, "branch", SoCylinder);
	cyl->height.setValue(lengthOfBranch.getValue());

	SoTranslation* t1 = SO_GET_PART(this, "transBefore", SoTranslation);
	t1->translation.setValue(0,lengthOfBranch.getValue()/2.0f, 0);

	SoTranslation* t2 = SO_GET_PART(this, "transAfter", SoTranslation);
	t2->translation.setValue(0,lengthOfBranch.getValue()/2.0f, 0);

}

void SoBotanicalBranch::resizeThicknessCB()
{
	//std::cout << "SoBotanicalBranch::resizeThickness" << sqrt(strands.getValue())/200.0f << std::endl;
	SoCylinder* cyl = SO_GET_PART(this, "branch", SoCylinder);
	cyl->radius.setValue(sqrt(strands.getValue())*thickness);

}

int SoBotanicalBranch::adjustSubbranches()
{
	((SoGroup *)subgeometry.getValue())->removeAllChildren();

	SoNodeKitListPart *subbranchList = (SoNodeKitListPart *)subbranches.getValue();
	
	int numChildren = subbranchList->getNumChildren();

	//std::cout << "SoBotanicalBase::numChildren" << numChildren << std::endl;
	float dir = 0;
	float diff = 6.28304/numChildren;

	int sumStrands = 0;

	for (int i=0; i<numChildren; i++)
	{
		//std::cout << "SoBotanicalBase::adjust child" << i << std::endl;
		
		SoBotanicalBase *child = (SoBotanicalBase *)subbranchList->getChild(i);
		
		if (child->isOfType(SoBotanicalBranch::getClassTypeId()) )
		{	
			SoBotanicalBranch *branch = (SoBotanicalBranch *)child;
			branch->setThickness(thickness);
			branch->setAlpha(alphaStart);
			branch->lengthOfBranch.setValue(lengthOfBranch.getValue());
			branch->direction.setValue(dir);
			branch->beta.setValue(beta.getValue());

			((SoGroup *)subgeometry.getValue())->addChild(branch);

			dir+=diff;

			int sum = branch->adjustSubbranches();

			branch->strands.setValue(sum);
		}
		else if (child->isOfType(SoBotanicalFruit::getClassTypeId()))
		{
			float str = ((SoBotanicalFruit *)child)->getSize();
			child->strands.setValue(str);
			((SoGroup *)subgeometry.getValue())->addChild(child);

		}

	sumStrands += child->strands.getValue();


	}

	for (int j=0; j<numChildren; j++)
	{
		SoBotanicalBase *child = (SoBotanicalBase *)subbranchList->getChild(j);
		
		if (child->isOfType(SoBotanicalBranch::getClassTypeId()) ) 
		{
			SoBotanicalBranch *branch = (SoBotanicalBranch *)child;
			branch->previousStrand.setValue(sumStrands);
		    branch->alpha.setValue((float)(alphaStart*(float)branch->strands.getValue()/(float)sumStrands));
		}

	}

	return sumStrands;
}

//-------------------------------------------------------------------------------------------

void SoBotanicalBranch::angleSensorCB(void *data, SoSensor*)
{

	SoBotanicalBranch* self = (SoBotanicalBranch*)data;
	assert(self != NULL);

	SoRotationXYZ* rAlpha = SO_GET_PART(self, "rotAlpha", SoRotationXYZ);
	rAlpha->axis.setValue("Z");
	rAlpha->angle.setValue(self->alpha.getValue());

	SoRotationXYZ* rBranch = SO_GET_PART(self, "rotBranch", SoRotationXYZ);
	rBranch->axis.setValue("Y");
	rBranch->angle.setValue(self->direction.getValue());

	SoMatrixTransform* shear = SO_GET_PART(self, "shear", SoMatrixTransform);
	shear->matrix.setValue(1,0,0,0,-tan(self->alpha.getValue()),1,0,0,0,0,1,0,0,0,0,1);
	
}


void SoBotanicalBranch::previousStrandSensorCB(void *data, SoSensor*)
{
	
	SoBotanicalBranch* self = (SoBotanicalBranch*)data;
	assert(self != NULL);
		
	SoCone* con = SO_GET_PART(self, "extBranch", SoCone);
	con->bottomRadius.setValue(sqrt(self->previousStrand.getValue())*self->thickness);
	con->height.setValue(sqrt(self->previousStrand.getValue())*self->thickness);

	SoTranslation* t4 = SO_GET_PART(self, "transCone", SoTranslation);
	t4->translation.setValue(0,sqrt(self->previousStrand.getValue())*self->thickness/2.0,0);

}