#pragma once
// World 
// View
// Project
class S07_SnakeDemo : public Scene
{
public:
	void Update() override;
	void Render() override;
private:
	class Rect  *m_pFood = nullptr;
	vector<class Rect*> m_cvBlocks;
	class Snake *m_pPlayer = nullptr;
	void  CreateFood();
	bool  CollisonFood();
	float  m_Time = 0.0f;

public:
	S07_SnakeDemo();
	~S07_SnakeDemo();
};
