#pragma once
// World 
// View
// Project
// ���� Class�� ����
class S12_DXRotationDemo : public Scene
{
public:
	void  Update() override;
	void  Render() override;
private:
	class Background  *m_pBackground = nullptr;
	class Texture     *m_pWizard = nullptr;
	class Marco       *m_pMarco = nullptr;

public:
	S12_DXRotationDemo();
	~S12_DXRotationDemo();
};
