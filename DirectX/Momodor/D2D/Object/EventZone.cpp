#include  "framework.h"
#include  "EventZone.h"
#include  "Collider/Collider.h"
///////////////////////////////////////////////////////////
// »ı¼ºÀÚ
//////////////////////////////////////////////////////////
EventZone::EventZone()
{
	
	SetName("EventZone");

	m_pCollider = new Collider();
	m_pCollider->SetParent(this);

}

EventZone::~EventZone()
{
	SAFE_DELETE(m_pCollider);
}
void EventZone::Update(Matrix V, Matrix P)
{
	Vector2 position = GetPosition();

	// Collider
	m_pCollider->SetPosition(position);
	m_pCollider->SetScale(m_Scale.x,m_Scale.y);
	m_pCollider->SetRotation(m_Rotation);
	m_pCollider->Update(V, P);

}

void EventZone::Render()
{
	if(GetCollisionCheck())
		m_pCollider->SetColor(1.0f, 0.0f, 0.0f);
	else
		m_pCollider->SetColor(0.0f, 1.0f, 0.0f);

	m_pCollider->Render();
}


