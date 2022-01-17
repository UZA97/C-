#include  "framework.h"
#include  "Fire.h"
#include  "Collider/Collider.h"
///////////////////////////////////////////////////////////
// »ý¼ºÀÚ
//////////////////////////////////////////////////////////
Fire::Fire()
{
	wstring  imgFile     = L"Macro.png";
	wstring  shaderFile  = L"Texture.fx";
	
	m_pAnimation = new Animation();
	AnimationClip *pClip = new AnimationClip();

	SetName("Fire");


	Texture *pImage = pClip->CreateTexture(L"fire.png", L"Texture.fx");

	const float width = 43.0f;
	const float height = 67.0f;

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			float startX = width * x;
			float startY = height * y;
			float endX = width * (x + 1);
			float endY = height * (y + 1);
			pClip->AddFrame(pImage, startX, startY, endX, endY, 0.1f);
		}
	}
	m_pAnimation->AddAnimationClip(pClip);
	m_pCollider = new Collider();
	m_pCollider->SetParent(this);


}

Fire::~Fire()
{
	SAFE_DELETE(m_pAnimation);
	SAFE_DELETE(m_pCollider);
}
void Fire::Update(Matrix V, Matrix P)
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

void Fire::Render()
{
	m_pAnimation->Render();

	if(GetCollisionCheck())
		m_pCollider->SetColor(1.0f, 0.0f, 0.0f);
	else
		m_pCollider->SetColor(0.0f, 1.0f, 0.0f);

	m_pCollider->Render();
}


