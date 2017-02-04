////////////////////////////////////////////////////////////////////////////////
//
//	JelloBall.h - 3D Physical Modelling of jello mass
//
//	Project:	3D Jello
//
//	Author:		Asen L. Kovatchev (aceh@bulinfo.net)
//	Created:	Nov 20, 1999
//
////////////////////////////////////////////////////////////////////////////////



#if !defined(AFX_JELLOBALL_H__781D97C0_9FA2_11D3_991B_525400E657F3__INCLUDED_)
#define AFX_JELLOBALL_H__781D97C0_9FA2_11D3_991B_525400E657F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#define MAX_VERTICES	1000000



class CJelloBall  
{
public:
	void MoveJello( float dx, float dy, float dz );
	CJelloBall();
	virtual ~CJelloBall();

	float	m_fX, m_fY, m_fZ, m_fR;

	BOOL HitTest( float x, float y, float z);
	void FrameMove();
	void CreateBall( float fX, float fY, float fZ, float fR );
	void CreateBox(float fX, float fY, float fZ);
	void Draw( IDirect3DDevice7 * pDev );
	void DrawCyl( IDirect3DDevice7 * pDev );
	void MoveCyl( float dx, float dy, float dz );

	int			m_nVerts;
	D3DVERTEX	m_lpVerts[MAX_VERTICES];	// D3D vertices
	D3DVECTOR	m_S[MAX_VERTICES];			// D3D vector speed

	int		m_nLinks;
	WORD	m_LinkFrom[MAX_VERTICES], m_LinkTo[MAX_VERTICES];
	float	m_LinkLen[MAX_VERTICES];
	WORD	m_LinksFromTo[MAX_VERTICES];
	WORD	m_TriLinks[MAX_VERTICES];
	int		m_nTriLinks;

	// cylinder data
	int			m_nCylVerts;
	D3DVERTEX	m_lpCylVerts[MAX_VERTICES];	// D3D vertices
	WORD	m_CylLinks[MAX_VERTICES];
	int		m_nCylLinks;
	float	m_fCX, m_fCY, m_fCZ1, m_fCZ2, m_fCR;

private:
	void CreateCyl();
	int AddVertex( float x, float y, float z, float dx, float dy, float dz );
	int AddCylVertex( float x, float y, float z );
	void AddLink( int nFrom, int nTo );
	void AddTriLink( int a1, int a2, int a3 );
	void AddTriCyl( int a1, int a2, int a3 );
};



#endif // !defined(AFX_JELLOBALL_H__781D97C0_9FA2_11D3_991B_525400E657F3__INCLUDED_)
