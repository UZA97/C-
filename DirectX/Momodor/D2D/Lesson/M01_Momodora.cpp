#include  "framework.h"
#include  "M01_Momodora.h"
#include   "Object/Line.h"
#include   "Object/MoMo.h"
#include   "Collider/Collider.h"

M01_Momodora::M01_Momodora()
{
	this->SetSceneName("M01_Momodora");
//	Main->SetWindowSize(1000, 720);
	// Object 생성
	wstring  strImageFile = L"/Momodora/background.png";
	wstring  strShader = L"Sprite.fx";

	m_pBackground[0] = new Texture(strImageFile, strShader);

	strImageFile = L"/Momodora/back1.png";
	m_pBackground[1] = new Texture(strImageFile, strShader);
	strImageFile = L"/Momodora/back2.png";
	m_pBackground[2] = new Texture(strImageFile, strShader);
	strImageFile = L"/Momodora/back3.png";
	m_pBackground[3] = new Texture(strImageFile, strShader);
	strImageFile = L"/Momodora/back4.png";
	m_pBackground[4] = new Texture(strImageFile, strShader);

	CAMERA->SetPosition(-480.0f, 0.0f);

	Vector2 size = m_pBackground[0]->GetRealTextureSize();

	float halfSizeX = size.x * 0.5f;
	float halfSizeY = size.y * 0.5f;
	float halfWidth =  (float)(Main->GetWidth()) * 0.5f;
	float halfHeight = (float)(Main->GetHeight()) * 0.5f;


	CAMERA->SetLeftCorner(Vector2(-halfSizeX + halfWidth,
		-halfSizeY + halfHeight));
	CAMERA->SetRightCorner(Vector2(halfSizeX - halfWidth,
		 halfSizeY - halfHeight));
	CAMERA->SetLimit(true);

	// Line Data Create

	Line *pLine = new Line();
	pLine->AddVertex(-1060.0f, -216.0f);
	pLine->AddVertex( 1060.0f, -216.0f);
	pLine->End();
	m_Lines.push_back(pLine);

	// Player 생성

	MOMO->SetPosition(-820.0f, 0.0f);
	MOMO->SetScale(1.0f, 1.0f);
	MOMO->SetLeftCorner(Vector2(-halfSizeX, -halfSizeY));
	MOMO->SetRightCorner(Vector2(halfSizeX, halfSizeY));


	// Fllowing Camera
	CAMERA->SetObject(MOMO);
}

M01_Momodora::~M01_Momodora()
{
	MOMO->Delete();
}
///////////////////////////////////////////////////
//  1. Object의 좌표들 Fixing
//  2. Object별로 View,Projection Matrix
///////////////////////////////////////////////////
void M01_Momodora::Update()
{
	// View, Project Setting
	Matrix  V, P;

	CAMERA->Update();
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();

	// 2. 각 Object별 position setting
	for(int i=0; i<5; i++)
	  m_pBackground[i]->Update(V, P); 

	// GroundCheck

	GroundCheck();

	CAMERA->Update(V, P);

	for (auto a : this->m_Lines)
		a->Update(V, P);

	MOMO->Update(V, P);




}

void M01_Momodora::Render()
{
	this->RenderBackground();

	for (auto a : this->m_Lines)
		a->Render();

	MOMO->Render();
	CAMERA->Render();

	Vector2 mouse = Mouse->GetPosition();
	CAMERA->WCtoVC(mouse);

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

void M01_Momodora::RenderBackground()
{
	Vector2 size  = m_pBackground[0]->GetRealTextureSize();
	Vector2 size2 = m_pBackground[1]->GetRealTextureSize();

	m_pBackground[1]->SetPosition(-size.x*0.5f + size2.x * 0.5f, 0.0f);
	m_pBackground[2]->SetPosition(-size.x*0.5f + size2.x * 0.5f, 0.0f);
	m_pBackground[3]->SetPosition(-size.x*0.5f + size2.x * 0.5f, 0.0f);
	m_pBackground[4]->SetPosition(-size.x*0.5f + size2.x * 0.5f, 0.0f);
	
	m_pBackground[4]->Render();
	m_pBackground[3]->Render();
	m_pBackground[2]->Render();
	m_pBackground[1]->Render();

	m_pBackground[1]->SetPosition(size.x*0.5f - size2.x * 0.5f, 0.0f);
	m_pBackground[2]->SetPosition(size.x*0.5f - size2.x * 0.5f, 0.0f);
	m_pBackground[3]->SetPosition(size.x*0.5f - size2.x * 0.5f, 0.0f);
	m_pBackground[4]->SetPosition(size.x*0.5f - size2.x * 0.5f, 0.0f);

	m_pBackground[4]->Render();
	m_pBackground[3]->Render();
	m_pBackground[2]->Render();
	m_pBackground[1]->Render();

	m_pBackground[0]->Render();


}

void M01_Momodora::GroundCheck()
{
	Vector2  pos = MOMO->GetPosition();
	Vector2  A = Vector2(pos.x, pos.y - MOMO->GetRealTextureSize().y*0.5f);
	Vector2  B = pos;

	for (UINT i = 0; i < m_Lines.size(); i++)
	{
		Line *p = m_Lines[i];
		for (int j = 0; j < (p->GetVertexSize()-1); j++)
		{
			Vector2 C = Vector2(p->GetPosition(j)->x, p->GetPosition(j)->y);
			Vector2 D = Vector2(p->GetPosition(j+1)->x, p->GetPosition(j+1)->y);
			Vector2 out;
			bool bCheck = Collider::IntersectionLine(A, B, C, D, out);
			if (bCheck)
			{
				MOMO->SetGround(true);
				MOMO->SetPosition(out.x, out.y + MOMO->GetRealTextureSize().y*0.5f);
			}
				
		}
	}


	


//	Line  *pLine = new Line(A, B);
//
//	m_Lines.push_back(pLine);
//
//	m_pPlayer->SetGround(true);


}


