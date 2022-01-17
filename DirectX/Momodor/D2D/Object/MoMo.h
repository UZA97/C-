#pragma once

#define    PIE          (float)(3.141592)
#define    VERT        (float)(PIE/2)
#define    LEFT_ANGLE  (float)(PIE/5*3)
#define    RIGHT_ANGLE (float)(PIE/5*2)
class MoMo : public GameObject
{
public:
	static MoMo* GetInstance()
	{
		static MoMo singleToneInstance;
		return &singleToneInstance;
	}

public: 
	void   Update(Matrix V, Matrix P)  override;
	void   Render() override;
	void   Delete();


public:
	void        SetLeftCorner(Vector2 left) { m_Left = left; }
	void        SetRightCorner(Vector2 right) { m_Right = right; }
	Vector2     GetRealTextureSize();
	void        SetMoveSpeed(float moveSpeed) { m_MoveSpeed = moveSpeed; }
	class Collider*  GetCollider() { return m_pCollider; }
	void        SetGround(bool ground); 

private:    // 변수
	enum eState
	{
		Idle = 0,
		Attack,
		Attack2,
		Run,
		JumpStart,
		Falling,
		Dead,
		Pray
	};
	eState     m_nState = eState::Idle;
	Animation  *m_pAnimation = nullptr;
	float      m_MoveSpeed = 200.0f;
	class      Collider      *m_pCollider = nullptr;
	bool       m_bGround = false;
	// 동작에 관련된 함수
	function<void(Vector2&)> m_pFunctionState = nullptr;  // 상태패턴 Like
	float      m_Gravirty = 0.0f;
	float      m_SpeedY   = 9.0f;
	float      m_SpeedX   = 8.0f;
	float      m_Angle = VERT;
	Vector2    m_Left;
	Vector2    m_Right;
	class      MoMoEffect  *m_pMoMoEffect = nullptr;
private:    // 함수
	void       IdleState(Vector2& position);
	void       RunState(Vector2& position);
	void       JumpState(Vector2& position);
	void       FallingState(Vector2& position);
	void       SetState(int Direction, eState state);
	bool       LeftCornerCheck(Vector2& pos);
	bool       RightCornerCheck(Vector2& pos);
	bool       IsLeft() { if (m_Rotation.y != 0.0) return true; return false; }
	bool       IsChangeSceen(Vector2& position);
private:
	MoMo();
	~MoMo();
};