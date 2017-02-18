#include "renderable.h"

#include "Engine.h"
//#include "../Game.h"



/*

void CSpecularEnable::render()
{
	GetGame()->GetDevice()->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
	m_pRenderable->render();
	GetGame()->GetDevice()->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
}

void CTransparentEnable::render()
{
	CComPtr<IDirect3DDevice9> m_pDevice = GetGame()->GetDevice();

	m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_BLENDFACTOR);
	m_pDevice->SetRenderState( D3DRS_DESTBLEND,D3DBLEND_INVBLENDFACTOR);
	m_pDevice->SetRenderState( D3DRS_BLENDFACTOR,0xAAAAAAAA);

	//m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	//m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	//m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
	m_pRenderable->render();
	GetGame()->GetDevice()->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
}

*/