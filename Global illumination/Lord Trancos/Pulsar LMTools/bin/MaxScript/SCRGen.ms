-----------------------------------------------------------------------------
-- Pulsar LMTools v1.1
-- Copyright (C) 2002, 2003 Lord Trancos. All rights reserved.
-----------------------------------------------------------------------------
--
-- This file is part of Pulsar LMTools.
-- 
-- Pulsar LMTools is free software; you can redistribute it and/or
-- modify it under the terms of the GNU General Public License
-- as published by the Free Software Foundation; either
-- version 2 of the License, or any later version.
-- 
-- Pulsar LMTools is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details (license.txt).
-- 
-- You should have received a copy of the GNU General Public License
-- along with this program; if not, write to the Free Software
-- Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
-- 
-- You should also be able to get a copy of the license here:
--   
--   http://www.gnu.org/licenses/gpl.txt
-- 
-----------------------------------------------------------------------------
-- SCRGen v0.1c 
-- by Lord Trancos.
-----------------------------------------------------------------------------
-- v0.1c - Added support for "Target Spot" lights.
-- v0.1b - Added support for "Free Direct" lights.
-----------------------------------------------------------------------------
--
-- This is a script for GMAX 1.1 (should also work with GMAX 1.0 and 3DS Max)
-- used for creating scripts for Pulsar Studio LMTools.
-- 
-- This is my first MaxScript, so do not expect many from it. I wasted only a
-- few time learning MaxScript (only the time needed for writing this script)
-- so the code should be a bit weird :P
--
-- Currently only "Free Direct", "Omni" and "Target Spot" lights are
-- processed.
--
-- Warning: Developed and tested ONLY on GMAX 1.1
--
-----------------------------------------------------------------------------
--
-- Usage:
--
--   Once you have placed the lights using GMax,...
--
--   1. Click on "Utilities" (the tab with the icon of a hammer; on the right
--      of GMAX)
--   2. Then click on the "MAXScript" button.
--   3. Click on the "Open Listener" button.
--   4. Choose "File \ Run Script"
--   5. Browse this file
--   6. Choose "Edit \ Select All"
--   7. Choose "Edit \ Copy"
--   8. On notepad (or something like) choose paste and save the file with
--      ".scr" extension.
--
-----------------------------------------------------------------------------

  ---------------------------

  function colorToHex _c = (

   -- function for writing a color value in delphi-friendly format (hex value)

   _ir = Int _c.r
   _ir = bit.shift _ir 16

   _ig = Int _c.g
   _ig = bit.shift _ig 8

   _ib = Int _c.b

   _irgb = _ir + _ig + _ib

   _srgb = bit.IntAsHex _irgb

   format "$%" _srgb

  )

  ---------------------------

-- clear listener
clearListener()

-- Write script header
format "; Script created automaticaly by SCRGen\n"
format "; -------------------------------------\n"
format "\n"

-- Write ambient light
format "; Ambient light\n"
format "ambient_global.color = "
colorToHex ambientColor
format "\n\n"

-- Write lights
if (lights.count > 0) then
  for _cnt = 1 to lights.count do
    if iskindof lights[_cnt] light do (

    -- light name
    format "; light [%]\n" lights[_cnt].name

    -- free directional lights
    if (lights[_cnt].type == #freeDirect) then (

      -- direction
      _invX = lights[_cnt].dir.x * -1
      _invY = lights[_cnt].dir.y * -1
      _invZ = lights[_cnt].dir.z * -1
      format "directional.direction = %, %, %\n" _invX _invZ _invY

      -- color
      format "directional.color = "
      colorToHex lights[_cnt].rgb
      format "\n"

      -- multiplier
      format "directional.multiplier = %\n" lights[_cnt].multiplier

      -- cast shadows
      format "directional.cast_shadows = %\n" lights[_cnt].castShadows

      -- enabled
      format "directional.enabled = %\n" lights[_cnt].enabled

      -- add
      format "directional.add\n"
    )

    -- omni directional point lights
    if (lights[_cnt].type == #omni) then (

      -- position
      format "omni_point.position = %, %, %\n" \
                                    lights[_cnt].position.x \
                                    lights[_cnt].position.z \
                                    lights[_cnt].position.y

      -- color
      format "omni_point.color = "
      colorToHex lights[_cnt].rgb
      format "\n"

      -- multiplier
      format "omni_point.multiplier = %\n" lights[_cnt].multiplier
  
      -- attenuation
      if (lights[_cnt].useFarAtten) then (
        _attS = lights[_cnt].farAttenStart
        _attE = lights[_cnt].farAttenEnd
      ) else (
        _attS = 0
        _attE = 0
      )
      format "omni_point.attenuation = %, %\n" _attS _attE

      -- cast shadows
      format "omni_point.cast_shadows = %\n" lights[_cnt].castShadows

      -- enabled
      format "omni_point.enabled = %\n" lights[_cnt].enabled

      -- add
      format "omni_point.add\n"
    )

    -- spot lights
    if (lights[_cnt].type == #targetSpot) then (

      -- position
      format "spotlight.position = %, %, %\n" \
                                    lights[_cnt].position.x \
                                    lights[_cnt].position.z \
                                    lights[_cnt].position.y

      -- target
      format "spotlight.target = %, %, %\n" \
                                 lights[_cnt].target.position.x \
                                 lights[_cnt].target.position.z \
                                 lights[_cnt].target.position.y

      -- theta (hotspot)
      _theta = degToRad(lights[_cnt].hotspot)
      format "spotlight.theta = %\n" _theta

      -- phi (falloff)
      _phi = degToRad(lights[_cnt].falloff)
      format "spotlight.phi = %\n" _phi

      -- color
      format "spotlight.color = "
      colorToHex lights[_cnt].rgb
      format "\n"

      -- multiplier
      format "spotlight.multiplier = %\n" lights[_cnt].multiplier
  
      -- attenuation
      if (lights[_cnt].useFarAtten) then (
        _attS = lights[_cnt].farAttenStart
        _attE = lights[_cnt].farAttenEnd
      ) else (
        _attS = 0
        _attE = 0
      )
      format "spotlight.attenuation = %, %\n" _attS _attE

      -- cast shadows
      format "spotlight.cast_shadows = %\n" lights[_cnt].castShadows

      -- enabled
      format "spotlight.enabled = %\n" lights[_cnt].enabled

      -- add
      format "spotlight.add\n"     
    )

    -- next light
    format "\n"

)