#include  "framework.h"
#include  "S07_SnakeDemo.h"
#include  "Renders/Shader.h"
#include  "Base/Camera.h"
#include  "Object/Rect.h"
#include  "Object/Snake.h"

// ������ ���� ����� ����� ���� �غ��ñ� �ٶ��ϴ�
#define  BOX_SIZE  20.0f
#define  MAPX      44
#define  MAPY      30
/////////////////////////////////////////////////
//  ������/�Ҹ���
//  Left/Right/Up/Down ������ �̵�
//  1. �̵��߿� ���̸� ������  �԰� �ǰ� ������ �����ȴ�
//  2. Ű�� ����Ű
//  3. ���� �ε����� ���� Kill
//  4. ���� ���̸� ������ ���̰� �ٽ� �����Ǵ� ��
////////////////////////////////////////////////
S07_SnakeDemo::S07_SnakeDemo()
{
	this->m_strSceneName = "S07_SnakeDemo";
	SetWindowText(Main->GetWindowHandler(), L"S07_SnakeDemo");
	Main->SetWindowSize(960, 720);

	srand(GetTickCount());

	// ���� Block ����

	Rect *pRect = new Rect();
	pRect->SetScale(BOX_SIZE, BOX_SIZE * MAPY);
	pRect->SetColor(0.3f, 0.2f, 0.65f);
	pRect->SetPosition(BOX_SIZE * MAPX *0.5 + BOX_SIZE*0.5, 0.0);
	m_cvBlocks.push_back(pRect);

	pRect = new Rect();
	pRect->SetScale(BOX_SIZE, BOX_SIZE * MAPY);
	pRect->SetColor(0.3f, 0.2f, 0.65f);
	pRect->SetPosition(-BOX_SIZE * MAPX *0.5 - BOX_SIZE * 0.5, 0.0);
	m_cvBlocks.push_back(pRect);

	// ���� Block ����

	pRect = new Rect();
	pRect->SetScale(BOX_SIZE*MAPX, BOX_SIZE);
	pRect->SetColor(0.3f, 0.2f, 0.65f);
	pRect->SetPosition(0.0, BOX_SIZE*MAPY*0.5 + BOX_SIZE*0.5);
	m_cvBlocks.push_back(pRect);

	pRect = new Rect();
	pRect->SetScale(BOX_SIZE*MAPX, BOX_SIZE);
	pRect->SetColor(0.3f, 0.2f, 0.65f);
	pRect->SetPosition(0.0, -BOX_SIZE*MAPY*0.5 - BOX_SIZE * 0.5);
	m_cvBlocks.push_back(pRect);


	// Snake ����

	m_pPlayer = new Snake();
	Vector2 min = Vector2(-BOX_SIZE * MAPX *0.5 + BOX_SIZE * 0.5, 
		                  -BOX_SIZE * MAPY*0.5  + BOX_SIZE * 0.5);

	Vector2 max = Vector2(BOX_SIZE * MAPX *0.5 - BOX_SIZE * 0.5,
		                  BOX_SIZE * MAPY*0.5  - BOX_SIZE * 0.5);

	m_pPlayer->SetBlock(min, max);
	CreateFood();
}

S07_SnakeDemo::~S07_SnakeDemo()
{
	SAFE_DELETE(m_pPlayer);
	// ��

	if (m_cvBlocks.size())
	{
		for (UINT i = 0; i < m_cvBlocks.size(); i++)
			delete m_cvBlocks[i];
		m_cvBlocks.erase(m_cvBlocks.begin(),
			             m_cvBlocks.begin() + m_cvBlocks.size());
	}
	SAFE_DELETE(m_pFood);

}
/////////////////////////////////////////////////////////////////
// Create Food
////////////////////////////////////////////////////////////////
void S07_SnakeDemo::CreateFood()
{
	m_pFood = new Rect();
	m_pFood->SetScale(BOX_SIZE, BOX_SIZE);


	while (1)
	{
		int x = rand() % MAPX;
		int y = rand() % MAPY;

		Vector2 pos2= Vector2((-MAPX / x + 5)* BOX_SIZE + BOX_SIZE * 0.5f,
			                  (-MAPY / y + 5)* BOX_SIZE + BOX_SIZE * 0.5f);
	
		if( m_pPlayer->IsFoodPosition(pos2))
			continue;
		m_pFood->SetPosition(pos2);
		break;
	}

}
// ���� �Ӹ��� Food ��ġ�� ���� ���� 
bool S07_SnakeDemo::CollisonFood()
{
	Vector2 pos1 = m_pPlayer->GetPosition();
	Vector2 pos2 = m_pFood->GetPosition();

	Vector2 min1 = Vector2(pos1.x - BOX_SIZE * 0.5f, pos1.y - BOX_SIZE * 0.5f);
	Vector2 max1 = Vector2(pos1.x + BOX_SIZE * 0.5f, pos1.y + BOX_SIZE * 0.5f);

	Vector2 min2 = Vector2(pos2.x - BOX_SIZE * 0.5f, pos2.y - BOX_SIZE * 0.5f);
	Vector2 max2 = Vector2(pos2.x + BOX_SIZE * 0.5f, pos2.y + BOX_SIZE * 0.5f);

	if ((min1.x > max2.x) || (max1.x < min2.x))
		return false;
	if ((min1.y > max2.y) || (max1.y < min2.y))
		return false;

	 // ���̸� ����
	return true;


}
void S07_SnakeDemo::Update()
{
	Matrix W, V, P;

	CAMERA->Update();
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();


	// Keyboard�� �Է��� �Ȱ�� 

	if (KEYMANAGER->Down(VK_LEFT))
	{
		if(m_pPlayer->GetDirection() != Snake::eDirection::RIGHT)
			m_pPlayer->SetDirection(Snake::eDirection::LEFT);
	}
		

	if (KEYMANAGER->Down(VK_RIGHT))
	{
		if (m_pPlayer->GetDirection() != Snake::eDirection::LEFT)
		m_pPlayer->SetDirection(Snake::eDirection::RIGHT);
	}
		

	if (KEYMANAGER->Down(VK_UP))
	{
		if (m_pPlayer->GetDirection() != Snake::eDirection::DOWN)
		    m_pPlayer->SetDirection(Snake::eDirection::UP);
	}
		
	if (KEYMANAGER->Down(VK_DOWN))
	{
		if (m_pPlayer->GetDirection() != Snake::eDirection::UP)
		m_pPlayer->SetDirection(Snake::eDirection::DOWN);
	}
		
	if (m_pFood)
	{
		if (CollisonFood())
		{
			// ������ �߰�
			// Food�� ��������
			SAFE_DELETE(m_pFood);
			m_pPlayer->AddTail();
			m_Time = 0.0f;

		}
	}

	else
	{
		m_Time = m_Time + TIMEMANAGER->Delta();
		if (m_Time > 0.5f)
		{
			CreateFood();
			m_Time = 0.0f;
		}

	}

	if(m_pFood)
		m_pFood->Update(V, P);

	// ��
	for(UINT i=0; i<m_cvBlocks.size(); i++)
		m_cvBlocks[i]->Update(V, P);
	// Snake
	m_pPlayer->Update(V, P);

}

void S07_SnakeDemo::Render()
{

	// ��
	for (UINT i = 0; i < m_cvBlocks.size(); i++)
		m_cvBlocks[i]->Render();

	m_pPlayer->Render();
	if (m_pFood)
		m_pFood->Render();
}


