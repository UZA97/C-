#pragma once
// World 
// View
// Project
// 향후 Class로 변경
class S13_DXCameraDemo : public Scene
{
public:
	void  Update() override;   // Update(Matrix v, Matrix P) --> Object에서 사용
	void  Render() override;
private:
	class Texture     *m_pBackground = nullptr;
	class Marco       *m_pMarco1 = nullptr;
	class Marco       *m_pMarco2 = nullptr;
public:
	S13_DXCameraDemo();
	~S13_DXCameraDemo();
};
