#pragma once
class MoMoEffect : public GameObject
{
public:
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;

private:  // ����
	class Animation *m_pAnimation;
	class Collider  *m_pCollider;

public: // ������ & �Ҹ���
	MoMoEffect();
	~MoMoEffect();

};