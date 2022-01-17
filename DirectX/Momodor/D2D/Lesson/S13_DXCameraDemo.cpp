#include  "framework.h"
#include  "S13_DXCameraDemo.h"
#include  "Object/Marco.h"

////////////////////////////////////////////////////////////
//  생성자
///////////////////////////////////////////////////////////
S13_DXCameraDemo::S13_DXCameraDemo()
{
	wstring strImageFie   = L"/Stage/stage.png";
	wstring strShaderFile = L"Sprite.fx";

	m_pBackground = new Texture(strImageFie, strShaderFile);
	m_pBackground->SetScale(2.5f, 2.5f);
//	m_pBackground->SetPosition(0.0f, -50.0f);

	m_pMarco1 = new Marco();
	m_pMarco1->SetScale(2.5f, 2.5f);

	m_pMarco2 = new Marco();
	m_pMarco2->SetScale(2.5f, 2.5f);
	m_pMarco2->SetPosition(-100.0f, -100.0f);
	m_pMarco2->SetMoveSpeed(300.0f);

	CAMERA->SetObject(m_pMarco1);
	CAMERA->SetOffset(100.0f, 100.0f);
}

S13_DXCameraDemo::~S13_DXCameraDemo()
{
	SAFE_DELETE(m_pBackground);
	SAFE_DELETE(m_pMarco1);
	SAFE_DELETE(m_pMarco2);
}



void S13_DXCameraDemo::Update()
{
	//1 . View,Pojection
	Matrix V, P;
	CAMERA->Update();
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();

	//2. 키보드, collision

	if (KEYMANAGER->Down('1'))
	{
		CAMERA->SetObject(m_pMarco1);
		CAMERA->SetOffset(100.0f, 100.0f);
	}

	if (KEYMANAGER->Down('2'))
	{
		CAMERA->SetObject(m_pMarco2);
		CAMERA->SetOffset(-100.0f, -100.0f);
	}
	if (KEYMANAGER->Down('3'))
		CAMERA->SetObject(NULL);
	if (KEYMANAGER->Down('4'))
		CAMERA->SetEffect();


	//3. 신에 있는 해당 Object Update()

	m_pBackground->Update(V, P);
	CAMERA->Update(V, P);
	m_pMarco1->Update(V, P);
	m_pMarco2->Update(V, P);


}

void S13_DXCameraDemo::Render()
{
	m_pBackground->Render();
	m_pMarco1->Render();
	m_pMarco2->Render();
	CAMERA->Render();

	// 2D Render
	DirectWrite::GetDC()->BeginDraw();
	{
		// example 문자 draw
		wstring str2 = L"Space Key를 누르시면 다시게임이 ";

		DirectWrite::RenderText(str2, Vector2(100, 100), 255, 0, 0, 80.f);

	}
	DirectWrite::GetDC()->EndDraw();


}


