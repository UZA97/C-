#pragma once
// World 
// View
// Project
// ���� Class�� ����
class S11_DXAniamtionDemo : public Scene
{
public:
	void  Update() override;
	void  Render() override;
private:
	class Background  *m_pBackground = nullptr;
	class Texture     *m_pWizard = nullptr;
	class Animation   *m_pAnimation = nullptr;
	class Marco       *m_pMarco = nullptr;

public:
	S11_DXAniamtionDemo();
	~S11_DXAniamtionDemo();
};
