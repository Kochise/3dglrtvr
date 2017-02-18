#pragma once
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include "Renderable.h"
#include "Vector.h"
#include "Matrix.h"

//new mesh vertex structure for shadow
struct VERTEX
{
    D3DXVECTOR3 p;
    D3DXVECTOR3 n;
    FLOAT       tu, tv;

    static const DWORD FVF;
};

class Mesh:  public CRenderable 
{
public:
	virtual ~Mesh(void);
	Mesh(char *name);
	HRESULT initTextMesh(char *value);
	HRESULT init();
	HRESULT SetFVF( DWORD dwFVF );
	void render();
	void cleanup();
	

	void SetTransform(CMatrix m) {m_Transform = m;}

	CVector3 getMin() {return m_vObjectMin;}
	CVector3 getMax() {return m_vObjectMax;}
	CComPtr<ID3DXMesh> getMesh() {return m_pMesh;}
private:
	char *filename;
	CComPtr<ID3DXMesh>              m_pMesh; // Our mesh object in sysmem
	D3DMATERIAL9*           m_pMeshMaterials; // Materials for our mesh
	LPDIRECT3DTEXTURE9*     m_pMeshTextures; // Textures for our mesh
	DWORD                   m_dwNumMaterials;   // Number of mesh materials

	float                   m_fObjectRadius;
	CVector3                m_vObjectCenter;

	CVector3                m_vObjectMin;
	CVector3                m_vObjectMax;

	CMatrix                 m_Transform;
};

typedef SmartPtr<Mesh> MeshPtr;
typedef vector<MeshPtr> MeshPtrVector;