#pragma once
// World 
// View
// Project
class S06_WVPDemo : public Scene
{
public:
	void Update() override;
	void Render() override;
private:
	class Rect  *m_pRect = nullptr;


public:
	S06_WVPDemo();
	~S06_WVPDemo();
};
