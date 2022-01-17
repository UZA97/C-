#include   "framework.h"
#include   "Animation.h"
#include   "Texture.h"
#include   "Renders/Shader.h"
#include   "AnimationClip.h"
///////////////////////////////////////////////////////////////
// 생성자
//////////////////////////////////////////////////////////////
Animation::Animation()
{
}
///////////////////////////////////////////////////////////////
// 소멸자
//////////////////////////////////////////////////////////////
Animation::~Animation()
{
	SAFE_DELETE(m_pTexture);

	if (m_cvAnimationClips.size())
	{
		for(UINT i=0; i< m_cvAnimationClips.size(); i++)
			delete m_cvAnimationClips[i];
		m_cvAnimationClips.erase(m_cvAnimationClips.begin(),
			m_cvAnimationClips.end());
	}

}
///////////////////////////////////////////////////////////////
//  AnimaionClip에 있는 데이터를 Update
//////////////////////////////////////////////////////////////
void Animation::Update(Matrix V, Matrix P)
{
	AnimationClip *pClip = nullptr;
	if (m_cvAnimationClips.size() == 0) return;
	if (m_nCurrentClip >= m_cvAnimationClips.size()) return;

	pClip = m_cvAnimationClips[m_nCurrentClip];

	pClip->SetPosition(m_Position);
	pClip->SetScale(m_Scale);
	pClip->SetRotation(m_Rotation);

	pClip->Update(V, P);               // Last에 Object View, Projection Update

}
///////////////////////////////////////////////////////////////
//  Render
//////////////////////////////////////////////////////////////
void Animation::Render()
{
	AnimationClip *pClip = nullptr;
	if (m_cvAnimationClips.size() == 0) return;
	if (m_nCurrentClip >= m_cvAnimationClips.size()) return;

	pClip = m_cvAnimationClips[m_nCurrentClip];
	pClip->Render();

}
///////////////////////////////////////////////////////////////
//  1. marco.png처럼 run,walk,attack etc이 한파일로 이루어진
//    경우는 Texture를 Aniamtion에 생성한다
//  2. 그렇지 않는 경우는  AnimationClip class에 있는 
//     CreateTexture()를 이용한다
//     walk.png, attack.png,
//  3. sprite 한컷씩 있는 경우 : Dead1.png ....... Dead15.png
//////////////////////////////////////////////////////////////
Texture * Animation::CreateTexture(wstring strImageFile, wstring strShaderFile)
{
	this->m_pTexture = new Texture(strImageFile, strShaderFile);
	return this->m_pTexture;
}
///////////////////////////////////////////////////////////////
// AnmationClip의 속성이 Once로 되어 있는 경우
// 현재 동작 중인가 Check
///////////////////////////////////////////////////////////////
bool Animation::IsPlay()
{
	AnimationClip *pClip = m_cvAnimationClips[m_nCurrentClip];
	return pClip->IsPlay();
}
///////////////////////////////////////////////////////////////
// VS,PS를 사용할때 technique11에 대한 tech 번호
//technique11 T0
//{
//	pass P0
//	{
//	}
//	pass P1
//	{
//	}
///////////////////////////////////////////////////////////////
void Animation::SetTech(int tech)
{
	AnimationClip *pClip = nullptr;
	if (m_cvAnimationClips.size() == 0) return;
	if (m_nCurrentClip >= m_cvAnimationClips.size()) return;

	pClip = m_cvAnimationClips[m_nCurrentClip];
	pClip->SetTech(tech);
}
///////////////////////////////////////////////////////////////
// VS,PS를 사용할때 pass에 대한 tech 번호
//technique11 T0
//{
//	pass P0
//	{
//	}
//	pass P1
//	{
//	}
///////////////////////////////////////////////////////////////
void Animation::SetPass(int pass)
{
	AnimationClip *pClip = nullptr;
	if (m_cvAnimationClips.size() == 0) return;
	if (m_nCurrentClip >= m_cvAnimationClips.size()) return;

	pClip = m_cvAnimationClips[m_nCurrentClip];
	pClip->SetPass(pass);
}
///////////////////////////////////////////////////////////////
// Shader파일에 float변수를 사용할때 데이터값 보내고자 할때
///////////////////////////////////////////////////////////////
void Animation::SetFloatConstant(string strName, float delta)
{
	AnimationClip *pClip = nullptr;
	if (m_cvAnimationClips.size() == 0) return;
	if (m_nCurrentClip >= m_cvAnimationClips.size()) return;

	pClip = m_cvAnimationClips[m_nCurrentClip];
	pClip->SetFloatConstant(strName, delta);
}
///////////////////////////////////////////////////////////////
// Player의 Update(),Render()에서 Call
///////////////////////////////////////////////////////////////
void Animation::SetPlay(UINT PlayNo)
{
	if (PlayNo == m_nCurrentClip)
		return;

	AnimationClip *pClip = nullptr;
	if (m_cvAnimationClips.size() == 0) return;
	if (m_nCurrentClip >= m_cvAnimationClips.size()) return;

	pClip = m_cvAnimationClips[m_nCurrentClip];

	pClip->SetPlay();
	m_nCurrentClip = PlayNo;

}

void Animation::SetStop()
{
	AnimationClip *pClip = nullptr;
	if (m_cvAnimationClips.size() == 0) return;
	if (m_nCurrentClip >= m_cvAnimationClips.size()) return;

	pClip = m_cvAnimationClips[m_nCurrentClip];
	pClip->SetStop();
}

Vector2 Animation::GetTextureSize()
{
	AnimationClip *pClip = m_cvAnimationClips[m_nCurrentClip];
	return pClip->GetTextureSize();
}

Vector2 Animation::GetRealTextureSize()
{
	AnimationClip *pClip = m_cvAnimationClips[m_nCurrentClip];
	return pClip->GetRealTextureSize();
}

Texture * Animation::GetTexture()
{
	AnimationClip *pClip = m_cvAnimationClips[m_nCurrentClip];
	return pClip->GetTexture();
}
