////////////////////////////////////////////////////////////////////////////////
//
//	DXApp.h - DirectX Application Support Definitions
//
//	Project:	3D Jello
//
//	Author:		Asen L. Kovatchev (aceh@bulinfo.net)
//	Created:	Nov 19, 1999
//
////////////////////////////////////////////////////////////////////////////////



#if !defined(AFX_DXAPP_H__3B73380E_9E2D_11D3_991B_525400E657F3__INCLUDED_)
#define AFX_DXAPP_H__3B73380E_9E2D_11D3_991B_525400E657F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "JelloBall.h"



class CDXApp  
{
public:
	CDXApp();
	virtual ~CDXApp();

	HRESULT InitD3D( HWND hWnd );

	void DeleteSceneObjects();
	void InitSceneObjects();

	void FrameMove(float fTimeKey);
	void RenderScene();
	void ShowFrame();


	ID3DXContext*			m_pd3dx;
	IDirectDraw7*			m_pDD;
	IDirect3D7*				m_pD3D;
	IDirect3DDevice7*		m_pd3dDevice;

	ID3DXSimpleShape*		m_pd3dxRoom;

	// keyboard support
	char					m_bKey[256];

	// materials
    D3DMATERIAL7 m_d3dMaterial1;
    D3DMATERIAL7 m_d3dMaterial2;

	// textures
	LPDIRECTDRAWSURFACE7	m_ptxtBricks;
	LPDIRECTDRAWSURFACE7	m_ptxtJello;
	LPDIRECTDRAWSURFACE7	m_ptxtObjects;

	// camera movement
	D3DXVECTOR3 m_vecVelocity;
	D3DXVECTOR3 m_vecAngularVelocity;
	D3DXVECTOR3 m_vecJello;
	float		m_fSpeed;
	float		m_fAngularSpeed;
	float		m_gr;

	D3DXMATRIX m_matView;
	D3DXMATRIX m_matPosition;
	D3DXMATRIX m_matProjection;
	D3DXMATRIX m_matWorld;

	// room dimensions
	float	m_fBoxX, m_fBoxY, m_fBoxZ;

	// some objects
	CJelloBall	m_JelloBall;
	D3DXVECTOR3 m_vecCylVel;

	BOOL	m_bShowHelp;
};



#endif // !defined(AFX_DXAPP_H__3B73380E_9E2D_11D3_991B_525400E657F3__INCLUDED_)
