#pragma once
class MoMoEffect : public GameObject
{
public:
	void  Update(Matrix V, Matrix P) override;
	void  Render() override;

private:  // 변수
	class Animation *m_pAnimation;
	class Collider  *m_pCollider;

public: // 생성자 & 소멸자
	MoMoEffect();
	~MoMoEffect();

};