#include  "framework.h"
#include  "Marco.h"
#include  "Collider/Collider.h"
///////////////////////////////////////////////////////////
// 생성자
//////////////////////////////////////////////////////////
Marco::Marco()
{
	wstring  imgFile     = L"Macro.png";
	wstring  shaderFile  = L"Texture.fx";
	
	SetName("Macro");
	m_pAnimation = new Animation();
	
	Texture *pTexture = m_pAnimation->CreateTexture(L"Marco.png", L"Sprite.fx");

	// IDLE
	{
		AnimationClip *pClip = new AnimationClip();

		pClip->AddFrame(pTexture, 4, 2, 34, 40, 0.1f);
		pClip->AddFrame(pTexture, 35, 2, 64, 40, 0.1f);
		pClip->AddFrame(pTexture, 64, 2, 94, 40, 0.1f);
		m_pAnimation->AddAnimationClip(pClip);
	}

	// WALK
	{
		AnimationClip *pClip = new AnimationClip();

		//Texture *pTexture = pClip->CreateTexture(L"Marco.png", L"Sprite.fx");
		pClip->AddFrame(pTexture, 0, 600, 32, 640, 0.1f);
		pClip->AddFrame(pTexture, 33, 600, 64, 640, 0.1f);
		pClip->AddFrame(pTexture, 65, 600, 96, 640, 0.1f);
		pClip->AddFrame(pTexture, 97, 600, 124, 640, 0.1f);
		pClip->AddFrame(pTexture, 125, 600, 154, 640, 0.1f);
		pClip->AddFrame(pTexture, 158, 600, 188, 640, 0.1f);
		pClip->AddFrame(pTexture, 191, 600, 222, 640, 0.1f);
		pClip->AddFrame(pTexture, 224, 600, 258, 640, 0.1f);
		pClip->AddFrame(pTexture, 259, 600, 294, 640, 0.1f);
		pClip->AddFrame(pTexture, 295, 600, 326, 640, 0.1f);
		pClip->AddFrame(pTexture, 329, 600, 360, 640, 0.1f);
		pClip->AddFrame(pTexture, 362, 600, 393, 640, 0.1f);
		m_pAnimation->AddAnimationClip(pClip);
	}
	
	m_pCollider = new Collider();
	m_pCollider->SetParent(this);


}

Marco::~Marco()
{
	SAFE_DELETE(m_pAnimation);
	SAFE_DELETE(m_pCollider);
}
void Marco::Update(Matrix V, Matrix P)
{
	Vector2 position = GetPosition();
	bool    bMove = false;

	// Key or Mouse 이동
	if (KEYMANAGER->Press('W'))
	{
		bMove = true;
		position.y = position.y + m_MoveSpeed *TIMEMANAGER->Delta();
	}
	if (KEYMANAGER->Press('S'))
	{
		bMove = true;
		position.y = position.y - m_MoveSpeed * TIMEMANAGER->Delta();
	}


	if (KEYMANAGER->Press('D'))
	{
		bMove = true;
		position.x = position.x + m_MoveSpeed * TIMEMANAGER->Delta();
		SetRotation(0.0f, 0.0f, 0.0f);
	}

	if (KEYMANAGER->Press('A'))
	{
		bMove = true;
		position.x = position.x - m_MoveSpeed * TIMEMANAGER->Delta();
		SetRotation(0.0f, 180.0f, 0.0f);
	}


	if (bMove)
		m_nState = eState::WALK;
	else
		m_nState = eState::IDLE;


	// Animation
	SetPosition(position);
	m_pAnimation->SetPlay((UINT)this->m_nState);
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

void Marco::Render()
{
	m_pAnimation->Render();
	if (GetCollisionCheck())
		m_pCollider->SetColor(1.0f, 0.0f, 0.0f);
	else
		m_pCollider->SetColor(0.0f, 1.0f, 0.0f);
	m_pCollider->Render();
}


