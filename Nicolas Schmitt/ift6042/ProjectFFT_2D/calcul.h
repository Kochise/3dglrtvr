/*
 *   Copyright (c) 2002 Nicolas Schmitt
 *
 *   This file is part of (s)Smoke.
 *
 *   (s)Smoke is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   (s)Smoke is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details (gpl.txt).
 *
 *   You should have received a copy of the GNU General Public License
 *   along with (s)Smoke; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#ifndef _CALCUL_H_
#define _CALCUL_H_

extern void calcul ( void );
extern void RenderScene ( void );
extern void InitModel ( void );
extern void FreeModel ( void );
extern void ResetSimulation ( void );
extern void MakeSources ( void);
extern void MakeSources2 ( void);
extern void MakeForces ( void );

#endif // _CALCUL_H_
