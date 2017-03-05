/*
 * Standard definitions file
 * 
 * This file is part of the "SoftPixel Engine" (Copyright (c) 2008 by Lukas Hermanns)
 * See "SoftPixelEngine.hpp" for license information.
 */

#include "Base/spStandard.hpp"
#include "Base/spInternalDeclarations.hpp"


namespace sp
{


/*
 * Internal global members
 */

s32 __Screen_Width  = 640;
s32 __Screen_Height = 480;

s32 __Screen_X = 0;
s32 __Screen_Y = 0;

bool __isKey        [MAX_COUNT_OF_KEYCODES] = { 0 };
bool __wasKey       [MAX_COUNT_OF_KEYCODES] = { 0 };
bool __hitKey       [MAX_COUNT_OF_KEYCODES] = { 0 };
bool __isMouseKey   [MAX_COUNT_OF_MOUSEKEYS] = { 0 };
bool __wasMouseKey  [MAX_COUNT_OF_MOUSEKEYS] = { 0 };
bool __hitMouseKey  [MAX_COUNT_OF_MOUSEKEYS] = { 0 };

s32 __CursorSpeedX = 0, __CursorSpeedY = 0, __MouseWheel = 0;

bool __isLighting = false, __isFog = false, __isTexturing = true;

f32 __GeneralRunTime = 1.0f;


} // /namespace sp



// ================================================================================
