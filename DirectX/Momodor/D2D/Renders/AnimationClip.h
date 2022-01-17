#pragma once
//////////////////////////////////////////////////
// Aniamtion Clip --> Image 여러장을 담을수 있는 Class
// rendering하는 class
// Animation Class
//   ---> Aniamtion Clip Class
//         --> Texture Class
//////////////////////////////////////////////////
class AnimationClip
{
public:
	void      Update(Matrix V, Matrix P);
	void      Render();
public:      // other
	void      AddFrame(Texture *image, float startX, float startY, float endX, float endY, float delta);
	class     Texture*  CreateTexture(wstring strImageFile, wstring strShaderFile);
	void      CreateShaderResourceView(wstring strImageFile);
	bool      IsPlay() { return m_bPlay; }

public:   // setter
	void        SetPlay(){ m_nCurrentFrame = 0; m_bPlay = true; m_PlayTime = 0.0f;}
	void		SetStop() { m_bPlay = false; }
	void        SetOnce() { m_nPlayMode = 0; }
	void		SetPosition(float x, float y) { m_Position = Vector2(x, y); }
	void		SetPosition(Vector2 position) { m_Position = position; }
	void		SetScale(float x, float y) { m_Scale = Vector2(x, y); }
	void		SetScale(Vector2 scale) { m_Scale = scale; }
	void	    SetRotation(float x, float y, float z) { m_Rotation = Vector3(x, y, z); }
	void        SetRotation(Vector3 rotation) { m_Rotation = rotation; }
	void		SetTech(int tech) { if (GetTexture()) GetTexture()->SetTech(tech); }
	void		SetPass(int pass) { if (GetTexture()) GetTexture()->SetPass(pass); }
	void		SetFloatConstant(string strName, float delta)
				{
				if (GetTexture())
					GetTexture()->SetFloatConstant(strName, delta);
				};
public:   // getter
	Vector2		GetPosition() { return m_Position; }
	Vector2		GetScale() { return m_Scale; }
	Vector3		GetRotation() { return m_Rotation; }
	Vector2     GetTextureSize();
	Vector2     GetRealTextureSize();     
	Texture*    GetTexture();



private:
	struct Frame    // Texture를 1컷씩 담는 구조체
	{
		class    Texture *m_Image = nullptr;
		Vector2  m_Offset = Vector2(0.0f, 0.0f);  // Image파일내에 그림 시작위치
		Vector2  m_OffsetSize = Vector2(1.0f, 1.0f);  // Image파일내에 그림 시작위치
		wstring  m_strImageFile;
		float    m_Time = 0.0f;
	};
	vector<Frame*>  m_cvFrames;  // struct Frame 담는 구조체
private:
	Vector2      m_Scale    = Vector2(1.0f, 1.0f);        // Scale
	Vector2      m_Position = Vector2(0.0f, 0.0f);        // 좌표
	Vector3      m_Rotation = Vector3(0.0f, 0.0f, 0.0f);  // 회전
	UINT         m_nCurrentFrame = 0;  // struct Frame No  --> Display No
	UINT         m_nPlayMode = 1;      // loop(1), Once(0)
	float        m_PlayTime = 0.0f;    // Update시에 시간 Check용
	bool         m_bPlay = true;       // Animation이 Play중일때는 true, Attack이 끝난는가 Check

private:
	class        Texture       *m_pTexture = nullptr;  // attack png파일 하나, 모션이 image에 있는 경우
	ID3D11ShaderResourceView   *m_pSRV     = nullptr;

public:
	AnimationClip();
	~AnimationClip();
};
