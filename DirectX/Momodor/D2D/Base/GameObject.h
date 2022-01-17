#pragma once
////////////////////////////////////////////////////////
//  Gameobject 추상화 Class
//       --> Intro,Boss : Class가 별도 별도로 존재 
//           상속받아서 사용을 하게함
//  1. Virtual로 정의된 함수가 있으면 추상화 Class라고 한다
//  2. Delete되는 순서 알아보기 :  Child -> Parent
//  3. private, protected 알아보기 :
////////////////////////////////////////////////////////
class GameObject
{
public:
	virtual void  Update(Matrix V, Matrix P) = 0;        // 순수 가상함수, 상속받은 곳에서 구현
	virtual void  Render() = 0;                          // 순수 가상함수, 상속받은 곳에서 구현
	                                  
	// Setter
	void		SetPosition(float x, float y) { m_Position = Vector2(x, y); }
	void		SetPosition(Vector2 position) { m_Position = position; }
	void		SetScale(float x, float y) { m_Scale = Vector2(x, y); }
	void		SetScale(Vector2 scale) { m_Scale = scale; }
	void		SetRotation(float x, float y, float z) { m_Rotation = Vector3(x, y, z); }
	void		SetRotation(Vector3 rotation) { m_Rotation = rotation; }
	void        SetName(string name) { m_strName = name; }
	void        SetCollisionCheck(bool collision) { m_bCollisionCheck = collision; }
	void        SetActive(bool active) { m_bActive = active; }
	// Getter
	Vector2     GetPosition() { return m_Position; }
	Vector2     GetScale()    { return m_Scale; }
	Vector3     GetRotation() { return m_Rotation; }
	string      GetName()     { return m_strName; }
	bool        IsActive()    { return m_bActive; }
	bool        GetCollisionCheck() { return m_bCollisionCheck; }

protected: // 다른 Class에서 접근 안되는 것 --> 상속받은 Class에서도 접근됨
	Vector2   m_Scale = Vector2(1.0f, 1.0f);       // Scale
	Vector2   m_Position = Vector2(0.0f, 0.0f);       // 좌표
	Vector3   m_Rotation = Vector3(0.0f, 0.0f, 0.0f);  // 회전
	string    m_strName;    // Object Name
	bool      m_bActive = true;
	bool      m_bCollisionCheck = false;   // 충돌검사시 Check

private:   // 다른 Class에서 접근 안되는 것 --> 상속받은 Class에서도 접근 안됨


public:
	GameObject() {};
	~GameObject(){};
};

