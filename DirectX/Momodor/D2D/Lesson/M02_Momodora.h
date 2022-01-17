#pragma once
class M02_Momodora : public Scene
{
public:
	void Update() override;
	void Render() override;
private:
	void  RenderBackground();
	void  GroundCheck();
	void  WallCheck();
private:
	Texture *m_pBackground[5];
	vector<class Line*> m_Lines;
public:
	M02_Momodora();
	~M02_Momodora();

};