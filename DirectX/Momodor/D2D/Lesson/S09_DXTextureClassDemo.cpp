#include  "framework.h"
#include  "S09_DXTextureClassDemo.h"
#include  "Base/Camera.h"
#include  "Renders/Texture.h"
#include  "Renders/AnimationClip.h"

/////////////////////////////////////////////////
// 생성자
/////////////////////////////////////////////////
S09_DXTextureClassDemo::S09_DXTextureClassDemo()
{
	// 게임요소를 생성 : background, player etc
	m_pPlayer1 = new Texture(L"fire.png", L"Texture.fx");
	m_pPlayer1->SetPosition(20.0f, 100.0f);
	m_pPlayer1->SetScale(1.0f, 1.0f);
	m_pPlayer1->SetRotation(0.0f, 0.0f, 45.0f);

	// 일부부분
	m_pPlayer2 = new Texture(L"fire.png", L"Texture.fx", 15, 34, 29, 54);
	m_pPlayer2->SetPosition(-(Main->GetWidth()*0.5f), -100.0f);
	m_pPlayer2->SetScale(3.0f, 3.0f);

	// Animation Clip

	m_pAnimationClip = new AnimationClip();
	m_pAnimationClip->SetPosition(-200, -200);

	Texture *pImage = m_pAnimationClip->CreateTexture(L"fire.png", L"Texture.fx");

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
			m_pAnimationClip->AddFrame(pImage, startX, startY, endX, endY, 0.1f);
		}
	}


	





	//과거
	//m_pPlayer2[0] = new Texture(L"fire.png", L"Texture.fx", 15, 34, 29, 54);
	//.....
	//m_pPlayer2[14] = new Texture(L"fire.png", L"Texture.fx", 65, 94, 29, 54);

	printf("%d\n", Main->GetWidth());
}

S09_DXTextureClassDemo::~S09_DXTextureClassDemo()
{
	SAFE_DELETE(m_pPlayer1);
}
//////////////////////////////////////////////////
// 신에서의 Update
// 1. View, Projection Matrix
// 2. 해당Oject 의 View,Projection Matrix
// 3. 충돌 Check
//////////////////////////////////////////////////
void S09_DXTextureClassDemo::Update()
{
	// 1. View , Projection
	Matrix V, P;

	CAMERA->Update();
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();


	// 2. 키보드, Timer 위치가 변경하여야 할 경우

	static float time = 0.0f;
	static int   count = 0;

	time = time + TIMEMANAGER->Delta();
	if (time >= 0.1f)
	{
		count++;
		time = 0.0f;
		if (count == 15)
			count = 0;
	}

	const float width  = 43.0f;
	const float height = 67.0f;

	for (int y = 0; y < 3; y++)
	{
		for (int x = 0; x < 5; x++)
		{
			float startX = width  * x;
			float startY = height * y;
			float endX   = width  * (x+1);
			float endY   = height * (y+1);

			if (count == (y * 3) + x)
			{
				m_pPlayer2->SetOffset(startX, startY);
				m_pPlayer2->SetOffsetSize(width, height);
			}
		}
	}



	// 3. 해당Oject 의 View,Projection Matrix

	CAMERA->Update(V, P);
	m_pPlayer1->Update(V, P);  //  World Matrix 해당 Object에서 보낸다
	m_pPlayer2->Update(V, P);


	m_pAnimationClip->Update(V, P);

}
//////////////////////////////////////////////////
// 신에서의 Render
// 1. 해당Oject 의 Render()를 호출
//////////////////////////////////////////////////
void S09_DXTextureClassDemo::Render()
{
	m_pPlayer1->Render();
	m_pPlayer2->Render();
	m_pAnimationClip->Render();
	CAMERA->Render();
}


