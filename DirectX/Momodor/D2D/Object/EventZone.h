#pragma once
class EventZone : public GameObject
{
public: 
	void   Update(Matrix V, Matrix P)  override;
	void   Render() override;
public:
	class Collider*  GetCollider() { return m_pCollider; }

private:
	class      Collider      *m_pCollider = nullptr;

public:
	EventZone();
	~EventZone();
};