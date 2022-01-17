#pragma once
class Snake
{
public:
	enum eDirection {
		UP = 0,
		RIGHT,
		DOWN,
		LEFT
	};
public: 
	void   Update(Matrix V, Matrix P);
	void   Render();

	void   AddTail();
	eDirection    GetDirection() { return m_Direction; }
	void   SetDirection(eDirection state) { m_Direction = state; }
	void   SetBlock(Vector2 minBlock, Vector2 maxBlock)
	{
		m_BlockMin = minBlock;
		m_BlockMax = maxBlock;
	}
	Vector2 GetPosition() { return m_Position; }
	bool    IsFoodPosition(Vector2 pos);

private:
	bool   CollisonFood(Vector2 pos1, Vector2 pos2);
	bool   CollisionSelf();
	vector<class Rect*>		m_cvSnakes;
	Vector2					m_Position = Vector2(0.0f, 0.0f);
	eDirection				m_Direction = eDirection::UP;
	float					m_Time = 0.0f;
	Vector2					m_BlockMin = Vector2(0.0, 0.0f);
	Vector2					m_BlockMax = Vector2(0.0, 0.0f);

public:
	Snake();
	~Snake();
};