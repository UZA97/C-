#pragma once
// World 
// View
// Project
// ���� Class�� ����
class S13_DXCameraDemo : public Scene
{
public:
	void  Update() override;   // Update(Matrix v, Matrix P) --> Object���� ���
	void  Render() override;
private:
	class Texture     *m_pBackground = nullptr;
	class Marco       *m_pMarco1 = nullptr;
	class Marco       *m_pMarco2 = nullptr;
public:
	S13_DXCameraDemo();
	~S13_DXCameraDemo();
};
