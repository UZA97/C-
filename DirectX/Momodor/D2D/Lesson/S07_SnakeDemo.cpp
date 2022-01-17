#include  "framework.h"
#include  "S07_SnakeDemo.h"
#include  "Renders/Shader.h"
#include  "Base/Camera.h"
#include  "Object/Rect.h"
#include  "Object/Snake.h"

// 짐에서 벽을 제대로 만들어 지게 해보시길 바랍니다
#define  BOX_SIZE  20.0f
#define  MAPX      44
#define  MAPY      30
/////////////////////////////////////////////////
//  생성자/소멸자
//  Left/Right/Up/Down 누르면 이동
//  1. 이동중에 먹이를 만나면  먹게 되고 꼬리가 증가된다
//  2. 키는 방향키
//  3. 벽에 부딛히면 뱀이 Kill
//  4. 뱀이 먹이를 먹으면 먹이가 다시 생성되는 것
////////////////////////////////////////////////
S07_SnakeDemo::S07_SnakeDemo()
{
	this->m_strSceneName = "S07_SnakeDemo";
	SetWindowText(Main->GetWindowHandler(), L"S07_SnakeDemo");
	Main->SetWindowSize(960, 720);

	srand(GetTickCount());

	// 수직 Block 생성

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

	// 수평 Block 생성

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


	// Snake 생성

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
	// 벽

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
// 뱀의 머리가 Food 위치와 겹쳐 지면 
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

	 // 먹이를 먹음
	return true;


}
void S07_SnakeDemo::Update()
{
	Matrix W, V, P;

	CAMERA->Update();
	V = CAMERA->GetViewMatrix();
	P = CAMERA->GetProjectionMatrix();


	// Keyboard로 입력이 된경우 

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
			// 꼬리를 추가
			// Food가 없어지게
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

	// 벽
	for(UINT i=0; i<m_cvBlocks.size(); i++)
		m_cvBlocks[i]->Update(V, P);
	// Snake
	m_pPlayer->Update(V, P);

}

void S07_SnakeDemo::Render()
{

	// 벽
	for (UINT i = 0; i < m_cvBlocks.size(); i++)
		m_cvBlocks[i]->Render();

	m_pPlayer->Render();
	if (m_pFood)
		m_pFood->Render();
}


