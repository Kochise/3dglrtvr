////////////////////////////////////////////////////////////////////////////////
//
//	DXApp.cpp - DirectX Application Support Implementation
//
//	Project:	3D Jello
//
//	Author:		Asen L. Kovatchev (aceh@bulinfo.net)
//	Created:	Nov 19, 1999
//
////////////////////////////////////////////////////////////////////////////////



#include "StdMain.h"
#include "DXApp.h"



//////////////////////////////////////////////////////////////////////
//	Construction/Destruction

CDXApp::CDXApp()
{
	m_pDD				= NULL;
	m_pD3D				= NULL;
	m_pd3dDevice		= NULL;

	m_vecVelocity			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecAngularVelocity	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecCylVel				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_vecJello				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_fSpeed		= 5.0f;
	m_fAngularSpeed	= 1.0f;

	m_gr = 0.0f;

	m_fBoxX	= 80.0f;
	m_fBoxY = 40.0f;
	m_fBoxZ = 100.0f;

	m_bShowHelp = FALSE;
}


CDXApp::~CDXApp()
{
	// Release the DDraw and D3D objects used by the app
	if (m_pD3D)	m_pD3D->Release();

    // Do a safe check for releasing the D3DDEVICE. RefCount should be zero.
    if (m_pd3dDevice)
        if (0 < m_pd3dDevice->Release())
			return; //E_FAIL;

	// Do a safe check for releasing DDRAW. RefCount should be zero.
    if (m_pDD)
        if (0 < m_pDD->Release())
			return;

	D3DXUninitialize();

	m_pd3dDevice     = NULL;
	m_pD3D           = NULL;
	m_pDD            = NULL;
}



//////////////////////////////////////////////////////////////////////
//	Direct3D Initialization

HRESULT CDXApp::InitD3D(HWND hWnd)
{
	HRESULT hr;

	// Initialize Direct3DX Library
	hr = D3DXInitialize();

    if (FAILED( hr ))
	{
        return hr;
	}

	// Initialize DirectX object instances
	hr = D3DXCreateContextEx(
		D3DX_DEFAULT,
		0 /* D3DX_CONTEXT_FULLSCREEN */, hWnd, NULL, D3DX_DEFAULT, 0,
		D3DX_DEFAULT, 0, 1, D3DX_DEFAULT, D3DX_DEFAULT,
		D3DX_DEFAULT, &m_pd3dx);

	if (FAILED( hr ))
	{
		return hr;
	}

	// Obtain references to DirectX objects
	m_pd3dDevice = m_pd3dx->GetD3DDevice();
	if (m_pd3dDevice == NULL)
	{
		return E_FAIL;
	}

	m_pD3D = m_pd3dx->GetD3D();
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	m_pDD = m_pd3dx->GetDD();
	if (m_pDD == NULL)
	{
		return E_FAIL;
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////
//	Scene Objects Initalization

//
// Light source
//

D3DXCOLOR g_colorLight(1.0f, 0.95f, 0.8f, 1.0f);
D3DXVECTOR4 g_vecLight(0.1f, -1.0f, -1.5f, 0.0f);


//
// Background
//

D3DXCOLOR g_colorClear (0.4f, 0.5f, 0.8f, 1.0f);

void CDXApp::InitSceneObjects()
{
    // Setup render states
    m_pd3dx->SetClearColor(g_colorClear);

    m_pd3dDevice->LightEnable(0, TRUE);
    m_pd3dDevice->SetRenderState(D3DRENDERSTATE_AMBIENT, 0xffffffff);
    m_pd3dDevice->SetRenderState(D3DRENDERSTATE_ZENABLE, D3DZB_TRUE);


    // Create light
    D3DLIGHT7 d3dLight;
    ZeroMemory(&d3dLight, sizeof(d3dLight));

    d3dLight.dltType = D3DLIGHT_DIRECTIONAL;
    d3dLight.dcvDiffuse.r = g_colorLight.r;
    d3dLight.dcvDiffuse.g = g_colorLight.g;
    d3dLight.dcvDiffuse.b = g_colorLight.b;
    d3dLight.dcvDiffuse.a = g_colorLight.a;
    d3dLight.dcvSpecular.r = 1.0f;
    d3dLight.dcvSpecular.g = 1.0f;
    d3dLight.dcvSpecular.b = 1.0f;
    d3dLight.dcvSpecular.a = 0.0f;
    d3dLight.dvDirection.x = g_vecLight.x;
    d3dLight.dvDirection.y = g_vecLight.y;
    d3dLight.dvDirection.z = g_vecLight.z;

    m_pd3dDevice->SetLight(0, &d3dLight);

    // Create materials
    ZeroMemory(&m_d3dMaterial1, sizeof(m_d3dMaterial1));
    m_d3dMaterial1.dcvDiffuse.r = 0.70f;
    m_d3dMaterial1.dcvDiffuse.g = 0.70f;
    m_d3dMaterial1.dcvDiffuse.b = 0.70f;
    m_d3dMaterial1.dcvDiffuse.a = 0.25f;
    m_d3dMaterial1.dcvAmbient.r = g_colorLight.r;
    m_d3dMaterial1.dcvAmbient.g = g_colorLight.g;
    m_d3dMaterial1.dcvAmbient.b = g_colorLight.b;
    m_d3dMaterial1.dcvAmbient.r = 0.0f;
    m_d3dMaterial1.dcvAmbient.g = 0.4f;
    m_d3dMaterial1.dcvAmbient.b = 0.6f;
    m_d3dMaterial1.dcvSpecular.r = 1.0f;
    m_d3dMaterial1.dcvSpecular.g = 1.0f;
    m_d3dMaterial1.dcvSpecular.b = 1.0f;
    m_d3dMaterial1.dcvSpecular.a = 1.0f;
    m_d3dMaterial1.dvPower = 50.0f;

    ZeroMemory(&m_d3dMaterial2, sizeof(m_d3dMaterial2));
    m_d3dMaterial2.dcvDiffuse.r = 0.70f;
    m_d3dMaterial2.dcvDiffuse.g = 0.70f;
    m_d3dMaterial2.dcvDiffuse.b = 0.70f;
    m_d3dMaterial2.dcvDiffuse.a = 0.25f;
    m_d3dMaterial2.dcvAmbient.r = g_colorLight.r;
    m_d3dMaterial2.dcvAmbient.g = g_colorLight.g;
    m_d3dMaterial2.dcvAmbient.b = g_colorLight.b;
    m_d3dMaterial2.dcvAmbient.r = 1.0f;
    m_d3dMaterial2.dcvAmbient.g = 1.0f;
    m_d3dMaterial2.dcvAmbient.b = 1.0f;
    m_d3dMaterial2.dcvSpecular.r = 1.0f;
    m_d3dMaterial2.dcvSpecular.g = 1.0f;
    m_d3dMaterial2.dcvSpecular.b = 1.0f;
    m_d3dMaterial2.dcvSpecular.a = 1.0f;
    m_d3dMaterial2.dvPower = 50.0f;

	// Load textures
	D3DX_SURFACEFORMAT sf;

	sf = D3DX_SF_UNKNOWN;
	D3DXCreateTextureFromFile( m_pd3dDevice, NULL, NULL, NULL, &sf, NULL, &m_ptxtBricks,
							   NULL, "bricks01.bmp", D3DX_FT_DEFAULT );
	sf = D3DX_SF_UNKNOWN;
	D3DXCreateTextureFromFile( m_pd3dDevice, NULL, NULL, NULL, &sf, NULL, &m_ptxtJello,
							   NULL, "tentacle.bmp", D3DX_FT_DEFAULT );
	sf = D3DX_SF_UNKNOWN;
	D3DXCreateTextureFromFile( m_pd3dDevice, NULL, NULL, NULL, &sf, NULL, &m_ptxtObjects,
							   NULL, "ground1.bmp", D3DX_FT_DEFAULT );

	// Create test shape
	D3DXMatrixTranslation( &m_matWorld, 0.0f, m_fBoxY / 2.0f, 0.0f );
    m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, m_matWorld);
	D3DXCreateBox( m_pd3dDevice, m_fBoxX, m_fBoxY, m_fBoxZ, 1, &m_pd3dxRoom );
	D3DXMatrixIdentity( &m_matWorld );
    m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_WORLD, m_matWorld);

    // Misc stuff
	D3DXMatrixTranslation(&m_matPosition, -8.0f, -m_fBoxY, m_fBoxZ / 2.0f);
	D3DXMatrixPerspectiveFov(&m_matProjection, D3DXToRadian(60.0f), 3.0f / 4.0f, 0.1f, 1000.0f);
	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_VIEW, m_matView);
	m_pd3dDevice->SetTransform(D3DTRANSFORMSTATE_PROJECTION, m_matProjection);

	memset(m_bKey, 0x00, sizeof(m_bKey));

	// Init jello box
	m_JelloBall.CreateBox( 0.0f, 1.0f, 0.0f );
}

void CDXApp::DeleteSceneObjects()
{

}


//////////////////////////////////////////////////////////////////////
//	Rendering

void CDXApp::ShowFrame()
{
	// Let the Direct3D extension to manage the page swapping
	m_pd3dx->UpdateFrame(0);
}


void CDXApp::FrameMove(float fTimeKey)
{
	//
	// Process keyboard input
	//

	D3DXVECTOR3 vecT(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vecR(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vecC(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vecJ(0.0f, 0.0f, 0.0f);

	if (m_bKey['1'])
		vecC.x -= 0.3f;

	if (m_bKey['2'])
		vecC.x += 0.3f;

	if (m_bKey[VK_SUBTRACT])
		vecC.y += 0.5f;

	if (m_bKey[VK_ADD])
		vecC.y -= 0.5f;

	if (m_bKey[VK_NUMPAD1] || m_bKey[VK_END])
		vecT.x -= 1.0f; // Slide Left

	if (m_bKey[VK_NUMPAD3] || m_bKey[VK_NEXT])
		vecT.x += 1.0f; // Slide Right

	if (0)
		vecT.y -= 1.0f; // Slide Down

	if (m_bKey[' '])
		vecJ.y += 0.2f; // Jump

	if (m_bKey['Q'])
		vecJ.z -= 0.05f; // Push Jello

	if (m_bKey['A'])
		vecJ.z += 0.05f; // Pull Jello

	if (m_bKey['O'])
		vecJ.x -= 0.05f; // Move Jello Left

	if (m_bKey['P'])
		vecJ.x += 0.05f; // Move Jello Right

	if (m_bKey[VK_UP])
		vecT.z -= 2.0f; // Move Forward

	if (m_bKey[VK_DOWN])
		vecT.z += 2.0f; // Move Backward

	if (m_bKey[VK_NUMPAD8])
		vecR.x -= 1.0f; // Pitch Down

	if (m_bKey[VK_NUMPAD2])
		vecR.x += 1.0f; // Pitch Up

	if (m_bKey[VK_NUMPAD6] || m_bKey[VK_RIGHT])
		vecR.y -= 1.0f; // Turn Right

	if (m_bKey[VK_NUMPAD4] || m_bKey[VK_LEFT])
		vecR.y += 1.0f; // Turn Left

	if (m_bKey[VK_NUMPAD9] || m_bKey[VK_PRIOR])
		vecR.z -= 2.0f; // Roll CW

	if (m_bKey[VK_NUMPAD7] || m_bKey[VK_HOME])
		vecR.z += 2.0f; // Roll CCW
	
	m_vecVelocity = m_vecVelocity * 0.9f + vecT * 0.2f;
	m_vecAngularVelocity = m_vecAngularVelocity * 0.95f + vecR * 0.05f;
	m_vecCylVel = m_vecCylVel * 0.9f + vecC * 0.1f;
	m_vecJello = m_vecJello * 0.9f + vecJ * 0.1f;
	
    //
    // Update position and view matricies
    //

	m_JelloBall.MoveJello( m_vecJello.x, m_vecJello.y, m_vecJello.z );
	m_JelloBall.MoveCyl( m_vecCylVel.x, m_vecCylVel.y, m_vecCylVel.z );

    D3DXMATRIX matT, matR;

    vecT = m_vecVelocity * 0.05f * m_fSpeed;
    vecR = m_vecAngularVelocity * 0.05f * m_fAngularSpeed;

    D3DXMatrixTranslation( &matT, vecT.x, vecT.y, vecT.z );
    D3DXMatrixMultiply( &m_matPosition, &matT, &m_matPosition );
	
    D3DXMatrixRotationYawPitchRoll( &matR, vecR.y, vecR.x, vecR.z );
    D3DXMatrixMultiply( &m_matPosition, &matR, &m_matPosition );

	m_gr = m_gr * 0.97f + 0.01f;
	m_matPosition.m31 -= m_gr;

	float f;
	f = (m_fBoxX / 2.0f - 1.0f);
	if (m_matPosition.m30 < -f) m_matPosition.m30 = -f;
	if (m_matPosition.m30 >  f) m_matPosition.m30 =  f;
	f = (m_fBoxY / 2.0f - 1.0f);
	if (m_matPosition.m31 < -3.0f) 
	{
		m_matPosition.m31 = -3.0f;
		m_gr = -m_gr;
	}
	if (m_matPosition.m31 >  f) m_matPosition.m31 =  f;
	f = (m_fBoxZ / 2.0f - 1.0f);
	if (m_matPosition.m32 < -f) m_matPosition.m32 = -f;
	if (m_matPosition.m32 >  f) m_matPosition.m32 =  f;

    D3DXMatrixInverse(&m_matView, NULL, &m_matPosition);

	D3DXMatrixIdentity( &matT );
    m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_VIEW, m_matView );
    m_pd3dDevice->SetTransform( D3DTRANSFORMSTATE_WORLD, matT );

	m_JelloBall.FrameMove();

	return;
}

void CDXApp::RenderScene()
{
    // Begin the scene
    if( FAILED( m_pd3dDevice->BeginScene() ) )
		return; // E_FAIL;

    // Clear the viewport to a blue color (dwColor = 0x000000ff). For apps with
	// z-buffers, this call can also clear the z-buffer (if present) as well.
	m_pd3dx->Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER);

    m_pd3dDevice->SetTextureStageState(0, D3DTSS_MINFILTER, D3DTFN_LINEAR);
    m_pd3dDevice->SetTextureStageState(0, D3DTSS_MAGFILTER, D3DTFG_LINEAR);
    m_pd3dDevice->SetTextureStageState(0, D3DTSS_MIPFILTER, D3DTFP_POINT);
    m_pd3dDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_pd3dDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CW);
	m_pd3dDevice->LightEnable(0, TRUE);
	m_pd3dDevice->SetTexture(0, NULL);
	m_pd3dDevice->SetMaterial(&m_d3dMaterial1);
//	m_JelloBall.Draw( m_pd3dDevice );

	m_pd3dDevice->SetRenderState(D3DRENDERSTATE_CULLMODE, D3DCULL_CCW);
	m_pd3dDevice->LightEnable(0, FALSE);
    m_pd3dDevice->SetMaterial(&m_d3dMaterial2);
	m_pd3dDevice->SetTexture(0, m_ptxtBricks);
	m_pd3dxRoom->Draw();
	m_pd3dDevice->SetTexture(0, m_ptxtJello);
	m_JelloBall.Draw( m_pd3dDevice );
	m_pd3dDevice->SetTexture(0, m_ptxtObjects);
	m_JelloBall.DrawCyl( m_pd3dDevice );

	// Print debug
	char str[1000];
	sprintf( str,
			 "Jello Vertices: %d\n"
			 "Jello Links: %d\n"
			 "Jello Triangles: %d\n"
			 "Cyl Vertices: %d\n"
			 "Cyl Triangles: %d\n",

			 m_JelloBall.m_nVerts,
			 m_JelloBall.m_nLinks,
			 m_JelloBall.m_nTriLinks / 3,
			 m_JelloBall.m_nCylVerts,
			 m_JelloBall.m_nCylLinks / 3);

#ifdef _DEBUG
	m_pd3dx->DrawDebugText(0.01f, 0.01f, 0xff7fcf7f, str);
#endif

	m_pd3dx->DrawDebugText(0.8f, 0.05f, 0xff7fcf7f, "F1 - Help");

	if (m_bShowHelp)
	{
		strcpy(str,	"[1]\t - Roll Cylinder Left\n"
					"[2]\t - Roll Cylinder Right\n"
					"[-]\t - Move Cylinder Up\n"
					"[+]\t - Move Cylinder Down\n"
					"\n"
					"[Space]\t - Jello Jetpack ;-)\n"
					"[Q]\t - Push Jello\n"
					"[A]\t - Pull Jello\n"
					"[O]\t - Roll Jello Left\n"
					"[P]\t - Roll Jello Right\n"
					"\n"
					"[Up]\t - Move Camera Forward\n"
					"[Down]\t - Move Camera Backward\n"
					"[Left]\t - Turn Camera Left\n"
					"[Right]\t - Turn Camera Right\n"
					"[End]\t - Strafe Camera Left\n"
					"[PgDn]\t - Strafe Camera Right\n"
					"\n" );

		m_pd3dx->DrawDebugText(0.104f, 0.104f, 0x00000000, str);
		m_pd3dx->DrawDebugText(0.1f, 0.1f, 0xffcfffcf, str);
	}

    // End the scene.
    m_pd3dDevice->EndScene();
}
