#pragma once
////////////////////////////////////////////////////////
//  Camera Class
////////////////////////////////////////////////////////
class Camera
{
public:
	static Camera* GetInstance()
	{
		static Camera singleToneInstance;
		return &singleToneInstance;
	}
	void		Update();                    // View, Projection Matrix Update
	void		Update(Matrix V, Matrix P);  // 카메라에 대한 m_pTexture Update
	void		Render();                    //  render
public:			// Setter
	void		SetPosition(float x, float y)  { m_Position = Vector2(x, y); }
	void		SetPosition(Vector2  position) { m_Position = position; }
	void		SetOffset(float x, float y)    { m_Offset = Vector2(x, y); }
	void		SetOffset(Vector2  offset)     { m_Offset = offset; }
	void		SetObject(class GameObject *pObject) { m_pGameObj = pObject; }
	void        SetEffect() { m_bEffect = true; m_PlayTime = 0.0f;}
	void		SetMoveSpeed(float x, float y) { m_MoveSpeed = Vector2(x, y); }
	void		SetMoveSpeed(Vector2  speed)   { m_MoveSpeed = speed; }
	void        SetLeftCorner(Vector2 left)    { m_Left = left; }
	void        SetRightCorner(Vector2 right) { m_Right = right; }
	void        SetLimit(bool limit) { m_bLimit = limit; }
public:    // Getter
	Vector2		GetPosition() { return m_Position; }
	Matrix		GetViewMatrix();
	Matrix		GetProjectionMatrix();
	void		WCtoVC(Vector2& position);   // Window Coord -> View Coord

private:
	void		PreUpdate();                 // 키보드 입력시 카메라 이동 위치 변환
    Matrix		LookAt(Vector3 eye, Vector3 at, Vector3 upVector);
	void        Effect();
private:
	class			Texture *m_pTexture = nullptr;
	Vector2			m_Position  = Vector2(0.0f, 0.0f);
	Vector2			m_MoveSpeed = Vector2(200.0f, 200.0f); // 카메라 위치 변경시 시간에 대한 Speed
	Matrix			m_View;
	Matrix			m_Projection;
	class           GameObject		*m_pGameObj = nullptr;  // GameObject를 선언하면 카메라가 GameOject에 붙어 있는 것 처럼 행동
	Vector2         m_Offset = Vector2(0.0f, 0.0f);
	Vector2         m_Left = Vector2(0.0f, 0.0f);
	Vector2         m_Right = Vector2(0.0f, 0.0f);
private:
	bool            m_bEffect  = false;
	bool            m_bLimit = false;
	float           m_PlayTime  = 0.0f;   // effect 되는 시간을 저장
	float           m_DelayTime = 0.5f;   // effect 발생시 delay 되는 시간

public:
	~Camera();
private:
	Camera();
	
};

