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
/** Implementation of SoBotanicalBase
  *
  * @author Valerie Maquil
  *
  * $Header: 
  * @file                                                                   */
 /* ======================================================================= */

#include "SoBotanicalBase.h"
#include <Inventor/nodes/SoSeparator.h>


//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

SO_KIT_SOURCE(SoBotanicalBase);

//----------------------------------------------------------------------------

void SoBotanicalBase::initClass()
{
	SO_KIT_INIT_CLASS(SoBotanicalBase, SoBaseKit, "BaseKit");
}

//----------------------------------------------------------------------------

SoBotanicalBase::SoBotanicalBase()
{
	//std::cout << "SoBotanicalBase::constructor" << std::endl;

	 SO_KIT_CONSTRUCTOR(SoBotanicalBase);
    
    //---                     partName, partClassName, nullByDefault, parentName, rightName, isPublicPart
	 SO_KIT_ADD_CATALOG_ENTRY(geometry, SoSeparator,   TRUE, this,           \x0, TRUE );


    SO_NODE_ADD_FIELD(lengthOfBranch, (10.0f));
	SO_NODE_ADD_FIELD(strands, (0));	
	
	SO_KIT_INIT_INSTANCE();

	lengthOfBranchSensor.setFunction(lengthOfBranchCB);
	lengthOfBranchSensor.setData(this);

	strandsSensor.setFunction(strandsCB);
	strandsSensor.setData(this);

    setUpConnections(TRUE, TRUE);

}

//----------------------------------------------------------------------------

SoBotanicalBase::~SoBotanicalBase()
{
}


//----------------------------------------------------------------------------

SbBool SoBotanicalBase::setUpConnections( SbBool onOff, SbBool doItAlways )
{
		//std::cout << "SoBotanicalBase::setUpConnections" << std::endl;

 if (!doItAlways && connectionsSetUp == onOff)
    return onOff;
  if (onOff) {
    // We connect AFTER base class.
    SoBaseKit::setUpConnections(onOff, doItAlways);
    //std::cout << "SoBotanicalBase::setSensor" << std::endl;
	lengthOfBranchSensor.attach(&lengthOfBranch);
	lengthOfBranchSensor.schedule();
	strandsSensor.attach(&strands);
	strandsSensor.schedule();

  }
  else {
    // We disconnect BEFORE base class.
    // Disconnect the field sensors.
	lengthOfBranchSensor.detach();
	strandsSensor.detach();
    SoBaseKit::setUpConnections(onOff, doItAlways);
  }
  return !(connectionsSetUp = onOff);
}

//----------------------------------------------------------------------------

void SoBotanicalBase::lengthOfBranchCB(void *data, SoSensor*)
{
	//std::cout << "SoBotanicalBase::lengthOfBranchCB" << std::endl;

	SoBotanicalBase* self = (SoBotanicalBase*)data;
	assert(self != NULL);
		
	self->resizeHeightCB();
}

void SoBotanicalBase::strandsCB(void *data, SoSensor*)
{
	//std::cout << "SoBotanicalBase::strandsCB" << std::endl;
	SoBotanicalBase* self = (SoBotanicalBase*)data;
	assert(self != NULL);
		
	self->resizeThicknessCB();
}

//-----------------------------------------------------------------------------