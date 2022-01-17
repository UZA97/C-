#include  "framework.h"
#include  "Marker.h"
#include  "Collider/Collider.h"
///////////////////////////////////////////////////////////
// »ı¼ºÀÚ
//////////////////////////////////////////////////////////
Marker::Marker()
{
	Init();
}
void Marker::Init()
{
	wstring  imgFile     = L"Macro.png";
	wstring  shaderFile  = L"Texture.fx";
	
	m_pAnimation = new Animation();


	SetName("Marker");


	Texture *pImage = m_pAnimation->CreateTexture(L"/Background/bullet.png", L"Texture.fx");

	// idle
	{
		AnimationClip *pClip = new AnimationClip();

		pClip->AddFrame(pImage, 37, 2, 55, 15, 0.3f);
		pClip->AddFrame(pImage, 117, 7, 132, 22, 0.3f);
		pClip->AddFrame(pImage, 37, 116, 53, 133, 0.3f);
		pClip->AddFrame(pImage, 204, 109, 220, 126, 0.3f);
		m_pAnimation->AddAnimationClip(pClip);
	}


	
	m_pCollider = new Collider();
	m_pCollider->SetParent(this);


}

Marker::Marker(Vector2 position, Vector2 scale)
{
	Init();
	SetPosition(position);
	SetScale(scale);
}

Marker::~Marker()
{
	SAFE_DELETE(m_pAnimation);
	SAFE_DELETE(m_pCollider);
}
void Marker::Update(Matrix V, Matrix P)
{
	Vector2 position = GetPosition();

	// Animation
	SetPosition(position);
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

void Marker::Render()
{
	m_pAnimation->Render();

	return;
	if(GetCollisionCheck())
		m_pCollider->SetColor(1.0f, 0.0f, 0.0f);
	else
		m_pCollider->SetColor(0.0f, 1.0f, 0.0f);

	m_pCollider->Render();
}


