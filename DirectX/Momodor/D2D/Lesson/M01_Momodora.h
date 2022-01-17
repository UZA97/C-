#pragma once
class M01_Momodora : public Scene
{
public:
	void Update() override;
	void Render() override;

private:
	void  RenderBackground();
	void  GroundCheck();
private:
	Texture *m_pBackground[5];
	vector<class Line*> m_Lines;

public:
	M01_Momodora();
	~M01_Momodora();
};