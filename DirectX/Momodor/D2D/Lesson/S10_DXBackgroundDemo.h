#pragma once
// World 
// View
// Project
// ���� Class�� ����
class S10_DXBackgroundDemo : public Scene
{
public:
	void  Update() override;
	void  Render() override;
private:
	class Background  *m_pBackground = nullptr;
	class Texture     *m_pMario = nullptr;
	class Texture     *m_pWizard = nullptr;

public:
	S10_DXBackgroundDemo();
	~S10_DXBackgroundDemo();
};
