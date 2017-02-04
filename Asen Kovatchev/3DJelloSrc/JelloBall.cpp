////////////////////////////////////////////////////////////////////////////////
//
//	JelloBall.cpp - 3D Physical Modelling of jello mass
//
//	Project:	3D Jello
//
//	Author:		Asen L. Kovatchev (aceh@bulinfo.net)
//	Created:	Nov 20, 1999
//
////////////////////////////////////////////////////////////////////////////////



#include "StdMain.h"
#include "JelloBall.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction

CJelloBall::CJelloBall()
{
	m_nVerts = 0;
	m_nLinks = 0;
	m_nTriLinks = 0;
	m_nCylVerts = 0;
	m_nCylLinks = 0;

	m_fX = 0.0f;
	m_fY = 0.0f;
	m_fZ = 0.0f;
	m_fR = 5.0f;
}

CJelloBall::~CJelloBall()
{
}

//////////////////////////////////////////////////////////////////////
// Implementation

void CJelloBall::Draw( IDirect3DDevice7 *pDev )
{
//	pDev->DrawIndexedPrimitive( D3DPT_LINELIST, D3DFVF_VERTEX, m_lpVerts, m_nVerts, m_LinksFromTo, 2 * m_nLinks, D3DDP_WAIT );
//	pDev->DrawPrimitive( D3DPT_LINESTRIP, D3DFVF_VERTEX, m_lpVerts, m_nVerts, D3DDP_WAIT );
//	pDev->DrawPrimitive( D3DPT_POINTLIST, D3DFVF_VERTEX, m_lpVerts, m_nVerts, D3DDP_WAIT );
	pDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_VERTEX, m_lpVerts, m_nVerts, m_TriLinks, m_nTriLinks, D3DDP_WAIT );
}

void CJelloBall::DrawCyl( IDirect3DDevice7 *pDev )
{
	pDev->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, D3DFVF_VERTEX, m_lpCylVerts, m_nCylVerts, m_CylLinks, m_nCylLinks, D3DDP_WAIT );
//	pDev->DrawPrimitive( D3DPT_LINESTRIP, D3DFVF_VERTEX, m_lpCylVerts, m_nCylVerts, D3DDP_WAIT );
}

void CJelloBall::CreateBall( float fX, float fY, float fZ, float fR )
{
	int i, j, m, n;
	float x, y, z;
	float alfa, beta;

	m = 5;
	n = 64;

	m_nVerts = 0;

	for (i = 0; i <= m; i++)
	{
		beta = D3DX_PI * i / m;
		y = (float)(fY + cos(beta) * fR);

		if ((i != 0) && (i != (m - 1)))
		{
			for (j = 0; j <= n; j++)
			{
				alfa = 2 * D3DX_PI * j / n;
				x = (float)(fX + sin(alfa) * sin(beta) * fR);
				z = (float)(fZ + cos(alfa) * sin(beta) * fR);
				AddVertex( x, y, z, 0.01f, 0.0f, 0.0f );
			}
		}
		else
		{
			AddVertex( fX, y, fZ, 0.01f, 0.0f, 0.0f );
		}
	}
}

int CJelloBall::AddVertex( float x, float y, float z,
						   float dx, float dy, float dz)
{
	int n = m_nVerts;

	const float k = 4.0f;

//	dx = dy = dz = 0.0f;

	m_lpVerts[n] = D3DVERTEX( D3DVECTOR( k * x, k * y, k * z ),
							  D3DVECTOR( x - m_fX, y - m_fY, z - m_fZ ),
							  (x + z) / 10.0f,
							  (y + z) / 10.0f );

	m_S[n] = D3DVECTOR( dx, dy, dz );
	m_nVerts++;
	return n;
}

void CJelloBall::FrameMove()
{
	int a1, a2, i;

	for (i = 0; i < m_nVerts; i++)
	{
		// Move atoms
		m_S[i] *= 0.95f;
		m_S[i].y = m_S[i].y - 0.1f;

		m_lpVerts[i].x += m_S[i].x;
		m_lpVerts[i].y += m_S[i].y;
		m_lpVerts[i].z += m_S[i].z;

		// Return back hit atoms and change their speed
		if (HitTest(m_lpVerts[i].x, m_lpVerts[i].y, m_lpVerts[i].z))
		{
			m_S[i] = -m_S[i];
			m_lpVerts[i].x += m_S[i].x;
			m_lpVerts[i].y += m_S[i].y;
			m_lpVerts[i].z += m_S[i].z;
		}
	}

	for (i = 0; i < m_nLinks; i++)
	{
		// Adjust links
		float dx, dy, dz, l, s;
		a1 = m_LinkFrom[i]; a2 = m_LinkTo[i];
		dx = m_lpVerts[a2].x - m_lpVerts[a1].x;
		dy = m_lpVerts[a2].y - m_lpVerts[a1].y;
		dz = m_lpVerts[a2].z - m_lpVerts[a1].z;

		l = (float)sqrt(fabs(dx * dx + dy * dy + dz * dz));
		dx /= l; dy /= l; dz /= l;

		s = (l - m_LinkLen[i]);
		s /= m_LinkLen[i]; s /= m_LinkLen[i];
		s *= 2.0f;
		m_S[a2].x -= s * dx;
		m_S[a2].y -= s * dy;
		m_S[a2].z -= s * dz;

		m_S[a1].x += s * dx;
		m_S[a1].y += s * dy;
		m_S[a1].z += s * dz;
	}
}

BOOL CJelloBall::HitTest(float x, float y, float z)
{
	BOOL bHit;

	bHit = FALSE;

	bHit = bHit || (y <= -20.0f);
	bHit = bHit || (y >= 20.0f);
	bHit = bHit || (x <= -40.0f);
	bHit = bHit || (x >= 40.0f);
	bHit = bHit || (z <= -50.0f);
	bHit = bHit || (z >= 50.0f);

	bHit = bHit || 
		(((x - m_fCX) * (x - m_fCX) + (y - m_fCY) * (y - m_fCY) <= m_fCR * m_fCR) &&
		 ((z >= m_fCZ2) && (z <= m_fCZ1)));

	return bHit;
}

void CJelloBall::CreateBox(float fX, float fY, float fZ)
{
	int x, y, z;

const int k = 5;

	WORD A[k][k][k];

	m_nVerts = 0;
	m_nLinks = 0;
	m_nTriLinks = 0;

	// Create vertices
	for (x = 0; x < k; x++)
	{
		for (y = 0; y < k; y++)
		{
			for (z = 0; z < k; z++)
			{
				A[x][y][z] = AddVertex( fX + x - k / 2.0f,
										fY + y - k / 2.0f,
										fZ + z - k / 2.0f,
										0.0f,
										0.0f,
//										-0.2f * (y) / k,
//										-0.7f * (z - k / 4.0f) / k,
										0.0f);
			}
		}
	}

	int i, j;

	for (i = 0; i < m_nVerts; i++)
		for (j = i + 1; j < m_nVerts; j++)
		{
			AddLink(i, j);
		}

	// Create links
/*
	for (x = 0; x < k; x++)
		for (y = 0; y < k; y++)
			for (z = 0; z < k; z++)
			{
				if (x > 1) AddLink( A[x][y][z], A[x - 2][    y][    z]);
				if (y > 1) AddLink( A[x][y][z], A[    x][y - 2][    z]);
				if (z > 1) AddLink( A[x][y][z], A[    x][    y][z - 2]);

				if ((x > 1) && (y > 1))
				{
					AddLink( A[x][y][z], A[x - 2][y - 2][    z]);	
				}

				if ((x > 1) && (y > 1))
				{
					AddLink( A[x - 2][y][z], A[x][y - 2][    z]);	
				}

				if ((x > 1) && (z > 1))
				{
					AddLink( A[x][y][z], A[x - 2][y][z - 2]);
				}

				if ((x > 1) && (z > 1))
				{
					AddLink( A[x - 2][y][z], A[x][y][z - 2]);
				}

				if ((y > 1) && (z > 1))
				{
					AddLink( A[x][y][z], A[x][y - 2][z - 2]);
				}

				if ((y > 1) && (z > 1))
				{
					AddLink( A[x][y - 2][z], A[x][y][z - 2]);
				}

				if ((x > 1) && (y > 1) && (z > 1))
				{
					AddLink( A[x][y][z], A[x - 2][y - 2][z - 2]);
					AddLink( A[x - 2][y][z], A[x][y - 2][z - 2]);
					AddLink( A[x][y - 2][z], A[x - 2][y][z - 2]);
					AddLink( A[x][y][z - 2], A[x - 2][y - 2][z]);
				}

				if (x > 0) AddLink( A[x][y][z], A[x - 1][    y][    z]);
				if (y > 0) AddLink( A[x][y][z], A[    x][y - 1][    z]);
				if (z > 0) AddLink( A[x][y][z], A[    x][    y][z - 1]);

				if ((x > 0) && (y > 0))
				{
					AddLink( A[x][y][z], A[x - 1][y - 1][    z]);	
				}

				if ((x > 0) && (y > 0))
				{
					AddLink( A[x - 1][y][z], A[x][y - 1][    z]);	
				}

				if ((x > 0) && (z > 0))
				{
					AddLink( A[x][y][z], A[x - 1][y][z - 1]);
				}

				if ((x > 0) && (z > 0))
				{
					AddLink( A[x - 1][y][z], A[x][y][z - 1]);
				}

				if ((y > 0) && (z > 0))
				{
					AddLink( A[x][y][z], A[x][y - 1][z - 1]);
				}

				if ((y > 0) && (z > 0))
				{
					AddLink( A[x][y - 1][z], A[x][y][z - 1]);
				}

				if ((x > 0) && (y > 0) && (z > 0))
				{
					AddLink( A[x][y][z], A[x - 1][y - 1][z - 1]);
					AddLink( A[x - 1][y][z], A[x][y - 1][z - 1]);
					AddLink( A[x][y - 1][z], A[x - 1][y][z - 1]);
					AddLink( A[x][y][z - 1], A[x - 1][y - 1][z]);
				}
			}
*/

	// Create triangle links
	for (x = 0; x < k; x++)
	{
		for (y = 0; y < k; y++)
		{
			for (z = 0; z < k; z++)
			{
				if ((z == 0) || (z == (k - 1)))
				{
					if ((x > 0) && (y > 0))
					{
						AddTriLink( A[x][y][z], A[x-1][y][z], A[x-1][y-1][z] );
						AddTriLink( A[x][y][z], A[x-1][y-1][z], A[x][y-1][z] );
						AddTriLink( A[x-1][y][z], A[x][y][z], A[x-1][y-1][z] );
						AddTriLink( A[x-1][y-1][z], A[x][y][z], A[x][y-1][z] );
					}
				}

				if ((x == 0) || (x == (k-1)))
				{
					if ((y > 0) && (z > 0))
					{
						AddTriLink( A[x][y][z], A[x][y-1][z], A[x][y-1][z-1] );
						AddTriLink( A[x][y][z], A[x][y-1][z-1], A[x][y][z-1] );
						AddTriLink( A[x][y-1][z], A[x][y][z], A[x][y-1][z-1] );
						AddTriLink( A[x][y-1][z-1], A[x][y][z], A[x][y][z-1] );
					}
				}

				if ((y == 0) || (y == (k - 1)))
				{
					if ((x > 0) && (z > 0))
					{
						AddTriLink( A[x][y][z], A[x-1][y][z], A[x-1][y][z-1] );
						AddTriLink( A[x][y][z], A[x-1][y][z-1], A[x][y][z-1] );
						AddTriLink( A[x-1][y][z], A[x][y][z], A[x-1][y][z-1] );
						AddTriLink( A[x-1][y][z-1], A[x][y][z], A[x][y][z-1] );
					}
				}
			}
		}
	}

	// Create cylinder
	CreateCyl();
}

void CJelloBall::AddLink(int nFrom, int nTo)
{
	int n = m_nLinks;
	m_LinkFrom[n] = nFrom;
	m_LinkTo[n] = nTo;
	m_LinksFromTo[2 * n] = nFrom;
	m_LinksFromTo[2 * n + 1] = nTo;
	
	float dx, dy, dz;
	dx = m_lpVerts[nTo].x - m_lpVerts[nFrom].x;
	dy = m_lpVerts[nTo].y - m_lpVerts[nFrom].y;
	dz = m_lpVerts[nTo].z - m_lpVerts[nFrom].z;
	m_LinkLen[n] = (float)sqrt(fabs(dx * dx + dy * dy + dz * dz));
	
	m_nLinks++;
}

void CJelloBall::AddTriLink(int a1, int a2, int a3)
{
	m_TriLinks[m_nTriLinks++] = a1;
	m_TriLinks[m_nTriLinks++] = a2;
	m_TriLinks[m_nTriLinks++] = a3;
}

void CJelloBall::AddTriCyl(int a1, int a2, int a3)
{
	m_CylLinks[m_nCylLinks++] = a1;
	m_CylLinks[m_nCylLinks++] = a2;
	m_CylLinks[m_nCylLinks++] = a3;
}

void CJelloBall::CreateCyl()
{
	int i, j;

const int	m = 2;
const int	n = 32;

	int A[m][n];
	int front, back;
	float x, y, z, alfa;

	m_fCX = 0.2f;
	m_fCY = -20.0f;
	m_fCZ1 = 20.0f;
	m_fCZ2 = -20.0f;
	m_fCR = 5.0f;

	for (i = 0; i < m; i++)
	{
		z = m_fCZ1 + (m_fCZ2 - m_fCZ1) * i / (m-1);

		for (j = 0; j < n; j++)
		{
			alfa = 2 * D3DX_PI * j / (n-1);
			x = (float)(m_fCX + cos(alfa) * m_fCR);
			y = (float)(m_fCY + sin(alfa) * m_fCR);
			A[i][j] = AddCylVertex( x, y, z );
		}
	}

	front = AddCylVertex( m_fCX, m_fCY, m_fCZ1 );
	back = AddCylVertex( m_fCX, m_fCY, m_fCZ2 );

	for (i = 1; i < m; i++)
	{
		for (j = 1; j < n; j++)
		{
			AddTriCyl( A[i][j], A[i][j-1], A[i-1][j-1]);
			AddTriCyl( A[i][j], A[i-1][j-1], A[i-1][j]);
			AddTriCyl( A[i][j-1], A[i][j], A[i-1][j-1]);
			AddTriCyl( A[i-1][j-1], A[i][j], A[i-1][j]);
		}

		AddTriCyl( A[i][0], A[i][n-1], A[i-1][n-1]);
		AddTriCyl( A[i][0], A[i-1][n-1], A[i-1][0]);
		AddTriCyl( A[i][n-1], A[i][0], A[i-1][n-1]);
		AddTriCyl( A[i-1][n-1], A[i][0], A[i-1][0]);
	}

	for (i = 1; i < n; i++)
	{
		AddTriCyl( A[0][i], A[0][i-1], front);
		AddTriCyl( A[0][i-1], A[0][i], front);
		AddTriCyl( A[m-1][i-1], A[m-1][i], back);
		AddTriCyl( A[m-1][i], A[m-1][i-1], back);
	}
}

int CJelloBall::AddCylVertex( float x, float y, float z )
{
	int n = m_nCylVerts;
	m_lpCylVerts[n] = D3DVERTEX( D3DVECTOR( x, y, z ),
								 D3DVECTOR( x - m_fCX, y - m_fCY, z ),
								 (x) / 10.0f,
								 (y + z) / 10.0f );

	m_nCylVerts++;
	return n;
}


void CJelloBall::MoveCyl(float dx, float dy, float dz)
{
	int i;

	if ((m_fCX + dx) < m_fCR - 40.0f) dx = 0.0f;
	if ((m_fCX + dx) > 40.0f - m_fCR) dx = 0.0f;
	if ((m_fCY + dy) < -20.0f) dy = 0.0f;
	if ((m_fCY + dy) > 20.0f - m_fCR) dy = 0.0f;

	for (i = 0; i < m_nCylVerts; i++)
	{
		D3DXVECTOR3 v;
		D3DXVECTOR4 v4;

		D3DXMATRIX mat, mat2;

		v.x = m_lpCylVerts[i].x;
		v.y = m_lpCylVerts[i].y;
		v.z = m_lpCylVerts[i].z;

		D3DXMatrixTranslation( &mat, -m_fCX, -m_fCY, 0.0f );

		D3DXMatrixRotationZ( &mat2, -dx / 10.0f );
		D3DXMatrixMultiply( &mat, &mat, &mat2 );

		D3DXMatrixTranslation( &mat2, m_fCX + dx, m_fCY + dy, dz );
		D3DXMatrixMultiply( &mat, &mat, &mat2 );

		D3DXVec3Transform( &v4, &v, &mat );

		m_lpCylVerts[i].x = v4.x / v4.w;
		m_lpCylVerts[i].y = v4.y / v4.w;
		m_lpCylVerts[i].z = v4.z / v4.w;
	}

	m_fCX += dx;
	m_fCY += dy;
	m_fCZ1 += dz;
	m_fCZ2 += dz;

	for (i = 0; i < m_nVerts; i++)
	{
		if (HitTest(  m_lpVerts[i].x,
				  m_lpVerts[i].y,
				  m_lpVerts[i].z ))
		{
			float dx, dy, l, r;
			
			dx = m_lpVerts[i].x - m_fCX;
			dy = m_lpVerts[i].y - m_fCY;
			l = (float)sqrt(dx * dx + dy * dy);
			r = m_fCR;

			m_lpVerts[i].x = 1.2f * dx * r / l + m_fCX;
			m_lpVerts[i].y = 1.2f * dy * r / l + m_fCY;
		}
	}
}

void CJelloBall::MoveJello(float dx, float dy, float dz)
{
	int i;
	for (i = 0; i < m_nVerts; i++)
	{
		m_S[i].x += dx;
		m_S[i].y += dy;
		m_S[i].z += dz;
	}
}
