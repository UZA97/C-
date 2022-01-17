#include  "framework.h"
#include  "S14_DXAABBDemo.h"
#include  "Object/Marco.h"
#include  "Object/Fire.h"
#include  "Object/Marker.h"
#include  "Object/EventZone.h"
#include  "Collider/Collider.h"

////////////////////////////////////////////////////////////
//  생성자
///////////////////////////////////////////////////////////
S14_DXAABBDemo::S14_DXAABBDemo()
{
	wstring strImageFie   = L"/Stage/City.png";
	wstring strShaderFile = L"Sprite.fx";

	Main->SetWindowSize(800, 600);

	m_pBackground = new Texture(strImageFie, strShaderFile);
	m_pBackground->SetScale(3.5f, 3.5f);
	m_pBackground->SetPosition(460.0f, 100.0f);

	m_pMarco = new Marco();
	m_pMarco->SetScale(2.5f, 2.5f);
	m_pMarco->SetPosition(-100, -50.0f);
	m_pMarco->SetMoveSpeed(300.0f);


	m_pFire = new Fire();
	m_pFire->SetScale(2.5f, 2.5f);
	m_pFire->SetPosition(200, -50.0f);
	m_pFire->SetRotation(0.0f, 0.0f, 45.0f);

	m_pMarker = new Marker();
	//m_pMarker->SetScale(2.5f, 2.5f);
	m_pMarker->SetPosition(-200.0f, 135.0f);

	m_pEventZone = new EventZone();
	m_pEventZone->SetScale(200.0f, 40.f);
	m_pEventZone->SetPosition(70.0f, -170.0f);



//	CAMERA->SetObject(m_pMarco1);
//	CAMERA->SetOffset(100.0f, 100.0f);
}

S14_DXAABBDemo::~S14_DXAABBDemo()
{
	SAFE_DELETE(m_pBackground);
	SAFE_DELETE(m_pMarco);
	SAFE_DELETE(m_pFire);
	SAFE_DELETE(m_pMarker);
	SAFE_DELETE(m_pEventZone);
}



void S14_DXAABBDemo::Update()
{
	//1 . View,Pojection
	Matrix V, P;
	CAMERA->Update();
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();

	//2. 키보드, collision

	bCheck1 = Collider::ClippingPolygon(m_pMarco->GetCollider(), m_pFire->GetCollider());


  //  bCheck1 = Collider::AaBB(m_pMarco->GetCollider(), m_pFire->GetCollider());
	if (bCheck1)
	{
		m_pMarco->SetCollisionCheck(true);
		m_pFire->SetCollisionCheck(true);
	}
	else
	{
		m_pMarco->SetCollisionCheck(false);
		m_pFire->SetCollisionCheck(false);
	}

	bCheck2 = Collider::AaBB(m_pMarco->GetCollider(), m_pMarker->GetCollider());
	if (bCheck2)
	{
		m_pMarco->SetCollisionCheck(true);
		m_pMarker->SetCollisionCheck(true);
	}
	else
	{
		m_pMarco->SetCollisionCheck(false);
		m_pMarker->SetCollisionCheck(false);
	}

	bCheck3 = Collider::AaBB(m_pMarco->GetCollider(), m_pEventZone->GetCollider());
	if (bCheck3)
	{
		m_pMarco->SetCollisionCheck(true);
		m_pEventZone->SetCollisionCheck(true);
	}
	else
	{
		m_pMarco->SetCollisionCheck(false);
		m_pEventZone->SetCollisionCheck(false);
	}




	//3. 신에 있는 해당 Object Update()

	m_pBackground->Update(V, P);
	CAMERA->Update(V, P);
	m_pMarco->Update(V, P);
	m_pFire->Update(V, P);
	m_pMarker->Update(V, P);
	m_pEventZone->Update(V, P);

}

void S14_DXAABBDemo::Render()
{
	ImGui::LabelText("Fire vs Marco", "%s", bCheck1 ? "O" : "X");
	ImGui::LabelText("Marker vs Marco", "%s", bCheck2 ? "O" : "X");
	ImGui::LabelText("Zone vs Marco", "%s", bCheck3 ? "O" : "X");


	Vector2 mouse = Mouse->GetPosition();
	CAMERA->WCtoVC(mouse);


	m_pBackground->Render();
	m_pMarco->Render();
	m_pFire->Render();
	m_pMarker->Render();
	m_pEventZone->Render();

	CAMERA->Render();

	// 2D Render
	DirectWrite::GetDC()->BeginDraw();
	{
		RECT  rect = { 0,0,500,200 };
		wstring str = L"FPS(ImGui) : " + to_wstring(ImGui::GetIO().Framerate);
		DirectWrite::RenderText(str, rect);

		RECT  rect2 = { 0,20,500,200 };
		wstring str2 = L"Mouse [" + to_wstring(mouse.x) + L"," + to_wstring(mouse.y) + L"]";
		DirectWrite::RenderText(str2, rect2);

	}
	DirectWrite::GetDC()->EndDraw();


}


