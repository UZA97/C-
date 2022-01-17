#pragma once
//////////////////////////////////////////////////
// Aniamtion  --> Image 여러장을 담을수 있는 Class
// rendering하는 class
// Animation Class
//   ---> Aniamtion Clip Class
//         --> Texture Class
//////////////////////////////////////////////////
class Animation
{
public:
	void      Update(Matrix V, Matrix P);
	void      Render();
public:      // other
	void      AddAnimationClip(class AnimationClip *pClip){m_cvAnimationClips.push_back(pClip);}
	class     Texture*  CreateTexture(wstring strImageFile, wstring strShaderFile);
	bool      IsPlay();  // 현재 Animation이 동작되고 있는가?

public:		// setter
	void		SetPosition(float x, float y) { m_Position = Vector2(x, y); }
	void		SetPosition(Vector2 position) { m_Position = position; }
	void		SetScale(float x, float y) { m_Scale = Vector2(x, y); }
	void		SetScale(Vector2 scale) { m_Scale = scale; }
	void		SetRotation(float x, float y, float z) { m_Rotation = Vector3(x, y, z); }
	void		SetRotation(Vector3 rotation) { m_Rotation = rotation; }
	void		SetTech(int tech);
	void		SetPass(int pass);
	void		SetFloatConstant(string strName, float delta);
	void        SetPlay(UINT PlayNo);    // AnimationClip 의  Index를 변경
	void        SetStop();               // 강제로 Stop하고 싶을때
public:   // getter
	Vector2		GetPosition() { return m_Position; }
	Vector2		GetScale()    { return m_Scale; }
	Vector3		GetRotation() { return m_Rotation; }
	Vector2     GetTextureSize();
	Vector2     GetRealTextureSize();
	Texture*    GetTexture();

	
private:
	Vector2      m_Scale    = Vector2(1.0f, 1.0f);        // Scale
	Vector2      m_Position = Vector2(0.0f, 0.0f);        // 좌표
	Vector3      m_Rotation = Vector3(0.0f, 0.0f, 0.0f);  // 회전
	vector<class AnimationClip*>   m_cvAnimationClips;    // IDLE,WALK,ATTACK
	UINT         m_nCurrentClip = 0;
	class        Texture    *m_pTexture = nullptr;        // 한 Image run,walk

public:
	Animation();
	~Animation();
};
