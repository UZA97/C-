#pragma once
// World 
// View
// Project
// ���� Class�� ����
class S09_DXTextureClassDemo : public Scene
{
public:
	void Update() override;
	void Render() override;
private:

private:
	class  Texture            *m_pPlayer1   = nullptr;
	class  Texture            *m_pPlayer2   = nullptr;
	class  AnimationClip      *m_pAnimationClip = nullptr;
public:
	S09_DXTextureClassDemo();
	~S09_DXTextureClassDemo();
};
