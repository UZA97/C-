#include "framework.h"
#include "MoMoEffect.h"
#include "Collider/Collider.h"

/////////////////////////////////////////////
// »ı¼ºÀÚ
/////////////////////////////////////////////
MoMoEffect::MoMoEffect()
{
	wstring strImage = L"/momodora/momo_effect_left.png";
	wstring strShader = L"00_Animation.fx";

	m_pAnimation = new Animation();
	
	
	// LEFT Attack
	{
		AnimationClip *pClip = new AnimationClip();
		Texture *pTexture = pClip->CreateTexture(strImage, strShader);
		float x = 192.0f;
		float y = 120.0f;
		for (int i = 0; i < 4; i++)
			pClip->AddFrame(pTexture, x*i, y, x*i + x, y + y, 0.04f);
		m_pAnimation->AddAnimationClip(pClip);
	//	pClip->SetOnce();
	}

	// RIGHT Attack
	{
		AnimationClip *pClip = new AnimationClip();
		strImage = L"/momodora/momo_effect_right.png";
		Texture *pTexture = pClip->CreateTexture(strImage, strShader);
		float x = 192.0f;
		float y = 120.0f;
		for (int i = 0; i < 4; i++)
			pClip->AddFrame(pTexture, x*i, y, x*i + x, y + y, 0.04f);
		m_pAnimation->AddAnimationClip(pClip);
		pClip->SetOnce();
	}

	// collider
	m_pCollider = new Collider();
	m_pCollider->SetParent(this);

}

MoMoEffect::~MoMoEffect()
{
}
void MoMoEffect::Update(Matrix V, Matrix P)
{
	Vector2 position = GetPosition();


	// Animation
	SetPosition(position);
//	m_pAnimation->SetPlay((UINT)this->m_nState);
	m_pAnimation->SetPosition(position);
	m_pAnimation->SetScale(m_Scale);
	m_pAnimation->SetRotation(m_Rotation);
	m_pAnimation->Update(V, P);

	// Collider
	m_pCollider->SetPosition(position);
	m_pCollider->SetScale(m_Scale.x*m_pAnimation->GetTextureSize().x,
		m_Scale.y*m_pAnimation->GetTextureSize().y);
	m_pCollider->SetRotation(m_Rotation);
	m_pCollider->Update(V, P);


}

void MoMoEffect::Render()
{
	m_pAnimation->Render();
}


