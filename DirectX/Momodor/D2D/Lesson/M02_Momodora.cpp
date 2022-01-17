#include  "framework.h"
#include  "M02_Momodora.h"
#include  "Object/Line.h"
#include  "Collider/Collider.h"

M02_Momodora::M02_Momodora()
{
	this->SetSceneName("M02_Momodora");
	this->SetActive(false);

	// Object 생성
	wstring  strImageFile = L"/Momodora/background2.png";
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

	// M02와 MO1이 크기가 같기 때문에  CAMERA Limit 하지 않음

	// Line Data Add
	Line *pLine = NULL;
	pLine = new Line();
	pLine->AddVertex(-960.0f, -216.0f);
	pLine->AddVertex(-624.0f, -216.0f);
	pLine->AddVertex(-624.0f, -165.0f);
	pLine->AddVertex(-384.0f, -165.0f);
	pLine->AddVertex(-384.0f, -310.0f);
	pLine->AddVertex(-96.0f, -310.0f);
	pLine->AddVertex(-96.0f, -265.0f);
	pLine->AddVertex(-48.0f, -265.0f);
	pLine->AddVertex(-48.0f, -216.0f);
	pLine->AddVertex(960.0f, -216.0f);
	pLine->End();
	this->m_Lines.push_back(pLine);

	pLine = new Line();
	pLine->AddVertex(-240.0f, -216.0f);
	pLine->AddVertex(-192.0f, -216.0f);
	pLine->AddVertex(-192.0f, -168.0f);
	pLine->AddVertex(-240.0f, -168.0f);
	pLine->AddVertex(-240.0f, -216.0f);
	pLine->End();
	this->m_Lines.push_back(pLine);
}

M02_Momodora::~M02_Momodora()
{
}
void M02_Momodora::Update()
{
	Matrix V, P;
	CAMERA->Update();
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();

	for (int i = 0; i < 5; i++)
		m_pBackground[i]->Update(V, P);
	CAMERA->Update(V, P);
	
	MOMO->Update(V, P);
	GroundCheck();
	WallCheck();

	for (auto a : m_Lines)
		a->Update(V, P);


}

void M02_Momodora::Render()
{
	RenderBackground();
	for (auto a : m_Lines)
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

void M02_Momodora::RenderBackground()
{
	Vector2 size = m_pBackground[0]->GetRealTextureSize();
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

void M02_Momodora::GroundCheck()
{
	Vector2  pos = MOMO->GetPosition();
	Vector2  A = Vector2(pos.x, pos.y - MOMO->GetRealTextureSize().y*0.5f);
	Vector2  B = pos;

	bool bCheck = false;

	for (UINT i = 0; i < m_Lines.size(); i++)
	{
		Line *p = m_Lines[i];
		for (int j = 0; j < (p->GetVertexSize() - 1); j++)
		{
			Vector2 C = Vector2(p->GetPosition(j)->x, p->GetPosition(j)->y);
			Vector2 D = Vector2(p->GetPosition(j + 1)->x, p->GetPosition(j + 1)->y);
			Vector2 out;
			bCheck = Collider::IntersectionLine(A, B, C, D, out);
			if (bCheck)
			{
				MOMO->SetGround(true);
				MOMO->SetPosition(out.x, out.y + MOMO->GetRealTextureSize().y*0.5f);
				break;
			}
		
		}
		if (bCheck)
			break;
	}
	if (!bCheck)
		MOMO->SetGround(false);

}
//////////////////////////////////////////
//         |     |
//         |   --|
//         |     |
void M02_Momodora::WallCheck()
{
	Vector2 position = MOMO->GetPosition();
	Vector2 size = MOMO->GetRealTextureSize();

	Vector2 Right = Vector2(position.x + size.x*0.5f, position.y);
	Vector2 Left  = Vector2(position.x - size.x*0.5f, position.y);
	Vector2 Up    = Vector2(position.x, position.y+size.y*0.5f);

	bool bCheck = false;

	for (UINT i = 0; i < m_Lines.size(); i++)
	{
		Line *p = m_Lines[i];
		for (int j = 0; j < (p->GetVertexSize() - 1); j++)
		{
			Vector2 C = Vector2(p->GetPosition(j)->x, p->GetPosition(j)->y);
			Vector2 D = Vector2(p->GetPosition(j + 1)->x, p->GetPosition(j + 1)->y);
			Vector2 out;
			bCheck = Collider::IntersectionLine(position, Right, C, D, out);
			if (bCheck)
			{
				MOMO->SetPosition(out.x - size.x*0.5, out.y);
				break;
			}
				
			bCheck = Collider::IntersectionLine(position, Left, C, D, out);
			if (bCheck)
			{
				MOMO->SetPosition(out.x + size.x*0.5, out.y);
				break;
			}
			
			bCheck = Collider::IntersectionLine(position, Up, C, D, out);
			if (bCheck)
			{
				MOMO->SetPosition(out.x , out.y- size.y*0.5);
				break;
			}

		}
		if (bCheck)
			break;
	}

	printf("---> %d\n", bCheck);

}


