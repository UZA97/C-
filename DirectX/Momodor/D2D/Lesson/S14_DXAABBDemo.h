#pragma once
// World 
// View
// Project
// ���� Class�� ����
class S14_DXAABBDemo : public Scene
{
public:
	void  Update() override;   // Update(Matrix v, Matrix P) --> Object���� ���
	void  Render() override;
private:
	class Texture     *m_pBackground = nullptr;
	class Marco       *m_pMarco     = nullptr;    // GameObject
	class Fire        *m_pFire      = nullptr;    // GameObject
	class Marker      *m_pMarker    = nullptr;    // GameObject
	class EventZone   *m_pEventZone = nullptr;    // GameObject
	bool  bCheck1 = false;
	bool  bCheck2 = false;
	bool  bCheck3 = false;
public:
	S14_DXAABBDemo();
	~S14_DXAABBDemo();
};
