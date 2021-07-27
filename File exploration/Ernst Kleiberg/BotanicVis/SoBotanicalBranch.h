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
/** Header file for API definitions of SoBotanicalBranch
  *
  * @author Valerie Maquil
  *
  * $Header: 
  * @file                                                                   */
 /* ======================================================================= */



#ifndef SoBotanicalBranch_H
#define SoBotanicalBranch_H

#include "SoBotanicalBase.h"
#include <stbapi/util/ivio.h>

#include <string>


/**
 * implements a branch of a botanic tree. Each branch is defined with means of a SoCylinder
 * and a SoCone making the connection between this and the previous branch. The bottom radius
 * of the SoCone corresponds to the radius of the SoCylinder and its peak is sheared in the 
 * direction of the branch.
 *
 * The direction of the branch is definded with 3 rotations. rotBranch defines the rotation
 * of THIS branch around the Y-axis, rotBeta defines the rotation of ALL the branches
 * of this hierarchy around the Y-axis. rotAlpha defines the rotation or this branch around the 
 * Z-axis.

CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>geometry </td><td>SoSeparator</td><td>SoSeparator</td><td>YES</td></tr>
  <tr> <td>hiddenGeometry </td><td>SoAbortGroup</td><td>SoAbortGroup</td><td>YES</td></tr>
  <tr> <td>subbranches </td><td>SoNodeKitListPart</td><td>---</td><td>NO</td></tr>
  <tr> <td>rotBranch </td><td>SoRotationXYZ</td><td>SoRotationXYZ</td><td>NO</td></tr>
  <tr> <td>ext</td><td>SoSeparator</td><td>SoSeparator</td><td>NO</td></tr>
  <tr> <td>shear </td><td>SoMatrixTransform</td><td>SoMatrixTransform</td><td>NO</td></tr>
  <tr> <td>transCone </td><td>SoTranslation</td><td>SoTranslation</td><td>NO</td></tr>
  <tr> <td>extBranch </td><td>SoCone</td><td>SoCone</td><td>NO</td></tr>
  <tr> <td>rotAlpha </td><td>SoRotationXYZ</td><td>SoRotationXYZ</td><td>NO</td></tr>
  <tr> <td>transAfter </td><td>SoTranslation</td><td>SoTranslation</td><td>NO</td></tr>
  <tr> <td>branch </td><td>SoCylinder</td><td>SoCylinder</td><td>NO</td></tr>
  <tr> <td>rotBeta </td><td>SoRotationXYZ</td><td>SoRotationXYZ</td><td>NO</td></tr>
  <tr> <td>transBefore </td><td>SoTranslation</td><td>SoTranslation</td><td>NO</td></tr>
  <tr> <td>subgeometry </td><td>SoCylinder</td><td>SoCylinder</td><td>NO</td></tr>
  </table>

Extra Information for List Parts from Above Table

  <table>
	<tr> <td>Part Name</td><td>Container Type</td><td>Possible Types</td></tr>
	<tr> <td>subbranches</td><td>SoGroup</td><td>SoBotanicalBase</td></tr>
   </table>


File Format / defaults:

@verbatim
SoBotanicalBranch {

  	SoSFFloat direction     	0
	SoSFFloat alpha         	0
	SoSFFloat beta          	1.62 #golden section
	SoSFInt32 strands         	0


    # from SoBotanicalBase
	SoSFInt32 strands			0
	SoSFFloat lengthOfBranch	10.0

}
@endverbatim


  @ingroup bot
*/
class BOTANIC_VIS_API SoBotanicalBranch : public SoBotanicalBase
{
    SO_KIT_HEADER(SoBotanicalBranch);

	/**
	 * children of this branch
	 */
    SO_KIT_CATALOG_ENTRY_HEADER(subbranches);
	/**
	 * in this part the information is stored that shouldn't be rendered 
	 */
    SO_KIT_CATALOG_ENTRY_HEADER(hiddenGeometry);
	/** 
	 * rotation of this branch around the Y-axis, relative to the first branch 
	 * of this hierarchical level
	 */
    SO_KIT_CATALOG_ENTRY_HEADER(rotBranch);
	/**
	 * translation of the cylinder BEFORE any rotations are performed. This 
	 * translation goes in Y-direction with a value of half of the height of 
	 * the cylinder.
	 */
	SO_KIT_CATALOG_ENTRY_HEADER(transBefore);
	/**
	 * rotation of this branch around the Z-axis. The angle of rotation depends 
	 * on the number of strands and on the field alpha 
	 */
    SO_KIT_CATALOG_ENTRY_HEADER(rotAlpha);
	/**
	 * rotation of all the branches of this hierarchical level around the Y-axis, 
	 * relative to the branches of the previous hierarchical level
	 */
    SO_KIT_CATALOG_ENTRY_HEADER(rotBeta);
	/**
	 * translation of the cylinder AFTER all rotations have been performed. This 
	 * translation goes in Y-direction with a value of half of the height of the 
	 * cylinder.
	 */
    SO_KIT_CATALOG_ENTRY_HEADER(transAfter);
	/**
	 * SoCylinder representing the branch
	 */
    SO_KIT_CATALOG_ENTRY_HEADER(branch);
	/**
	 * geometry of the cone making the connection between this and the previous branch
	 */
	SO_KIT_CATALOG_ENTRY_HEADER(ext);
	/**
	 * rotation of the cone around the Y-axis
	 */
	SO_KIT_CATALOG_ENTRY_HEADER(rotExtBranch);
	/**
	 * the cone itself
	 */
	SO_KIT_CATALOG_ENTRY_HEADER(extBranch);
	/**
	 * the translation of the cone in Y-direction, to put it at the bottom of the cylinder.
	 * This translation corresponds to half the height of the cylinder
	 */
	SO_KIT_CATALOG_ENTRY_HEADER(transCone);
	/**
	 * the shearing of the cone: the peak of the cone is sheared in direction of the rotation
	 * of the branch
	 */
	SO_KIT_CATALOG_ENTRY_HEADER(shear);
	/**
	 * pointers of the elements of "subbranches" are stored here to ensure that the above
	 * transformations will be performed on these children too.
	 */
	SO_KIT_CATALOG_ENTRY_HEADER(subgeometry);



public:
	/** 
	 * inventor class initialization
	 */
    static void initClass();
	/**
	 * constructor
	 */
    SoBotanicalBranch();

	/** 
	 * rotation angle of THIS branch around the Y-axis. Is chosen such that the branches are
	 * distributed equally around the previous branch.
	 */
	SoSFFloat direction;
	/**
	 * this angle constitutes a reference for the rotation of each branch around
	 * the Z-axis. The angles of rotAlpha are chosen such that their sum equals the angle
	 * alpha.
	 */
	SoSFFloat alpha;
	/**
	 * rotation angle of ALL the branches of this level around the Y-axis. Often the angle 
	 * 360/phi is chosen where phi = (1 + sqrt(5)/2) is the golden section.
	 */
	SoSFFloat beta;
	/** 
	 * this field stores the number of strands of the father branch. Is used to calculate rotAlpha
	 * and the bottom radius of the cone.
	 */
	SoSFInt32 previousStrand;

	/**
	 * sets the connections of the sensors
	 */
	virtual SbBool setUpConnections( SbBool onOff, SbBool doItAlways = FALSE );

	/**
	 * sets the scaling value for the radius of this branch
	 */
	void setThickness(float size) {thickness = size;};
	/**
	 * sets the alpha of this branch
	 */
	void setAlpha(float angle) {alphaStart = angle;};
	
	/**
	 * calculates the correct parameters for its subbranches and sets their fields.
	 */
	int adjustSubbranches();

	/**
	 * if TRUE this branch has as only child a SoBotanicalFruit
	 */
	bool isFruit;

protected:
	/**
	 * destructor
	 */
    virtual ~SoBotanicalBranch();

	/** 
	 * adapts all parts depending on the length of the branch
	 */
	virtual void resizeHeightCB();
	/** 
	 * adapts all parts depending on the thickness of the branch
	 */
	virtual void resizeThicknessCB();

	/**
	 * field sensor attached to alpha; calls angleSensorCB
	 */
	SoFieldSensor alphaSensor;
	/**
	 * field sensor attached to direction; calls angleSensorCB
	 */
	SoFieldSensor directionSensor;

	/**
	 * adapts rotAlpha, rotBranch and the shearing transformation of the cone
	 */
	static void angleSensorCB(void *, SoSensor*);

	/**
	 * field sensor attached to previousStrand; calls previousStrandSensorCB
	 */
	SoFieldSensor previousStrandSensor;
	/**
	 * adapts the bottom radius, the height and the translation of the cone
	 */
	static void previousStrandSensorCB(void *, SoSensor*);
	
	/**
	 * thickness of this branch
	 */
	float thickness;
	/**
	 * rotation angle of rotAlpha
	 */
	float alphaStart;
};

//----------------------------------------------------------------------------

#endif // SoBotanicalBranch_H