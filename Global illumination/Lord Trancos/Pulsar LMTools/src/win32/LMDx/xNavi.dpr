// ---------------------------------------------------------------------------
// Pulsar LMTools v1.1
// Copyright (C) 2002, 2003 Lord Trancos. All rights reserved.
// ---------------------------------------------------------------------------
// 
// This file is part of Pulsar LMTools
// 
// Pulsar LMTools is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or any later version.
// 
// Pulsar LMTools is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details (license.txt).
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
// 
// You should also be able to get a copy of the license here:
//   
//   http://www.gnu.org/licenses/gpl.txt
// 
// ---------------------------------------------------------------------------
//
// Developed by Lord Trancos <lordtrancos@softhome.net> 
//
// ---------------------------------------------------------------------------
//
// DXLab - Visit us at: http://www.dxlab.tk or http://dxlab.host.sk
//
// ---------------------------------------------------------------------------

// LMDX - LMTS Viewer & Direct3D .X to .LMTS converter

program xNavi;

uses
  Forms,
  xNaviU1 in 'xNaviU1.pas' {Form1},

  DX8GFX in '..\DX8GFX.PAS',
  DX8XMESH in '..\DX8XMESH.PAS',
  DX8TEXT in '..\DX8TEXT.PAS',
  DX8LPE in '..\DX8LPE.PAS',
  DX8NOIDX in '..\DX8NOIDX.PAS',
  DX8GUI3D in '..\DX8GUI3D.PAS',
  DX8LMTS in '..\DX8LMTS.PAS',
  DX8DI in '..\DX8DI.PAS',

  lmts in '..\..\cross\lmts.pas',
  tga in '..\..\cross\tga.pas',
  bitmap in '..\..\cross\bitmap.pas',
  filez in '..\..\cross\filez.pas',

  filez_ex in '..\filez_ex.pas',
  xcomdlg in '..\xcomdlg.pas',
  timing in '..\timing.pas',

  common in 'common.pas',
  xexport in 'xexport.pas',
  xNaviU2 in 'xNaviU2.pas' {Form2},
  xNaviU3 in 'xNaviU3.pas' {Form3},
  xNaviU4 in 'xNaviU4.pas' {Form4};

{$R *.RES}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
