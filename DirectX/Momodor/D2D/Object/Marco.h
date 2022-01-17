#pragma once
class Marco : public GameObject
{
public: 
	void   Update(Matrix V, Matrix P)  override;
	void   Render() override;


public:
	void  SetMoveSpeed(float moveSpeed) { m_MoveSpeed = moveSpeed; }
	class Collider*  GetCollider() { return m_pCollider; }

private:
	enum eState 
	{
        IDLE = 0,
		WALK
	};
	eState     m_nState = eState::IDLE;
private:
	Animation  *m_pAnimation = nullptr;
	float      m_MoveSpeed = 200.0f;
	class      Collider      *m_pCollider = nullptr;

public:
	Marco();
	~Marco();
};