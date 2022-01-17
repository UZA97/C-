#pragma once
class Marker : public GameObject
{
public: 
	void   Update(Matrix V, Matrix P)  override;
	void   Render() override;


public:
	void  SetMoveSpeed(float moveSpeed) { m_MoveSpeed = moveSpeed; }
	class Collider*  GetCollider() { return m_pCollider; }
	

private:
	void       Init();
	Animation  *m_pAnimation = nullptr;
	float      m_MoveSpeed = 200.0f;
	class      Collider      *m_pCollider = nullptr;

public:
	Marker();
	Marker(Vector2 position, Vector2 scale);
	~Marker();
};