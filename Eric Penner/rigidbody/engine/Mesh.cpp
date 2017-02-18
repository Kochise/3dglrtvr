#include "mesh.h"

#include "util\text.h"
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include "../game.h"

#include "Engine.h"

const DWORD VERTEX::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

Mesh::Mesh(char *name)
{
	filename = name;
	m_pMesh          = NULL; // Our mesh object in sysmem
	m_pMeshMaterials = NULL; // Materials for our mesh
	m_pMeshTextures  = NULL; // Textures for our mesh
	m_dwNumMaterials = 0L;   // Number of mesh materials

    m_fObjectRadius  = 0;
	m_vObjectCenter  = CVector3(0,0,0);

	m_Transform       = CMatrix::Identity();
}

//-----------------------------------------------------------------------------
// Name: InitGeometry()
// Desc: Load the mesh and build the material and texture arrays
//-----------------------------------------------------------------------------


HRESULT Mesh::init()
{
	HRESULT hr = S_OK;
	CComPtr<IDirect3DDevice9> m_pd3dDevice = GetGame()->GetDevice();
    CComPtr<ID3DXBuffer> pD3DXMtrlBuffer;
	CComPtr<ID3DXMesh>        pTempMesh; 
    CComPtr<ID3DXBuffer> pAdjacencyBuffer;

	char pathFileName[100];
	strcpy(pathFileName,"xfiles\\");
	strcat(pathFileName,filename);
    // Load the mesh from the specified file

    if( FAILED( hr = D3DXLoadMeshFromX(	pathFileName, D3DXMESH_MANAGED, m_pd3dDevice, 
									&pAdjacencyBuffer.p, &pD3DXMtrlBuffer.p, NULL, 
									&m_dwNumMaterials, &m_pMesh.p ) ) )
    {
		MessageBox(NULL, "Could not load x file", pathFileName, MB_OK);
        return E_FAIL;
    }


    DWORD dw32BitFlag = (m_pMesh->GetOptions() & D3DXMESH_32BIT);

    // perform simple cleansing operations on mesh
    if( FAILED( hr = D3DXCleanMesh( m_pMesh, (DWORD*)pAdjacencyBuffer->GetBufferPointer(), &pTempMesh, 
                                        (DWORD*)pAdjacencyBuffer->GetBufferPointer(), NULL ) ) )
    {
		MessageBox(NULL, "Could not clean x file", "Meshes.exe", MB_OK);
        return E_FAIL;
    }
    m_pMesh = pTempMesh;
	pTempMesh = NULL;

    //  Perform a weld to try and remove excess vertices like the model bigship1.x in the DX9.0 SDK (current model is fixed)
    //  Weld the mesh using all epsilons of 0.0f.  A small epsilon like 1e-6 works well too
    D3DXWELDEPSILONS Epsilons;
    memset(&Epsilons, 0, sizeof(D3DXWELDEPSILONS));
    if( FAILED( hr = D3DXWeldVertices( m_pMesh, 0, &Epsilons, 
                                       (DWORD*)pAdjacencyBuffer->GetBufferPointer(), 
                                       (DWORD*)pAdjacencyBuffer->GetBufferPointer(), NULL, NULL ) ) )
    {
		MessageBox(NULL, "Could not weld vertices", "Meshes.exe", MB_OK);
        return E_FAIL;
    }

    // verify validity of mesh for simplification
    if( FAILED( hr = D3DXValidMesh( m_pMesh, (DWORD*)pAdjacencyBuffer->GetBufferPointer(), NULL ) ) )
    {
		MessageBox(NULL, "Not valid x file for simplification", "Meshes.exe", MB_OK);
        return E_FAIL;
    }

	/* Not working
    if( FAILED( hr = m_pMesh->OptimizeInplace(NULL,(DWORD*)pAdjacencyBuffer->GetBufferPointer(),NULL,NULL,NULL) ))
    {
		MessageBox(NULL, "Can't optimize mesh", "Meshes.exe", MB_OK);
        return E_FAIL;
    }
	*/
	

    // We need to extract the material properties and texture names from the 
    // pD3DXMtrlBuffer
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    m_pMeshMaterials = new D3DMATERIAL9[m_dwNumMaterials];
    m_pMeshTextures  = new LPDIRECT3DTEXTURE9[m_dwNumMaterials];

    for( DWORD i=0; i<m_dwNumMaterials; i++ )
    {
        // Copy the material
        m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

        // Set the ambient color for the material (D3DX does not do this)
        m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;

        m_pMeshTextures[i] = NULL;
        if( d3dxMaterials[i].pTextureFilename != NULL && 
            lstrlen(d3dxMaterials[i].pTextureFilename) > 0 )
        {
            // Create the texture
			char pathTextureName[100];
			strcpy(pathTextureName,"textures\\");			
			strcat(pathTextureName, d3dxMaterials[i].pTextureFilename);
            if( FAILED( D3DXCreateTextureFromFile( m_pd3dDevice, 
                                                pathTextureName, 
                                                &m_pMeshTextures[i] ) ) )
            {
				//Take this out some meshes don't have textures
				MessageBox(NULL, "Could not load texture map", pathTextureName, MB_OK);
            }
//			m_pMeshTextures[i]->GenerateMipSubLevels();
        }
    }

	CComPtr<IDirect3DVertexBuffer9> pVertexBuffer;
    // Lock the vertex buffer, to generate a simple bounding sphere
    hr = m_pMesh->GetVertexBuffer( &pVertexBuffer.p );
    if( FAILED(hr) ) return E_FAIL;

    void* pVertices;
    hr = pVertexBuffer->Lock( 0, 0, &pVertices, D3DLOCK_NOSYSLOCK );
    if( FAILED(hr) ) return E_FAIL;

    hr = D3DXComputeBoundingSphere( (D3DXVECTOR3*)pVertices, m_pMesh->GetNumVertices(),
                                    D3DXGetFVFVertexSize(m_pMesh->GetFVF()),
                                    &m_vObjectCenter, &m_fObjectRadius );
	
	hr = D3DXComputeBoundingBox( (D3DXVECTOR3*)pVertices, m_pMesh->GetNumVertices(),
                                  D3DXGetFVFVertexSize(m_pMesh->GetFVF()),
								  &m_vObjectMin,&m_vObjectMax );

	pVertexBuffer->Unlock();

    if( FAILED(hr) || m_dwNumMaterials == 0 ) return E_FAIL;

    if (m_pMesh->GetFVF() != VERTEX::FVF)
    {
        hr = m_pMesh->CloneMeshFVF( dw32BitFlag|D3DXMESH_MANAGED, VERTEX::FVF, 
                                            m_pd3dDevice, &pTempMesh.p );
        if (FAILED(hr))return E_FAIL;
        D3DXComputeNormals( pTempMesh, NULL );

        m_pMesh = pTempMesh;
		pTempMesh = NULL;
    }
	

	return S_OK;
}
void Mesh::render()
{
	CComPtr<IDirect3DDevice9> pDevice = GetGame()->GetDevice();

	D3DXMATRIX OldWorld;
	D3DXMATRIX NewWorld;

	pDevice->GetTransform(D3DTS_WORLD,&OldWorld);
	NewWorld = m_Transform * OldWorld;
	pDevice->SetTransform(D3DTS_WORLD,&NewWorld);


	// Meshes are divided into subsets, one for each material. Render them in
    // a loop
    for( DWORD i=0; i<m_dwNumMaterials; i++ )
    {
		// Set the material and texture for this subset
        pDevice->SetMaterial( &m_pMeshMaterials[i] );
        if (m_pMeshTextures)
			pDevice->SetTexture( 0, m_pMeshTextures[i] );
        
        // Draw the mesh subset
        m_pMesh->DrawSubset( i );
    }

	pDevice->SetTransform(D3DTS_WORLD,&OldWorld);
}
//-----------------------------------------------------------------------------
// Name:
// Desc:
//-----------------------------------------------------------------------------
HRESULT Mesh::SetFVF( DWORD dwFVF )
{
	CComPtr<IDirect3DDevice9> pd3dDevice= GetGame()->GetDevice();
    CComPtr<ID3DXMesh> pTempSysMemMesh = NULL;

    if( m_pMesh )
    {
        if( FAILED( m_pMesh->CloneMeshFVF( D3DXMESH_SYSTEMMEM, dwFVF,
                                                 pd3dDevice, &pTempSysMemMesh ) ) )
            return E_FAIL;
    }

    if( pTempSysMemMesh ) m_pMesh = pTempSysMemMesh;

    // Compute normals in case the meshes have them
    if( m_pMesh )
        D3DXComputeNormals( m_pMesh, NULL );

    return S_OK;
}

HRESULT Mesh::initTextMesh(char *value)
{
	HRESULT hr;
	m_dwNumMaterials = 1;
	CComPtr<IDirect3DDevice9> pd3dDevice= GetGame()->GetDevice();
    m_pMeshMaterials = new D3DMATERIAL9;
    m_pMeshMaterials->Diffuse.r = m_pMeshMaterials->Ambient.r = 0.0f;
    m_pMeshMaterials->Diffuse.g = m_pMeshMaterials->Ambient.g = 0.0f;
    m_pMeshMaterials->Diffuse.b = m_pMeshMaterials->Ambient.b = 1.0f;
    m_pMeshMaterials->Diffuse.a = m_pMeshMaterials->Ambient.a = 1.0f;
    pd3dDevice->SetMaterial( m_pMeshMaterials );
	Text *text = Text::getInstance();
	hr = text->CreateD3DXTextMesh(value,&m_pMesh);
	return hr;
	
}

//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
void Mesh::cleanup()
{
    if( m_pMeshMaterials != NULL ) 
        delete[] m_pMeshMaterials;

    if( m_pMeshTextures )
    {
        for( DWORD i = 0; i < m_dwNumMaterials; i++ )
        {
            if( m_pMeshTextures[i] )
				m_pMeshTextures[i]->Release();
        }
        delete[] m_pMeshTextures;
    }
	m_pMesh = NULL;
}
Mesh::~Mesh(void)
{
}
