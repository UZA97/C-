#pragma once
class CloseScene : public Scene
{
public:
	void Update();
	void Render();

private:
	class Rect  *m_pRect = nullptr;
	vector<class Rect*> m_cvBlocks;
	class Snake *m_pPlayer = nullptr;
public:
	CloseScene();
	~CloseScene();
};