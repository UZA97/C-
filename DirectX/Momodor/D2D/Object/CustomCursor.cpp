#include  "framework.h"
#include  "CustomCursor.h"
#include  "Collider/Collider.h"

///////////////////////////////////////////////////////////////////////
// »ı¼ºÀÚ
///////////////////////////////////////////////////////////////////////
CustomCursor::CustomCursor(wstring strImageFile, wstring strShaderFile)
{
	// ShowCursor(false);
	m_pTexture = new Texture(strImageFile, strShaderFile);
}

CustomCursor::~CustomCursor()
{
	SAFE_DELETE(m_pTexture);
}
void CustomCursor::Update(Matrix V, Matrix P)
{
	Vector2 pos = Mouse->GetPosition();
	CAMERA->WCtoVC(pos);

	SetPosition(pos);
	m_pTexture->SetPosition(pos);
	m_pTexture->SetScale(m_Scale);
	m_pTexture->Update(V, P);

}

void CustomCursor::Render()
{
	m_pTexture->Render();
}


