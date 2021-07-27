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
/** Header file for API definitions of SoBotanicalFruit
  *
  * @author Valerie Maquil
  *
  * $Header: 
  * @file                                                                   */
 /* ======================================================================= */



#ifndef SoBotanicalFruit_H
#define SoBotanicalFruit_H

#include "SoBotanicalBase.h"
#include <Inventor/sensors/SoNodeSensor.h>

#include <string>


/**
 * implements a fruit of a botanical tree. Each fruit is represented by a SoSphere
 * with variable radius and colour. A fruit corresponds to one or more SoTreeLeaf's 
 * where the radius is commuted by adding up the sizes of the SoTreeLeaf's.

CATALOG PARTS

  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>translation </td><td>SoTranslation</td><td>SoTranslation</td><td>NO</td></tr>
  <tr> <td>color </td><td>SoMaterial</td><td>SoMaterial</td><td>NO</td></tr>
  <tr> <td>fruit </td><td>SoSphere</td><td>SoSphere</td><td>NO</td></tr>
  <tr> <td>data </td><td>SoNodeKitListPart</td><td>---</td><td>NO</td></tr>
  </table>

Extra Information for List Parts from Above Table

  <table>
	<tr> <td>Part Name</td><td>Container Type</td><td>Possible Types</td></tr>
	<tr> <td>data</td><td>SoGroup</td><td>SoTreeLeaf</td></tr>
   </table>

File Format / defaults:

@verbatim
SoBotanicalFruit {

    # from SoBotanicalBase
	SoSFInt32 strands			0
	SoSFFloat lengthOfBranch	10.0

}
@endverbatim


  @ingroup bot
*/
class BOTANIC_VIS_API SoBotanicalFruit : public SoBotanicalBase
{
    SO_KIT_HEADER(SoBotanicalFruit);
	/**
	 * colour of the fruit
	 */
	SO_KIT_CATALOG_ENTRY_HEADER(color);
	/**
	 * SoSphere representing the fruit
	 */
    SO_KIT_CATALOG_ENTRY_HEADER(fruit);
	/**
	 * list of the SoTreeLeaf's represented by this fruit
	 */
	SO_KIT_CATALOG_ENTRY_HEADER(data);



public:
	/**
	 * inventor class initialization
	 */
    static void initClass();
	/**
	 * constructor
	 */
    SoBotanicalFruit();

	/**
	 * sets the connections of the sensors
	 */
	virtual SbBool setUpConnections( SbBool onOff, SbBool doItAlways = FALSE );

	/**
	 * returns the size of this fruit
	 */
	float getSize() {return size;};

protected:
	/**
	 * destructor
	 */
    virtual ~SoBotanicalFruit();


	virtual void resizeHeightCB();
	virtual void resizeThicknessCB();

	SoNodeSensor dataSensor;
	/**
	 * adapts the geometry of the fruit using the information stored in "data"
	 */
	static void dataSensorCB(void *data, SoSensor *sen);

	/**
	 * size of this fruit. Corresponds to a logarithmic value of the sum of the 
	 * sizes of the leaves represented by this fruit
	 */
	float size;



};

//----------------------------------------------------------------------------

#endif