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
/** Header file for API definitions of SoBotanicalBase
  *
  * @author Valerie Maquil
  *
  * $Header: 
  * @file                                                                   */
 /* ======================================================================= */



#ifndef SoBotanicalBase_H
#define SoBotanicalBase_H

#ifdef _WIN32
#include <SoWinEnterScope.h>
#endif

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/sensors/SoFieldSensor.h>

#ifdef _WIN32
#include <SoWinLeaveScope.h>
#endif

#include "BotanicVis.h"
#include <string>
#include <stdlib.h>
#include <iostream>


/**
 * base class for all elements of a botanical tree

  CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>geometry </td><td>SoSeparator</td><td>SoSeparator</td><td>YES</td></tr>
  </table>



  File Format / defaults:

  @verbatim
  SoBotanicalBase {
	  SoSFInt32 strands			0
	  SoSFFloat lengthOfBranch	10.0
  }
  @endverbatim


  @ingroup bot
 */
class BOTANIC_VIS_API SoBotanicalBase : public SoBaseKit
{
    SO_KIT_HEADER(SoBotanicBase);
	/**
	 * contains the geometry of this element of the botanical tree
	 */
    SO_KIT_CATALOG_ENTRY_HEADER(geometry);


public:
	/**
	 * this field specifies the height of the cylinders representing the branches
	 */
	SoSFFloat lengthOfBranch;
	/**
	 * in this field the number of strands of this element is stored. The number of strands
	 * of one botanical branch equals the sum of strands of its subbranches. The number
	 * of strands of a fruit depends on the size of the leafs represented by this fruit
	 */ 
	SoSFInt32 strands;	

	/** 
	 *Inventor class initialization 
	 */
    static void initClass();
	/** 
	 * constructor
	 */
    SoBotanicalBase();

	/**
	 * sets the connections of the sensors
	 */
	virtual SbBool setUpConnections( SbBool onOff, SbBool doItAlways = FALSE );

protected:
	/**
	 * destructor
	 */
    virtual ~SoBotanicalBase();

	/** 
	 * sensor attached to the field "lengthOfBranches"
	 */
	SoFieldSensor lengthOfBranchSensor;
	/**
	 * calls resizeHeightCB()
	 */
	static void lengthOfBranchCB(void *, SoSensor*);
	virtual void resizeHeightCB() {};///< does nothing

	/** 
	 * sensor attached to the field "strands"
	 */
	SoFieldSensor strandsSensor;
	/**
	 * calls strandsCB()
	 */
	static void strandsCB(void *, SoSensor*);
	virtual void resizeThicknessCB() {};///< does nothing


};

//----------------------------------------------------------------------------

#endif 