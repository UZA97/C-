#include  "framework.h"
#include  "MoMo.h"
#include  "Collider/Collider.h"
#include  "Lesson/M01_Momodora.h"
#include "Object/MoMoEffect.h"

///////////////////////////////////////////////////////////
// 생성자
//////////////////////////////////////////////////////////
MoMo::MoMo()
{
	wstring  imgFile     = L"/Momodora/momo_idle.png";
	wstring  shaderFile  = L"Texture.fx";
	
	SetName("Macro");
	m_pAnimation = new Animation();
	
	// IDLE
	{
		AnimationClip *pClip = new AnimationClip();
		Texture *pTexture = pClip->CreateTexture(imgFile, L"Sprite.fx");

		float x = 480 / 5;
		float y = 192 / 2;

		for(int i =0;  i<5; i++)
		pClip->AddFrame(pTexture, x*i+24, y+40, x*i+x-24, y+y, 0.2f);
		m_pAnimation->AddAnimationClip(pClip);
	}

	// ATTACK
	{
		AnimationClip *pClip = new AnimationClip();

		imgFile = L"/Momodora/momo_attack.png";
		Texture *pTexture = pClip->CreateTexture(imgFile, L"Sprite.fx");

		float x = 384 / 4;
		float y = 192 / 2;

		for (int i = 0; i < 4; i++)
			pClip->AddFrame(pTexture, x*i + 24, y + 40, x*i + x - 24, y + y, 0.2f);
		m_pAnimation->AddAnimationClip(pClip);
	}

	// ATTACK2
	{
		AnimationClip *pClip = new AnimationClip();

		imgFile = L"/Momodora/momo_attack2.png";
		Texture *pTexture = pClip->CreateTexture(imgFile, L"Sprite.fx");

		float x = 384 / 4;
		float y = 192 / 2;

		for (int i = 0; i < 4; i++)
			pClip->AddFrame(pTexture, x*i + 24, y + 40, x*i + x - 24, y + y, 0.2f);
		m_pAnimation->AddAnimationClip(pClip);
	}

	// Run
	{
		AnimationClip *pClip = new AnimationClip();

		imgFile = L"/Momodora/momo_run.png";
		Texture *pTexture = pClip->CreateTexture(imgFile, L"Sprite.fx");

		float x = 768 / 8;
		float y = 192 / 2;

		for (int i = 0; i < 8; i++)
			pClip->AddFrame(pTexture, x*i + 24, y + 40, x*i + x - 24, y + y, 0.2f);
		m_pAnimation->AddAnimationClip(pClip);
	}

	// Jump
	{
		AnimationClip *pClip = new AnimationClip();

		imgFile = L"/Momodora/momo_jump.png";
		Texture *pTexture = pClip->CreateTexture(imgFile, L"Sprite.fx");

		float x = 1056 / 11;
		float y = 192 / 2;

		for (int i = 0; i < 11; i++)
			pClip->AddFrame(pTexture, x*i + 24, y + 40, x*i + x - 24, y + y, 0.2f);
		m_pAnimation->AddAnimationClip(pClip);
	}

	// falling
	{
		AnimationClip *pClip = new AnimationClip();

		imgFile = L"/Momodora/momo_jump.png";
		Texture *pTexture = pClip->CreateTexture(imgFile, L"Sprite.fx");

		float x = 266 / 3;
		float y = 192 / 2;

		for (int i = 0; i < 3; i++)
			pClip->AddFrame(pTexture, x*i + 24, y + 40, x*i + x - 24, y + y, 0.2f);
		m_pAnimation->AddAnimationClip(pClip);
	}
	// dead
	{
		AnimationClip *pClip = new AnimationClip();

		imgFile = L"/Momodora/momo_dead.png";
		Texture *pTexture = pClip->CreateTexture(imgFile, L"Sprite.fx");

		float x = 1536 / 16;
		float y = 192 / 2;

		for (int i = 0; i < 16; i++)
			pClip->AddFrame(pTexture, x*i + 24, y + 40, x*i + x - 24, y + y, 0.2f);
		m_pAnimation->AddAnimationClip(pClip);
	}
	// dead
	{
		AnimationClip *pClip = new AnimationClip();

		imgFile = L"/Momodora/momo_pray.png";
		Texture *pTexture = pClip->CreateTexture(imgFile, L"Sprite.fx");

		float x = 384 / 4;
		float y = 192 / 2;

		for (int i = 0; i < 4; i++)
			pClip->AddFrame(pTexture, x*i + 24, y + 40, x*i + x - 24, y + y, 0.2f);
		m_pAnimation->AddAnimationClip(pClip);
	}


	m_pCollider = new Collider();
	m_pCollider->SetParent(this);

	// function binding
	m_pFunctionState = bind(&MoMo::IdleState, this, placeholders::_1);

	// effect
	m_pMoMoEffect = new MoMoEffect();

}

MoMo::~MoMo()
{
	Delete();
}
void MoMo::Delete()
{
	SAFE_DELETE(m_pAnimation);
	SAFE_DELETE(m_pCollider);
}
void MoMo::Update(Matrix V, Matrix P)
{
	Vector2 position = GetPosition();

	m_pFunctionState(position);  

	if( m_bGround == false)
		position.y = position.y - m_MoveSpeed * TIMEMANAGER->Delta();

	// Scene이 변경되는 것을 Check
	if (IsChangeSceen(position))
		return;

	// effect

	m_pMoMoEffect->Update(V, P);
	printf("status = %d ground %d %f %f\n ", m_nState, this->m_bGround,position.x, position.y);



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

void MoMo::Render()
{
	m_pAnimation->Render();

	// effect
	m_pMoMoEffect->Render();


	if (GetCollisionCheck())
		m_pCollider->SetColor(1.0f, 0.0f, 0.0f);
	else
		m_pCollider->SetColor(0.0f, 1.0f, 0.0f);
	m_pCollider->Render();
}



Vector2 MoMo::GetRealTextureSize()
{
	Vector2 size = Vector2(m_pAnimation->GetTextureSize().x*GetScale().x,
		m_pAnimation->GetTextureSize().y*GetScale().y);

	return size;
}
void MoMo::SetGround(bool ground)
{
	m_bGround = ground;
	if (ground == true)
	{
		if( this->m_nState == eState::Falling)
			SetState(IsLeft() ? 1 : 0, eState::Idle);
	}
//		return;
//	SetState(IsLeft() ? 1 : 0, eState::Falling);

}
/////////////////////////////////////////////////////////////////
// Idle State
/////////////////////////////////////////////////////////////////
void MoMo::IdleState(Vector2& position)
{
	if (KEYMANAGER->Down(VK_LEFT))
		SetState(1,eState::Run);
	if (KEYMANAGER->Down(VK_RIGHT))
		SetState(0, eState::Run);
	if (KEYMANAGER->Down('A'))
	{
		SetState(IsLeft()?1:0, eState::JumpStart);
		m_Angle = VERT;
	}
}
/////////////////////////////////////////////////////////////////
// Run State
/////////////////////////////////////////////////////////////////
void MoMo::RunState(Vector2& position)
{
	if (KEYMANAGER->Up(VK_LEFT))
		SetState(1, eState::Idle);
	if (KEYMANAGER->Up(VK_RIGHT))
		SetState(0, eState::Idle);
	if (KEYMANAGER->Press(VK_LEFT) && LeftCornerCheck(position))
		position.x = position.x - 4;
		
	if (KEYMANAGER->Press(VK_RIGHT) && RightCornerCheck(position))
		position.x = position.x + 4;

	if (KEYMANAGER->Down('A'))
	{
		SetState(IsLeft() ? 1 : 0, eState::JumpStart);
		if(IsLeft())
		   m_Angle = LEFT_ANGLE;
		else
		  m_Angle = RIGHT_ANGLE;
	}
}

void MoMo::JumpState(Vector2 & position)
{
	if (m_Gravirty > m_SpeedY)  // fall상태
		SetState(IsLeft() ? 1 : 0, eState::Falling);
	else
	{
		m_bGround = false;

		if( LeftCornerCheck(position) && RightCornerCheck(position))
			position.x = position.x + cosf(m_Angle)*m_SpeedX;
		position.y = position.y + sinf(m_Angle)*m_SpeedY - m_Gravirty;
		m_Gravirty = m_Gravirty + 0.09f;
	}
}

void MoMo::FallingState(Vector2 & position)
{
	if (LeftCornerCheck(position) && RightCornerCheck(position))
		position.x = position.x + cosf(m_Angle)*m_SpeedX;
	position.y = position.y + sinf(m_Angle)*m_SpeedY - m_Gravirty;
	m_Gravirty = m_Gravirty + 0.09f;

	if( m_bGround == true)
		SetState(IsLeft() ? 1 : 0, eState::Idle);
}

void MoMo::SetState(int direction,eState state)
{

	switch (state)
	{
	case eState::Idle:
		       m_pFunctionState = bind(&MoMo::IdleState, this, placeholders::_1);
		       break;
	case eState::Run:
				m_pFunctionState = bind(&MoMo::RunState, this, placeholders::_1);
				break;
	case eState::JumpStart:
				m_Gravirty = 0.0f;
				m_bGround = false;
				m_pFunctionState = bind(&MoMo::JumpState, this, placeholders::_1);
				break;
	case eState::Falling:
				m_pFunctionState = bind(&MoMo::FallingState, this, placeholders::_1);
				break;

	}

	m_nState = state;
	if (direction == 1)
		SetRotation(0.0f, 180.0f, 0.0f);
	else
		SetRotation(0.0f,  0.0f, 0.0f);

}

bool MoMo::LeftCornerCheck(Vector2 & position)
{
	//M01_Momodora *pScene = (M01_Momodora*)SCENEMANAGER->GetScene("M01_Momodora");
	Vector2 size = m_pAnimation->GetRealTextureSize();
	Vector2 Left = Vector2(m_Left.x - size.x*0.5f, m_Left.y + size.y*0.5f);
	Vector2 pos = Vector2(position.x - size.x*0.5f, position.y);

	if( pos.x < Left.x)
	  return false;
	return true;
}

bool MoMo::RightCornerCheck(Vector2 & position)
{
	Vector2 size = m_pAnimation->GetRealTextureSize();
	Vector2 Right = Vector2(m_Right.x - size.x*0.5f, m_Right.y - size.y*0.5f);
	Vector2 pos = Vector2(position.x - size.x*0.5f, position.y);

	if (pos.x > Right.x)
		return false;
	return true;
}
////////////////////////////////////////////////////////////
//  Player의 현재 포지션에 따라 Scene변경을 Check
////////////////////////////////////////////////////////////
bool MoMo::IsChangeSceen(Vector2 & position)
{
	Scene *pScene = SCENEMANAGER->GetCurrentScene();
	Vector2 size = m_pAnimation->GetRealTextureSize();
	Vector2 Left  = Vector2(m_Left.x  + size.x*0.5, m_Left.y + size.y*0.5f);
	Vector2 Right = Vector2(m_Right.x - size.x*0.5, m_Right.y - size.y*0.5f);

	if (position.x < Left.x)
	{
		if (pScene->GetSceneName() == "M02_Momodora")
		{
			SCENEMANAGER->ChangeScene("M01_Momodora");
			SetPosition(880.0f, position.y);
			return true;
		}
			
	}
	if (position.x > Right.x)
	{
		if (pScene->GetSceneName() == "M01_Momodora")
		{
			SCENEMANAGER->ChangeScene("M02_Momodora");
			SetPosition(-880.0f, position.y);
			return true;
		}
			
	}

	return false;
}
