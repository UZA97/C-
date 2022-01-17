#pragma once
////////////////////////////////////////////////////////
//  Scene �߻�ȭ Class
//       --> Intro,Boss : Class�� ���� ������ ���� 
//           ��ӹ޾Ƽ� ����� �ϰ���
//  1. Virtual�� ���ǵ� �Լ��� ������ �߻�ȭ Class��� �Ѵ�
//  2. Delete�Ǵ� ���� �˾ƺ��� :  Child -> Parent
//  3. private, protected �˾ƺ��� :
////////////////////////////////////////////////////////
class Scene
{
public:
	virtual void  Update() = 0;        // ���� �����Լ�, ��ӹ��� ������ ����
	virtual void  Render() = 0;        // ���� �����Լ�, ��ӹ��� ������ ����
	                                   // virtual void  Update(); --> �����Լ�
	// Setter
	void   SetActive(bool active) { m_Active = active; }
	void   SetSceneName(string scnename) { m_strSceneName = scnename; }
	// Getter
	bool   IsActive() { return m_Active; }
	string GetSceneName() { return m_strSceneName; }
protected: // �ٸ� Class���� ���� �ȵǴ� �� --> ��ӹ��� Class������ ���ٵ�
	bool   m_Active = true;
	string m_strSceneName;

private:   // �ٸ� Class���� ���� �ȵǴ� �� --> ��ӹ��� Class������ ���� �ȵ�


public:
	Scene() {};
	~Scene(){};
};

