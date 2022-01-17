#include  "framework.h"
#include  "Snake.h"
#include  "Rect.h"

#define  BOX_SIZE  20.0f
//////////////////////////////////////////////////
// 생성자
/////////////////////////////////////////////////
Snake::Snake()
{
	Rect *pRect = new Rect();
	pRect->SetPosition(m_Position);
	pRect->SetColor(1.0f, 0.0f, 0.0f);
	pRect->SetScale(BOX_SIZE, BOX_SIZE);
	m_cvSnakes.push_back(pRect);  // 뱀의 머리

	Vector2 pos = m_Position;


	for (UINT i = 0; i < 10; i++)
	{
		Rect *pRect = new Rect();
		pos.y = pos.y - BOX_SIZE;
		pRect->SetPosition(pos);
		pRect->SetColor(1.0f, 0.0f, 1.0f);
		pRect->SetScale(BOX_SIZE-2, BOX_SIZE-2);
		m_cvSnakes.push_back(pRect);  // 뱀의 꼬리

	}



}
Snake::~Snake()
{
	if (m_cvSnakes.size())
	{
		for (UINT i = 0; i < m_cvSnakes.size(); i++)
		{
			delete m_cvSnakes[i];
		}
		m_cvSnakes.erase(m_cvSnakes.begin(), 
			             m_cvSnakes.begin() + m_cvSnakes.size());
	}


}
void Snake::Update(Matrix V, Matrix P)
{
	Vector2 pos = m_Position;   // 뱀의 머리

	m_Time = m_Time + TIMEMANAGER->Delta();
	if (m_Time <= 0.25f)
		return;
	m_Time = 0.0f;

	switch (m_Direction)
	{
	case eDirection::UP:
		pos.y = pos.y + BOX_SIZE;
		break;
	case eDirection::LEFT:
		pos.x = pos.x - BOX_SIZE;
		break;
	case eDirection::DOWN:
		pos.y = pos.y - BOX_SIZE;
		break;
	case eDirection::RIGHT:
		pos.x = pos.x + BOX_SIZE;
		break;
	}


	// 위치변경
	for (UINT i = (m_cvSnakes.size() - 1); i > 0; i--)
	{
			Rect *pRect1 = m_cvSnakes[i];
			Rect *pRect2 = m_cvSnakes[i-1];
			pRect1->SetPosition(pRect2->GetPosition());
	}

	m_Position = pos;
	m_cvSnakes[0]->SetPosition(pos);

	// 벽에 닿았을때 Close  1. 위치 좌표 2. Object & Object 충돌 : // 내일

	if (pos.y <= m_BlockMin.y || pos.y >= m_BlockMax.y)
	{
		SCENEMANAGER->ChangeScene("CloseScene");
		printf("Game Close\n");
	}
		
	if (pos.x <= m_BlockMin.x || pos.x >= m_BlockMax.x)
	{
		SCENEMANAGER->ChangeScene("CloseScene");
		printf("Game Close\n");
	}

	// 몸통에 닿았을때 Close  2. Object & Object 충돌

	if (CollisionSelf())
	{
		SCENEMANAGER->ChangeScene("CloseScene");
		printf("Game Close\n");

	}


	for (UINT i = 0; i < m_cvSnakes.size(); i++)
	{
		m_cvSnakes[i]->Update(V, P);
	}


}
void Snake::Render()
{
	for (auto snake : m_cvSnakes)
		snake->Render();
}

void Snake::AddTail()
{
	Rect *pRect = new Rect();
	pRect->SetColor(0.5f, 0.3f, 1.0f);
	pRect->SetScale(BOX_SIZE - 2, BOX_SIZE - 2);
	m_cvSnakes.push_back(pRect);  // 뱀의 꼬리
}

bool Snake::IsFoodPosition(Vector2 pos)
{
	bool retValue = false;
	for (UINT i = 0; i < m_cvSnakes.size(); i++)
	{
		if(!CollisonFood(pos, m_cvSnakes[i]->GetPosition()) )
			continue;
		retValue = true;
	}

	return retValue;
}

bool Snake::CollisonFood(Vector2 pos1, Vector2 pos2)
{

	Vector2 min1 = Vector2(pos1.x - BOX_SIZE * 0.5f, pos1.y - BOX_SIZE * 0.5f);
	Vector2 max1 = Vector2(pos1.x + BOX_SIZE * 0.5f, pos1.y + BOX_SIZE * 0.5f);

	Vector2 min2 = Vector2(pos2.x - BOX_SIZE * 0.5f, pos2.y - BOX_SIZE * 0.5f);
	Vector2 max2 = Vector2(pos2.x + BOX_SIZE * 0.5f, pos2.y + BOX_SIZE * 0.5f);

	if ((min1.x > max2.x) || (max1.x < min2.x))
		return false;
	if ((min1.y > max2.y) || (max1.y < min2.y))
		return false;
	return true;

}

bool Snake::CollisionSelf()
{
	bool change_scene = false;

	// 머리에 대한 좌표 
	Vector2 pos = m_cvSnakes[0]->GetPosition();

	for (UINT i = 1; i < this->m_cvSnakes.size(); i++)
	{
		Vector2 pos2 = m_cvSnakes[i]->GetPosition();
		if (pos == pos2)  // 겹쳐진것
		{
			change_scene = true;
			break;
		}
	}
	return change_scene;
}


