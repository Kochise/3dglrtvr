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
/** Header file for API definitions of SoBotanicVis
  *
  * @author Valerie Maquil
  *
  * $Header: 
  * @file                                                                   */
 /* ======================================================================= */

#ifndef SoBotanicVis_H
#define SoBotanicVis_H

#ifdef WIN32
#include <SoWinEnterScope.h>
#endif


#include <Inventor/fields/SoSFFloat.h>
#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFInt32.h>
#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/fields/SoSFTrigger.h>
#include "SoBuildBotanicAction.h"


#ifdef WIN32
#include <SoWinLeaveScope.h>
#endif


#include <string>
#include "BotanicVis.h"

//----------------------------------------------------------------------------

/**
 * provides means to realise a botanical visualisation of a hierarchical structure.
 * Therefore the information need to be structured into SoTreeNode's and SoTreeLeaf's, 
 * where a SoTreeNode may have multiple SoTreeNode's and SoTreeLeaf's as children as in 
 * a conventional n-ary tree.
 *
 * This structure will then be reorganized in a botanical tree where a set of leaves corresponds
 * to a fruit and a node corresponds to a branch. The field "numOfBranches" indicates here
 * the number of branching connections leaving one branch, "lengthOfBranches" indicates the 
 * height of the SoCylinder of each branch and "thickness" specifies a scaling value for the 
 * radius of these cylinders.
 *
 * More information on this visualisation technique can be found in the paper "Botanical
 * Visualization of huge hierarchies" by Ernst Kleiberg, Huub van de Wetering and Jarke
 * J. van Wijk on the url http://www.win.tue.nl/~vanwijk/botatree.pdf

CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>geometry </td><td>SoSeparator</td><td>SoSeparator</td><td>NO</td></tr>
  <tr> <td>treeElements </td><td>SoSeparator</td><td>SoSeparator</td><td>YES</td></tr>
  </table>


File Format / defaults:

@verbatim
SoBotanicVis {

	SoSFInt32   numOfBranches		3
	SoSFFloat   lengthOfBranches	10.0f
	SoSFFloat   thickness			0.5f
	SoSFFloat   alpha				1.0f
	SoSFFloat   beta				1.62f #golden ratio
	SoSFTrigger trigger			

}
@endverbatim


  @ingroup bot
*/
class BOTANIC_VIS_API SoBotanicVis : public SoBaseKit
{
    SO_KIT_HEADER(SoBotanicVis);
	/**
	 * contains the geometry of the botanic tree; is build automatically
	 */
	SO_KIT_CATALOG_ENTRY_HEADER(geometry);
	/**
	 * contains the conventional tree to be visualized
	 */
	SO_KIT_CATALOG_ENTRY_HEADER(treeElements);

public:
	/** 
	 * Inventor class initialization 
	 */
    static void initClass();
	/**
	 * constructor
	 */
    SoBotanicVis();

	/**
	 * field that has to be connected to a SoPushButton to interactively rebuild 
	 * the botanical tree
	 */
	SoSFTrigger trigger;
	/**
	 * this field specifies the height of the cylinders representing the branches
	 */
	SoSFFloat lengthOfBranches;
	/**
	 * scaling value for the radius of the cylinders representing the branches.
	 * The relative value is dependent on the number of strands of each branch.
	 */
	SoSFFloat thickness;
	/**
	 * this angle constitutes a reference for the rotation of each branch around
	 * the Z-axis. 
	 */
	SoSFFloat alpha;
	/**
	 * The angle beta specifies a rotation angle around the Y-axis which remains
	 * constantly during the building process. Each branch is rotated with this 
	 * angle beta to ensure a displacing of the branches and fruits
	 */
	SoSFFloat beta;
	/**
	 * This field specifies how many branching connections should at most leave one
	 * branch.
	 */
	SoSFInt32 numOfBranches;

	/**
	 * sets the connections of the sensors
	 */
	virtual SbBool setUpConnections( SbBool onOff, SbBool doItAlways = FALSE );

protected:

	/**
	 * starts the SoBuildBotanicAction to build the botanical tree
	 */
	void buildTree();
	/**
	 * destructor
	 */
	virtual ~SoBotanicVis();

	/**
	 * is used to interactively change the parameters of the tree. If TRUE, the 
	 * structure of the botanical tree need to be reformed and the action need
	 * to be started again
	 */
	bool numOfBranchesChanged;

	/**
	 * this sensor detects changes of the field numOfBranches and sets the flag
	 * numOfBranchesChanged to TRUE
	 */
	SoFieldSensor treeSensor;
	/**
	 * sets the flag numOfBranchesChanged to TRUE
	 */
	static void treeSensorCB(void *data, SoSensor *sen);

	/** 
	 * this sensor is attached to the field "trigger"
	 */
	SoFieldSensor triggerSensor;
	/** 
	 * is called when the field "trigger" is triggered, i.e. when the SoPushButton
	 * coupled to this field is pressed.
	 */
	static void triggerSensorCB(void *data, SoSensor *sen);

};

//----------------------------------------------------------------------------

#endif // SoBotanicVis_H