#include  "framework.h"
#include  "S11_DXAniamtionDemo.h"
#include  "Object/Background.h"
#include  "Renders/Texture.h"
#include  "Renders/Animation.h"
#include  "Renders/AnimationClip.h"
#include  "Object/Marco.h"

/////////////////////////////////////////////////////
// 생성자
////////////////////////////////////////////////////
S11_DXAniamtionDemo::S11_DXAniamtionDemo()
{
	m_pBackground = new Background();
	m_pBackground->SetPosition(Vector2(0.0f, -300.0f));

	wstring   strImage = L"/mario.png";
	wstring   shaderFile = L"Texture.fx";


	strImage = L"/Wizard.png";
	m_pWizard = new Texture(strImage, shaderFile);
	m_pWizard->SetPosition(-322.0f, -194.0f);
	m_pWizard->SetScale(3.0f, 3.0f);


	m_pAnimation = new Animation();
	m_pAnimation->SetScale(2.5f, 2.5f);
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

	m_pMarco = new Marco();
	m_pMarco->SetScale(1.5f, 1.5f);
	m_pMarco->SetPosition(-100.0f, 100.0f);
}

S11_DXAniamtionDemo::~S11_DXAniamtionDemo()
{
}
////////////////////////////////////////////////////
// Scene Update
//  1. View, Projection Matrix
//  2.  Collision
//  3. Object별로 Update()를 실시
////////////////////////////////////////////////////
void S11_DXAniamtionDemo::Update()
{
	Matrix V, P;

	//1. View, Projection Matrix 
	CAMERA->Update();
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();

	//2. Collsion, 키보드......

	Vector2 position = Mouse->GetPosition();
	CAMERA->WCtoVC(position);

	if (Mouse->Down(0))
		printf("1 Button Down\n");
	if (Mouse->DoubleClick(2))
		printf("3 Button DoubleClick\n");

	if (KEYMANAGER->Down('D'))
		m_pAnimation->SetPlay(1);





//	printf("VC : %f %f\n", position.x, position.y);



	//3. 
	m_pBackground->Update(V, P);
	m_pWizard->Update(V, P);
	CAMERA->Update(V, P);
	m_pAnimation->Update(V, P);
	m_pMarco->Update(V, P);
}

void S11_DXAniamtionDemo::Render()
{
	m_pBackground->Render();

	// Render전에 Debug 용으로  ImGUI 를 이용




	m_pWizard->Render();

	CAMERA->Render();
	m_pAnimation->Render();
	m_pMarco->Render();
}


