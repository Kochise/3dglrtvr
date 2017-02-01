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
/** Header file for API definitions of SoTreeNode
  *
  * @author Valerie Maquil
  *
  * $Header: 
  * @file                                                                   */
 /* ======================================================================= */


#ifndef SoTreeNode_H
#define SoTreeNode_H

#include "SoTreeBase.h"
#include <stbapi/util/ivio.h>

#include <string>


/**
 * implements a node of a conventional tree. The subnodes and leaves of this node 
 * are specified in the part "subnodes"
 
  All Parts
  <table>
  <tr> <td>Part Name</td><td>Part Type</td><td>Default Type</td><td>NULL Default</td></tr>
  <tr> <td>subnodes </td><td>SoNodeKitListPart</td><td>---</td><td>NO</td></tr>
  </table>

Extra Information for List Parts from Above Table

  <table>
	<tr> <td>Part Name</td><td>Container Type</td><td>Possible Types</td></tr>
	<tr> <td>subnodes</td><td>SoGroup</td><td>SoTreeBase</td></tr>
   </table>


  @ingroup bot
*/
class BOTANIC_VIS_API SoTreeNode : public SoTreeBase
{
    SO_KIT_HEADER(SoTreeNode);
	/** 
	 * subnodes and leaves of this node
	 */
    SO_KIT_CATALOG_ENTRY_HEADER(subnodes);


public:
	/**
	 * inventor class initialization
	 */
    static void initClass();
	/**
	 * constructor
	 */
    SoTreeNode();

protected:
	/**
	 * destructor
	 */
    virtual ~SoTreeNode();

};

//----------------------------------------------------------------------------

#endif // SoTreeNode_H