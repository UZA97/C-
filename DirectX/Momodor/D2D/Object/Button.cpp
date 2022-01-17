#include  "framework.h"
#include  "Button.h"
#include  "Collider/Collider.h"
///////////////////////////////////////////////////////////
// 생성자
//////////////////////////////////////////////////////////
Button::Button()
{
	Init();
}
bool Button::IsPtInRect(Vector2 position)
{
	bool   check = m_pCollider->IsPtInRect(position);

	if (check == false)
		return false;

	// 상세하게 Check  : 타원형 데이터

	vector<Vector2*> cvPolygons;
	Vector2  size = m_pAnimation->GetRealTextureSize();
	for (int i = 0; i < 361; i++)
	{
		float X = (size.x*0.47f)*cosf(3.141592*i / 180.0f) + GetPosition().x;
		float Y = (size.y*0.47f)*sinf(3.141592*i / 180.0f) + GetPosition().y;
		cvPolygons.push_back(new Vector2(X, Y));
	}

	check = Collider::IsPtInRegion(cvPolygons, position);

	for (UINT i = 0; i < cvPolygons.size(); i++)
	{
		delete cvPolygons[i];
	}
	cvPolygons.erase(cvPolygons.begin(), cvPolygons.end());


	return check;



}
void Button::Init()
{
	wstring  imgFile1     = L"/Button/btn_jump_no.png";
	wstring  imgFile2     = L"/Button/btn_jump_dim.png";
	wstring  shaderFile  = L"Sprite.fx";
	
	m_pAnimation = new Animation();


	SetName("Button");


	// 밝은 색상
	{
		AnimationClip *pClip = new AnimationClip();
		Texture *pImage = pClip->CreateTexture(imgFile1, shaderFile);
		pClip->AddFrame(pImage, 0,0, 236, 178,  0.3f);
		pClip->AddFrame(pImage, 0,0, 236, 178,  0.3f);
		pClip->AddFrame(pImage, 0,0, 236, 178,  0.3f);
		pClip->AddFrame(pImage, 0,0, 236, 178,  0.3f);
		m_pAnimation->AddAnimationClip(pClip);
	}

	// 어두운 색상
	{
		AnimationClip *pClip = new AnimationClip();
		Texture *pImage = pClip->CreateTexture(imgFile2, shaderFile);
		pClip->AddFrame(pImage, 0, 0, 236, 178, 0.3f);
		pClip->AddFrame(pImage, 0, 0, 236, 178, 0.3f);
		pClip->AddFrame(pImage, 0, 0, 236, 178, 0.3f);
		pClip->AddFrame(pImage, 0, 0, 236, 178, 0.3f);
		m_pAnimation->AddAnimationClip(pClip);
	}

	
	m_pCollider = new Collider();
	m_pCollider->SetParent(this);


}

Button::Button(Vector2 position, Vector2 scale)
{
	Init();
	SetPosition(position);
	SetScale(scale);
}

Button::~Button()
{
	SAFE_DELETE(m_pAnimation);
	SAFE_DELETE(m_pCollider);
}
void Button::Update(Matrix V, Matrix P)
{
	Vector2 position = GetPosition();

	Vector2 pos = Mouse->GetPosition();
	CAMERA->WCtoVC(pos);

	if (Mouse->Down(0))
	{
		if (IsPtInRect(pos))
		{
			if (this->m_fpButtonCallBack)
				m_fpButtonCallBack();
		}

	}
	// Hover
	if (IsPtInRect(pos))
		m_pAnimation->SetPlay(1);
	else
		m_pAnimation->SetPlay(0);



	// Animation
	SetPosition(position);
	m_pAnimation->SetPosition(position);
	m_pAnimation->SetScale(m_Scale);
	m_pAnimation->SetRotation(m_Rotation);
	m_pAnimation->Update(V, P);

	// Collider
	m_pCollider->SetPosition(position);
	m_pCollider->SetScale(m_Scale.x*m_pAnimation->GetTextureSize().x,
		                  m_Scale.y*m_pAnimation->GetTextureSize().y);
	m_pCollider->SetRotation(m_Rotation);
	m_pCollider->Update(V, P);


}

void Button::Render()
{
	m_pAnimation->Render();
	m_pCollider->Render();
}


