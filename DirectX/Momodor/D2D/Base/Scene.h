#pragma once
////////////////////////////////////////////////////////
//  Scene 추상화 Class
//       --> Intro,Boss : Class가 별도 별도로 존재 
//           상속받아서 사용을 하게함
//  1. Virtual로 정의된 함수가 있으면 추상화 Class라고 한다
//  2. Delete되는 순서 알아보기 :  Child -> Parent
//  3. private, protected 알아보기 :
////////////////////////////////////////////////////////
class Scene
{
public:
	virtual void  Update() = 0;        // 순수 가상함수, 상속받은 곳에서 구현
	virtual void  Render() = 0;        // 순수 가상함수, 상속받은 곳에서 구현
	                                   // virtual void  Update(); --> 가상함수
	// Setter
	void   SetActive(bool active) { m_Active = active; }
	void   SetSceneName(string scnename) { m_strSceneName = scnename; }
	// Getter
	bool   IsActive() { return m_Active; }
	string GetSceneName() { return m_strSceneName; }
protected: // 다른 Class에서 접근 안되는 것 --> 상속받은 Class에서도 접근됨
	bool   m_Active = true;
	string m_strSceneName;

private:   // 다른 Class에서 접근 안되는 것 --> 상속받은 Class에서도 접근 안됨


public:
	Scene() {};
	~Scene(){};
};

