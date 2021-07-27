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
/** Implementation of SoBotanicalFruit
  *
  * @author Valerie Maquil
  *
  * $Header: 
  * @file                                                                   */
 /* ======================================================================= */

#include "SoBotanicalFruit.h"
#include "SoTreeLeaf.h"
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoMaterial.h>



//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoBotanicalFruit);

//----------------------------------------------------------------------------

void SoBotanicalFruit::initClass()
{
	SO_KIT_INIT_CLASS(SoBotanicalFruit, SoBotanicalBase, "SoBotanicalBase");
}

//----------------------------------------------------------------------------

SoBotanicalFruit::SoBotanicalFruit()
{
	 SO_KIT_CONSTRUCTOR(SoBotanicalFruit);
    
    //--- partName, partClassName, nullByDefault, parentName, rightName, isPublicPart
	SO_KIT_ADD_CATALOG_ENTRY(color, SoMaterial,   FALSE, geometry,           \x0, TRUE );
	SO_KIT_ADD_CATALOG_ENTRY(fruit, SoSphere,   FALSE, geometry,           \x0, TRUE );

	SO_KIT_ADD_CATALOG_LIST_ENTRY(data,		SoGroup, FALSE, this, \x0, SoTreeLeaf, TRUE);
		
	SO_KIT_INIT_INSTANCE();	

	((SoMaterial*)color.getValue())->diffuseColor.set1Value(0, 0, .9f, 0);

	dataSensor.setData(this);
	dataSensor.setFunction(dataSensorCB);

	setUpConnections(TRUE, TRUE);
}

//----------------------------------------------------------------------------

SoBotanicalFruit::~SoBotanicalFruit()
{
}

//----------------------------------------------------------------------------

void SoBotanicalFruit::resizeHeightCB()
{
	//do nothing
}

void SoBotanicalFruit::resizeThicknessCB()
{
	//do nothing
}

//----------------------------------------------------------------------------

SbBool SoBotanicalFruit::setUpConnections( SbBool onOff, SbBool doItAlways )
{
	//std::cout << "SoBotanicalFruit::setUpConnections" << std::endl;

 if (!doItAlways && connectionsSetUp == onOff)
    return onOff;
  if (onOff) {
    // We connect AFTER base class.
    SoBotanicalBase::setUpConnections(onOff, doItAlways);
    
	dataSensor.attach(data.getValue());
	dataSensor.schedule();
	dataSensor.setPriority(0);

  }
  else {
    // We disconnect BEFORE base class.
    // Disconnect the field sensors.
	dataSensor.detach();

    SoBotanicalBase::setUpConnections(onOff, doItAlways);
  }
  return !(connectionsSetUp = onOff);
}

//----------------------------------------------------------------------------

void SoBotanicalFruit::dataSensorCB( void *data, SoSensor *sen)
{
	//std::cout << "SoBotanicalFruit::dataSensorCB" << std::endl;

	SoBotanicalFruit* self = (SoBotanicalFruit*)data;
		
	SoNodeKitListPart *fruitData = SO_GET_PART(self, "data", SoNodeKitListPart);

	int numChildren = fruitData->getNumChildren();
	int totalSize = 0;

	for (int i = 0; i<numChildren; i++)
	{
		SoTreeLeaf *child = (SoTreeLeaf *)fruitData->getChild(i);

		totalSize += child->size.getValue();
	}

	self->size = log((float)totalSize);

	SoSphere* sph = SO_GET_PART(self, "fruit", SoSphere);
	sph->radius.setValue(log((float)totalSize/2.0));

}


//----------------------------------------------------------------------------