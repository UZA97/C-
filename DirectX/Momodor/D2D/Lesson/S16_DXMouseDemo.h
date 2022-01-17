#pragma once
// World 
// View
// Project
// 향후 Class로 변경
class S16_DXMouseDemo : public Scene
{
public:
	void  Update() override;   // Update(Matrix v, Matrix P) --> Object에서 사용
	void  Render() override;

public:
	void  OnButton();
	int   Test();
	int   Test2(int a, int b);
private:
	class Texture     *m_pBackground = nullptr;
	class Rect        *m_pRect1      = nullptr;
	class Rect        *m_pRect2      = nullptr;
	class Button      *m_pButton     = nullptr;
	class CustomCursor *m_pCustomCursor = nullptr;
	vector<class Marker*>  m_cvMarkers;
public:
	S16_DXMouseDemo();
	~S16_DXMouseDemo();
};
