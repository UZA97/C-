#pragma once
////////////////////////////////////////////////////////
// Scene�� �����ϴ� Class
////////////////////////////////////////////////////////
class SceneManager
{
public:
	static SceneManager* GetInstance()
	{
		static SceneManager instance;
		return &instance;
	}

public:
	void	Update();      
	void	Render();
	void	Delete();
	void	ChangeScene(string name);
	Scene*	GetScene(string name);
	Scene*  GetCurrentScene();
private:
	void  ThreadStart();
	                                

private: 
	vector<class Scene*>  m_cvScenes;   // Scene�� ��� �ִ� vector


private:
	SceneManager();
	~SceneManager();
};

