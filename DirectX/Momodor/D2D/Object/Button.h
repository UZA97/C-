#pragma once
class Button : public GameObject
{
public: 
	void   Update(Matrix V, Matrix P)  override;
	void   Render() override;


public:
	void  SetMoveSpeed(float moveSpeed) { m_MoveSpeed = moveSpeed; }
	class Collider*  GetCollider() { return m_pCollider; }
	void  SetCallback(function<void(void)> cbFunstion) { m_fpButtonCallBack = cbFunstion; }
	

private:
	bool       IsPtInRect(Vector2 position);
	void       Init();
	Animation  *m_pAnimation = nullptr;
	float      m_MoveSpeed = 200.0f;
	class      Collider      *m_pCollider = nullptr;
	function<void(void)>     m_fpButtonCallBack = nullptr;  // int (*exec)();

public:
	Button();
	Button(Vector2 position, Vector2 scale);
	~Button();
};