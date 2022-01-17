#include  "framework.h"
#include  "Background.h"
#include  "Renders/Texture.h"

//////////////////////////////////////////////////////
// 생성자
//////////////////////////////////////////////////////
Background::Background()
{
	wstring   imageFile   = L"/Background/Tile.png";
	wstring   shaderFile  = L"Texture.fx";
	m_pTexture = new Texture(imageFile, shaderFile);

	imageFile = L"/Background/Bush.png";
	m_pBush[0] = new Texture(imageFile, shaderFile);
	imageFile = L"/Background/Bush2.png";
	m_pBush[1] = new Texture(imageFile, shaderFile);

	imageFile = L"/Background/Cloud.png";
	m_pCloud[0] = new Texture(imageFile, shaderFile, 0, 0,140,128);
	m_pCloud[1] = new Texture(imageFile, shaderFile, 140, 0, 344, 128);


}

Background::~Background()
{
	SAFE_DELETE(m_pTexture);
	SAFE_DELETE(m_pBush[0]);
	SAFE_DELETE(m_pBush[1]);
	SAFE_DELETE(m_pCloud[0]);
	SAFE_DELETE(m_pCloud[1]);


}
void Background::Update(Matrix V, Matrix P)
{
	m_pTexture->SetPosition(m_Position);
	m_pTexture->Update(V, P);

	m_pBush[0]->Update(V, P);
	m_pBush[1]->Update(V, P);

	m_pCloud[0]->Update(V, P);
	m_pCloud[1]->Update(V, P);


}

void Background::Render()
{
	Vector2 position = m_Position;

	position.x = -m_pTexture->GetRealTextureSize().x*10.0f;

	for (int i = 0; i < 30; i++)
	{
		m_pTexture->SetPosition(position);
		m_pTexture->Render();

		position.x = position.x + m_pTexture->GetRealTextureSize().x;
	}

	// Position을 여기서 .....

	m_pBush[1]->SetPosition(-423, -160);
	m_pBush[1]->Render();


	m_pBush[0]->SetPosition(-280, -218);
	m_pBush[0]->Render();

	m_pBush[1]->SetPosition(-50, -160);
	m_pBush[1]->Render();


	m_pBush[1]->SetPosition(250, -160);
	m_pBush[1]->Render();


	m_pBush[0]->SetPosition(400, -218);
	m_pBush[0]->Render();


	m_Time = m_Time + TIMEMANAGER->Delta()*100.0f;
	if (m_Time >= 500.0f)
		m_Time = 0;

	float x = -500.0f;
	x= x+m_Time;

	m_pCloud[0]->SetPosition(x+40, 120);
	m_pCloud[0]->Render();

	m_pCloud[0]->SetPosition(x + 200, 120);
	m_pCloud[0]->Render();

	m_pCloud[1]->SetPosition(x + 500, 120);
	m_pCloud[1]->Render();


}


